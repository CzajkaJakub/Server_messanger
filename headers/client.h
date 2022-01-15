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
} requestToSendToServer, requestFromServerToReceive;


void clearConsole();
char readResponseFromUser();
void registerNewUser();
long readIdFromUser();
bool checkAvailabilityOfId();
_Noreturn void userMenu();
void changeRequestType(int type);
void setIdAsMessage();
void sendRequestToServer();
void showAllAccounts();
void registeredStatus();
void unregisteredStatus();



#endif //INTER_SERVER_MESSENGER_CLIENT_H
