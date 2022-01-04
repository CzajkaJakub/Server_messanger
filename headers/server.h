//
// Created by Patrycja on 02/01/2022.
//

#ifndef INTER_SERVER_MESSENGER_SERVER_H
#define INTER_SERVER_MESSENGER_SERVER_H

struct message
{
    long type;
    char message[1024];
} registrationServer, responseServer;

void readServerSettings();
_Noreturn void registerNewUsers();
int checkAvailabilityOfId();
void saveUserIdInServerMemory();

#endif //INTER_SERVER_MESSENGER_SERVER_H
