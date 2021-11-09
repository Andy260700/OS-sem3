#include <iostream>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <thread>
#include <vector>
#include <mutex>
#include <signal.h>

#include "message_buffer.h"

std::mutex mut;
std::vector<pid_t> pids;
int msgid;

int main()
{

    key_t key = ftok("queue", 65);

    if (key == -1)
    {
        std::cerr << "Error" << std::endl;
        exit(0);
    }
    msgid = msgget(key, 0666 | IPC_CREAT);

    signal(SIGINT, [](int _) {
        msgctl(msgid, IPC_RMID, NULL);
        exit(0);
    });

    std::thread notifier([]()
                         {
                             MessageBuffer message;
                             while (true)
                             {
                                 msgrcv(msgid, &message, sizeof message, 1, 0);
                                 std::cout << "Connected :" << message.msg_text << std::endl;
                                 std::lock_guard<std::mutex> lock(mut);
                                 pids.push_back(std::stoi(message.msg_text));
                             }
                         });

    std::thread sender([]()
                       {
                           MessageBuffer message;
                           std::cin.getline(message.msg_text, MAX_BUFFER);
                           std::lock_guard<std::mutex> lock(mut);
                           for (auto id : pids)
                           {
                               message.msg_type = id;
                               msgsnd(msgid, &message, sizeof message, 0);
                           }
                       });

    std::thread receiver([]()
                         {
                             MessageBuffer message;
                             while (true)
                             {
                                 msgrcv(msgid, &message, sizeof message, 2, 0);
                                 std::cout << message.msg_text << std::endl;
                             }
                         });

    notifier.join();
    sender.join();
    receiver.join();
}