//
// Created by Patrycja on 02/01/2022.
//

#ifndef INTER_SERVER_MESSENGER_CLIENT_H
#define INTER_SERVER_MESSENGER_CLIENT_H

#include <stdbool.h>

long userId;
int queue;
bool registered = false;
long privateTypeToReceiveMessagesFromServer;

struct message
{
    long type;
    char message[1024];
} requestToSendToServer, requestFromServerToReceive;

void clearConsole();
char readResponseFromUser();
void registerNewUser();
long readIdFromUser();
bool checkAvailabilityOfIdAndSave();
void userMenu();
void changeRequestType(long type);
void setIdAsMessage();
void sendRequestToServer(long type);
void showAllAccounts();
void registeredStatus();
void unregisteredStatus();
void deregisterFromTheServer();
_Noreturn void startReceivingMessagesFromServer();
void registerToRoom();
long readCorrectId();
void showAllRooms();
void showAllRoomsWithUsers();
void sendMessageToUser();
void changeMessageInRequest(const char *message);
void sendMessageToAllUsersInRoom();
void sendRegistrationForm();




#endif //INTER_SERVER_MESSENGER_CLIENT_H
