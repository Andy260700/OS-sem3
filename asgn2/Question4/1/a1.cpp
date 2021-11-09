#include <iostream>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_BUFFER 1024

using std::vector;

int spawn_process(std::function<void(int)> &&function);

int main()
{
    vector<int> handles;

    for (int i = 0; i < 10; ++i)
    {
        int handle = spawn_process([](int fd)
                                   {
                                       char msg[MAX_BUFFER];
                                       read(fd, msg, MAX_BUFFER);
                                       pid_t id = getpid();
                                       std::cout << "Process " << id << " received: ";
                                       std::cout << msg << std::endl;
                                       close(fd);
                                   });
        handles.push_back(handle);
    }

    char buffer[MAX_BUFFER];
    std::cin.getline(buffer, MAX_BUFFER);

    for (auto handle : handles)
    {
        write(handle, buffer, 1024);
        close(handle);
    }

    for (int i = 0; i < 10; ++i)
    {
        wait(NULL);
    }
}

int spawn_process(std::function<void(int)> &&function)
{
    int fd[2];
    pipe(fd);
    pid_t pid = fork();
    if (pid == 0)
    {
        close(fd[1]);
        function(fd[0]);
        exit(0);
    }
    close(fd[0]);
    if (pid == -1)
        std::cout << "ERROR" << std::endl;

    return fd[1];
}