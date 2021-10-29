#include <iostream>
#include "../include/Reader.hpp"
#include "../include/Writer.hpp"
#include "../include/Resource.hpp"
#include <functional>
#include <sys/wait.h>
#include <random>
#include <unistd.h>

pid_t spawn_process(std::function<void()> &&);

using std::cout, std::endl;

int main()
{
    Resource r(5);

    for (int i = 0; i < 5; ++i)
    {
        spawn_process(
            [&r]()
            {
                Writer writer(r);
                std::random_device dev;
                std::mt19937 rng(dev());
                std::uniform_int_distribution<std::mt19937::result_type> dist6(1, 6);
                for (int i = 0; i < 10; ++i)
                {
                    sleep(dist6(rng));
                    writer.write(i);
                }
            });
    }

    for (int i = 0; i < 5; ++i)
    {
        spawn_process(
            [&r]()
            {
                Reader reader(r);
                std::random_device dev;
                std::mt19937 rng(dev());
                std::uniform_int_distribution<std::mt19937::result_type> dist6(1, 6);
                for (int i = 0; i < 10; ++i)
                {
                    sleep(dist6(rng));
                    cout << reader.read() << endl;
                }
            });
    }

    // cout << r.get();

    for (int i = 0; i < 10; ++i)
    {
        wait(NULL);
    }
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
        cout << "ERROR" << endl;
    return pid;
}