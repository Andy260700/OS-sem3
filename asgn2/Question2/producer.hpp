#ifndef PRODUCER_HPP
#define PRODUCER_HPP
#include "buffer.hpp"

template <typename T>
class Producer
{
private:
    Buffer<T> &m_buffer;
    sem_t *resource_semaphore;
    sem_t *producer_semaphore;
    sem_t *consumer_semaphore;

public:
    Producer(Buffer<T> &buffer);
    ~Producer();
    void produce(T value);
};

template <typename T>
Producer<T>::Producer(Buffer<T> &buffer) : m_buffer(buffer)
{
    producer_semaphore = sem_open(PRODUCER_SEMAPHORE, 0);

    if (producer_semaphore == SEM_FAILED)
    {
        std::cerr << "Producer Semaphore Failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    consumer_semaphore = sem_open(CONSUMER_SEMAPHORE, 0);

    if (consumer_semaphore == SEM_FAILED)
    {
        std::cerr << "Consumer Semaphore Failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    resource_semaphore = sem_open(RESOURCE_SEMAPHORE, 0);

    if (resource_semaphore == SEM_FAILED)
    {
        std::cerr << "Resource Semaphore Failed" << std::endl;
        exit(EXIT_FAILURE);
    }
}
template <typename T>
Producer<T>::~Producer()
{
    sem_close(producer_semaphore);
    sem_close(consumer_semaphore);
    sem_close(resource_semaphore);
}

template <typename T>
void Producer<T>::produce(T value)
{
    sem_wait(producer_semaphore);
    sem_wait(resource_semaphore);
    std::cout << getpid() << " produced: " << value << std::endl;
    m_buffer.push(value);
    sem_post(consumer_semaphore);
    sem_post(resource_semaphore);
}


#endif