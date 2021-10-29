#ifndef WRITER_HPP
#define WRITER_HPP
#include "Resource.hpp"
#include <semaphore.h>

template <typename T>
class Writer
{
private:
    Resource<T> &resource;
    sem_t *no_reader_semaphore;
    sem_t *no_writer_semaphore;
    sem_t *read_try;
    sem_t *resource_semaphore;

public:
    Writer(Resource<T> &res);
    ~Writer();
    void write(T val);
};

template <typename T>
void Writer<T>::write(T val)
{
    sem_wait(no_writer_semaphore);
    resource.startWriting();
    if(resource.getWriterCount() == 1)
    {
        sem_wait(read_try);
    }
    sem_post(no_writer_semaphore);
    sem_wait(resource_semaphore);
    resource.set(val);
    sem_post(resource_semaphore);
    sem_wait(no_writer_semaphore);
    resource.endWriting();
    if(resource.getWriterCount() == 0)
    {
        sem_post(read_try);
    }
    sem_post(no_writer_semaphore);
};

template <typename T>
Writer<T>::Writer(Resource<T> &res) : resource(res)
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
Writer<T>::~Writer()
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