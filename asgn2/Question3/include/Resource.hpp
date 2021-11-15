#ifndef RESOURCE_HPP
#define RESOURCE_HPP
#define READER_SEMAPHORE "counting_semaphore"
#define WRITER_SEMAPHORE "writing_semaphore"
#define READ_TRY "readtry"
#define RESOURCE_SEMAPHORE "resource"
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
#include <sys/shm.h>
#include <sys/types.h>
#include <fcntl.h>
#include <iostream>
#include <semaphore.h>

template <typename T>
class Resource
{
    struct shared_mem
    {
        T val;
        int reader_count = 0;
        int writer_count = 0;
    };

private:
    shared_mem *ptr;
    sem_t *no_reader_semaphore;
    sem_t *no_writer_semaphore;
    sem_t *read_try;
    sem_t *resource_semaphore;

public:
    Resource(T val);
    Resource(Resource<T> const &res);
    ~Resource();
    T get() const;
    void set(T val);
    int getReaderCount() const;
    void startReading() const;
    void endReading() const;
    int getWriterCount() const;
    void startWriting() const;
    void endWriting() const;
};

template <typename T>
Resource<T>::Resource(Resource<T> const &res)
{
    ptr = res.ptr;
}

template <typename T>
Resource<T>::Resource(T val)
{
    sem_unlink(READER_SEMAPHORE);
    sem_unlink(WRITER_SEMAPHORE);
    sem_unlink(READ_TRY);
    sem_unlink(RESOURCE_SEMAPHORE);
    no_reader_semaphore = sem_open(READER_SEMAPHORE, O_CREAT, 0660, 1);
    if (no_reader_semaphore == SEM_FAILED)
    {
        std::cout << "ERROR";
        exit(1);
    }
    no_writer_semaphore = sem_open(WRITER_SEMAPHORE, O_CREAT, 0660, 1);
    if (no_writer_semaphore == SEM_FAILED)
    {
        std::cerr << "ERROR";
        exit(1);
    }

    read_try = sem_open(READ_TRY, O_CREAT, 0660, 1);
    if (read_try == SEM_FAILED)
    {
        std::cout << "OUT";
        exit(1);
    }

    resource_semaphore = sem_open(RESOURCE_SEMAPHORE, O_CREAT, 0660, 1);
    if (resource_semaphore == SEM_FAILED)
    {
        std::cerr << "ERROR";
        exit(1);
    }
    ptr = (shared_mem *)mmap(NULL, sizeof(shared_mem), PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    if (ptr == NULL)
        std::cerr << "ERROR";
    ptr->val = val;
}

template <typename T>
Resource<T>::~Resource()
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
    munmap(ptr, sizeof(shared_mem));
}

template <typename T>
T Resource<T>::get() const
{
    // std::cout << ptr->val;
    return ptr->val;
}

template <typename T>
int Resource<T>::getReaderCount() const
{
    return ptr->reader_count;
}

template <typename T>
int Resource<T>::getWriterCount() const
{
    return ptr->writer_count;
}

template <typename T>
void Resource<T>::startReading() const
{
    ++ptr->reader_count;
}

template <typename T>
void Resource<T>::startWriting() const
{
    ++ptr->writer_count;
}

template <typename T>
void Resource<T>::endReading() const
{
    --ptr->reader_count;
}

template <typename T>
void Resource<T>::endWriting() const
{
    --ptr->writer_count;
}

template <typename T>
void Resource<T>::set(T val)
{
    ptr->val = val;
}

#endif