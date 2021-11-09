#include <iostream>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <string>
#include <thread>

#include "message_buffer.h"



int main()
{

    key_t key = ftok("queue", 65);

    int msgid = msgget(key, 0666 | IPC_CREAT);

    pid_t p = getpid();

    MessageBuffer message;
    std::string str = std::to_string(p);
    strncpy(message.msg_text, str.c_str(), MAX_BUFFER);
    message.msg_type = 1;
    msgsnd(msgid, &message, sizeof message, 0);

    std::thread sender([msgid]()
                       {
                           MessageBuffer message;
                           while (true)
                           {
                               std::cin.getline(message.msg_text, MAX_BUFFER);
                               message.msg_type = 2;
                               msgsnd(msgid, &message, sizeof message, 0);
                           }
                       });

    std::thread receiver([msgid, p]()
                         {
                             MessageBuffer message;
                             while (true)
                             {
                                 msgrcv(msgid, &message, sizeof message, p, 0);
                                 std::cout << message.msg_text << std::endl;
                             }
                         });

    sender.join();
    receiver.join();
}