//
// Created by Patrycja on 02/01/2022.
//

#ifndef INTER_SERVER_MESSENGER_SERVER_H
#define INTER_SERVER_MESSENGER_SERVER_H

struct serverMessage
{
    long type;
    char message[1024];
} serverRequestToReceive, serverMessageToSendToClient;



#endif //INTER_SERVER_MESSENGER_SERVER_H
