//
// Created by Patrycja on 02/01/2022.
//

#ifndef INTER_SERVER_MESSENGER_SERVER_H
#define INTER_SERVER_MESSENGER_SERVER_H

int queue;
int maxRooms = 5;
long canal[5][5];
int maxUsers = 5;
long users[5];

struct serverMessage
{
    long type;
    char message[1024];
} serverRequestToReceive, serverMessageToSendToClient;

_Noreturn void startWorking();
long checkRequest();
void registerNewUser();
bool checkAvailabilityOfIdAndSave();
void sendRequestToClient(long type, const char *message);
void showAllAccount();
long getIdFromMessage();
void deregisterUserFromTheServer();
void registerUserToRoom();
void removeUserFromRoom(long userId);
void receiveUsersDecision(long userId);
void addUserToRoom(long id);
void showAllRooms();
void showAllRoomsWithUsers();
void sendMessageToUser();
void sendMessageToAllUsersInRoom();



#endif //INTER_SERVER_MESSENGER_SERVER_H
