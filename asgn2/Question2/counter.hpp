#ifndef COUNTER_HPP
#define COUNTER_HPP

#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
#include <sys/shm.h>
#include <sys/types.h>
#include <fcntl.h>
#include <iostream>
#include <semaphore.h>
class Counter
{
    private:
        int *count;
    public:
        Counter()
        {
            count = (int *)mmap(NULL, sizeof(int), PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
            *count = 0;
        }
        ~Counter()
        {
            munmap(count, sizeof(int));
        }
        
        void add(int val)
        {
            *count += val;
        }

        int get()
        {
            return *count;
        }
};
#endif