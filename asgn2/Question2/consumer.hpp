#ifndef CONSUMER_HPP
#define CONSUMER_HPP

#include "buffer.hpp"

template <typename T>
class Consumer
{
private:
    Buffer<T> &m_buffer;
    sem_t *resource_semaphore;
    sem_t *producer_semaphore;
    sem_t *consumer_semaphore;

public:
    Consumer(Buffer<T> &buffer);
    ~Consumer();
    T consume();
};

template <typename T>
Consumer<T>::Consumer(Buffer<T> &buffer) : m_buffer(buffer)
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
Consumer<T>::~Consumer()
{
    sem_close(producer_semaphore);
    sem_close(consumer_semaphore);
    sem_close(resource_semaphore);
}

template <typename T>
T Consumer<T>::consume()
{
    sem_wait(consumer_semaphore);
    sem_wait(resource_semaphore);
    T result = m_buffer.pop();
    std::cout << "Consumed: " << result << std::endl;
    sem_post(producer_semaphore);
    sem_post(resource_semaphore);
    return result;
}
#endif