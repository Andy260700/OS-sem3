#include "buffer.hpp"
#include "producer.hpp"
#include "consumer.hpp"
#include <iostream>

pid_t spawn_process(std::function<void()> &&function);

int main()
{
    Buffer<int> buffer(5);

    spawn_process([&buffer]() 
    {
        Producer producer(buffer);
        for(int i = 0; i < 10; ++i)
        {
            producer.produce(i + 1);
        };
    });

    spawn_process([&buffer]() 
    {
        Consumer consumer(buffer);
        for(int i = 0; i < 10; ++i)
        {
            consumer.consume();
            sleep(1);
        };
    });

    wait(NULL);
    wait(NULL);
}

pid_t spawn_process(std::function<void()> &&function)
{
    pid_t pid = fork();
    if (pid == 0)
    {
        function();
        exit(0);
    }
    if (pid == -1)
        std::cout << "ERROR" << std::endl;
    return pid;
}