#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
#include <sys/shm.h>
#include <sys/types.h>
#include <fcntl.h>
#include <iostream>
#include <semaphore.h>

#define PRODUCER_SEMAPHORE "producer_semaphore"
#define CONSUMER_SEMAPHORE "consumer_semaphore"
#define RESOURCE_SEMAPHORE "resource_semaphore"

template <typename T>
class Buffer
{
private:
    T *arr;
    const size_t m_size;
    size_t *front;
    size_t *rear;
    sem_t *resource_semaphore;
    sem_t *producer_semaphore;
    sem_t *consumer_semaphore;

public:
    Buffer(size_t size);
    ~Buffer();
    void push(T value);
    T pop();
    bool isFull();
    bool isEmpty();
};

template <typename T>
Buffer<T>::Buffer(size_t size) : m_size(size)
{
    arr = (T *)mmap(nullptr, m_size * sizeof(T) + sizeof(size_t) + sizeof(size_t), PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    front = (size_t *)((char *) arr + m_size * sizeof(T));
    rear = (front + 1);
    *front = *rear = -1;
    if(arr == nullptr)
    {
        std::cerr << "MMAP Failure" << std::endl;
        exit(EXIT_FAILURE);
    }

    sem_unlink(PRODUCER_SEMAPHORE);
    sem_unlink(CONSUMER_SEMAPHORE);
    sem_unlink(RESOURCE_SEMAPHORE);

    producer_semaphore = sem_open(PRODUCER_SEMAPHORE, O_CREAT, 0660, m_size);

    if (producer_semaphore == SEM_FAILED)
    {
        std::cerr << "Producer Semaphore Failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    consumer_semaphore = sem_open(CONSUMER_SEMAPHORE, O_CREAT, 0660, 0);

    if (consumer_semaphore == SEM_FAILED)
    {
        std::cerr << "Consumer Semaphore Failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    resource_semaphore = sem_open(RESOURCE_SEMAPHORE, O_CREAT, 0660, 1);
    
    if(resource_semaphore == SEM_FAILED)
    {
        std::cerr << "Resource Semaphore Failed" << std::endl;
        exit(EXIT_FAILURE);
    }
}

template <typename T>
Buffer<T>::~Buffer()
{
    munmap(arr, m_size * sizeof(T));

    sem_close(producer_semaphore);
    sem_close(consumer_semaphore);
    sem_close(resource_semaphore);

    sem_unlink(PRODUCER_SEMAPHORE);
    sem_unlink(CONSUMER_SEMAPHORE);
    sem_unlink(RESOURCE_SEMAPHORE);
}

template <typename T>
void Buffer<T>::push(T value)
{
    if (isFull())
    {
        return;
    }
    if (isEmpty())
    {
        *front = 0;
        *rear = 0;
        arr[0] = value;
    }
    else
    {
        *rear = (*rear + 1) % m_size;
        arr[*rear] = value;
    }
}

template <typename T>
T Buffer<T>::pop()
{
    if (isEmpty())
    {
        return -1;
    }

    T result = arr[*front];

    if (*front == *rear)
    {
        *front = -1;
        *rear = -1;
    }
    else
    {
        *front = (*front + 1) % m_size;
    }
    return result;
}

template <typename T>
bool Buffer<T>::isFull()
{
    return (*rear + 1) % m_size == *front;
}

template <typename T>
bool Buffer<T>::isEmpty()
{
    return *front == -1 && *rear == -1;
}
#endif
