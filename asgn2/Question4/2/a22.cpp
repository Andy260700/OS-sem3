#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <thread>
#define BUFFER_SIZE 1024

int main()
{

    int fd1 = open("file1.fifo", O_RDONLY);
    int fd2 = open("file2.fifo", O_WRONLY);

    std::thread sender([fd2]()
                       {
                           char send_buffer[BUFFER_SIZE];
                           while (true)
                           {
                               std::cin.getline(send_buffer, BUFFER_SIZE);
                               if (write(fd2, send_buffer, BUFFER_SIZE) == 0)
                                   exit(0);
                           }
                       });

    std::thread receiver([fd1]()
                         {
                             char receive_buffer[BUFFER_SIZE];
                             while (true)
                             {
                                 if (read(fd1, receive_buffer, BUFFER_SIZE) == 0)
                                     exit(0);
                                 std::cout << receive_buffer << std::endl;
                             }
                         });

    sender.join();
    receiver.join();

    close(fd1);
    close(fd2);
}