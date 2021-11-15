#include "buffer.hpp"
#include "producer.hpp"
#include "consumer.hpp"
#include <iostream>

pid_t spawn_process(std::function<void()> &&function);

int main()
{
    std::cout << "Enter the size of the buffer: ";
    size_t buffer_size;
    std::cin >> buffer_size;
    Buffer<int> buffer(buffer_size);

    int p, c;
    std::cout << "Enter the number of producers: ";
    std::cin >> p;
    std::cout << "Enter the number of consumers: ";
    std::cin >> c;

    Counter counter;

    for (int i = 0; i < p; ++i)
    {
        spawn_process([&buffer, c]()
                      {
                          Producer producer(buffer);
                          for (int i = 0; i < c; ++i)
                          {
                              producer.produce(i + 1);
                          };
                      });
    }

    for (int i = 0; i < c; ++i)
    {
        spawn_process([&buffer, p, &counter]()
                      {
                          Consumer consumer(buffer);
                          for (int i = 0; i < p; ++i)
                          {
                              consumer.consume(counter);
                          };
                      });
    }

    for(int i = 0; i < (p + c); ++i)
    {
        wait(NULL);
    }

    std::cout << counter.get() << std::endl;
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