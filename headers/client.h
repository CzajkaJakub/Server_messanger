//
// Created by Patrycja on 02/01/2022.
//

#ifndef INTER_SERVER_MESSENGER_CLIENT_H
#define INTER_SERVER_MESSENGER_CLIENT_H

#include <stdbool.h>

struct clientMessage
{
    long type;
    char message[1024];
} clientRegistrationToSendToServer, clientRegistrationToReceiveFromServer;





#endif //INTER_SERVER_MESSENGER_CLIENT_H
