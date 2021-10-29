#ifndef READER_HPP
#define READER_HPP
#include "Resource.hpp"
#include <semaphore.h>

template <typename T>
class Reader
{
private:
    Resource<T> &resource;
    sem_t *no_reader_semaphore;
    sem_t *no_writer_semaphore;
    sem_t *read_try;
    sem_t *resource_semaphore;

public:
    Reader(Resource<T> &res);
    ~Reader();
    T read() const;
};

template <typename T>
Reader<T>::Reader(Resource<T> &res) : resource(res)
{
    no_reader_semaphore = sem_open(READER_SEMAPHORE, 0);
    if (no_reader_semaphore == SEM_FAILED)
    {
        std::cout << "ERROR";
        exit(1);
    }
    no_writer_semaphore = sem_open(WRITER_SEMAPHORE, 0);
    if (no_writer_semaphore == SEM_FAILED)
    {
        std::cerr << "ERROR";
        exit(1);
    }

    read_try = sem_open(READ_TRY, 0);
    if (read_try == SEM_FAILED)
    {
        std::cout << "OUT";
        exit(1);
    }

    resource_semaphore = sem_open(RESOURCE_SEMAPHORE, 0);
    if (resource_semaphore == SEM_FAILED)
    {
        std::cerr << "ERROR";
        exit(1);
    }
}

template <typename T>
T Reader<T>::read() const
{
    sem_wait(read_try);
    sem_wait(no_reader_semaphore);
    resource.startReading();
    if(resource.getReaderCount() == 1)
    {
        sem_wait(resource_semaphore);
    }
    sem_post(no_reader_semaphore);
    sem_post(read_try);

    int val = resource.get();

    sem_wait(no_reader_semaphore);
    resource.endReading();
    if(resource.getReaderCount() == 0)
    {
        sem_post(resource_semaphore);
    }
    sem_post(no_reader_semaphore);
    return val;
}

template <typename T>
Reader<T>::~Reader()
{
    if (no_reader_semaphore != NULL)
        sem_close(no_reader_semaphore);
    if (no_writer_semaphore != NULL)
        sem_close(no_writer_semaphore);
    if (read_try != NULL)
        sem_close(read_try);
    if (resource_semaphore != NULL)
    {
        sem_close(resource_semaphore);
    }
}

#endif