//
// Created by Patrycja on 02/01/2022.
//

#ifndef INTER_SERVER_MESSENGER_CLIENT_H
#define INTER_SERVER_MESSENGER_CLIENT_H

#include <stdbool.h>

struct message
{
    long type;
    char message[1024];
} clientRegistration, clientResponse;

void registerNewUser();
void readSettings();
char readResponseFromUser();
long readIdFromUser();
long checkAvailabilityOfId(long tempId);
void userMenu();




#endif //INTER_SERVER_MESSENGER_CLIENT_H
