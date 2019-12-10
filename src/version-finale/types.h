//
// Created by naysson on 22/11/2019.
//

#ifndef TP3_TYPES_H
#define TP3_TYPES_H

#define BUFFER_MESSAGE_LENGTH 100

struct message_buffer
{
    long mtype;
    char mtext[BUFFER_MESSAGE_LENGTH];
    long pid;
};

#endif //TP3_TYPES_H
