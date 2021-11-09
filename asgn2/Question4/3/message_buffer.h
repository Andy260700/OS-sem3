#ifndef MESSAGE_BUFFER
#define MESSAGE_BUFFER
#define MAX_BUFFER 1024

struct MessageBuffer
{
    long msg_type;
    char msg_text[MAX_BUFFER];
};

#endif
