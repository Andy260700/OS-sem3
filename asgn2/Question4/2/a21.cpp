#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <thread>
#include <signal.h>

#define BUFFER_SIZE 1024

int flag = true;
int fd1;
int fd2;
void signal_handler(int);

int main()
{
    
    unlink("file1.fifo");
    unlink("file2.fifo");
    mkfifo("file1.fifo", 0666);
    mkfifo("file2.fifo", 0666);
    fd1 = open("file1.fifo", O_WRONLY);
    fd2 = open("file2.fifo", O_RDONLY);
    signal(SIGINT, signal_handler);

    std::thread sender([]()
                       {
                           char send_buffer[BUFFER_SIZE];
                           while (flag)
                           {
                               std::cin.getline(send_buffer, BUFFER_SIZE);
                               if (write(fd1, send_buffer, BUFFER_SIZE) == 0)
                               {
                                   flag = false;
                               }
                           }
                       });

    std::thread receiver([]()
                         {
                             char receive_buffer[BUFFER_SIZE];
                             while (flag)
                             {
                                 if (read(fd2, receive_buffer, BUFFER_SIZE) == 0)
                                 {
                                     flag = false;
                                     break;
                                 }
                                 std::cout << receive_buffer << std::endl;
                             }
                         });

    receiver.join();
    sender.join();
    close(fd1);
    close(fd2);
    unlink("file1.fifo");
    unlink("file2.fifo");
}

void signal_handler(int i)
{
    close(fd1);
    close(fd2);
    unlink("file1.fifo");
    unlink("file2.fifo");
    exit(0);
}