//
// Created by Jakub on 02/01/2022.
//

#ifndef INTER_SERVER_MESSENGER_SERVER_H
#define INTER_SERVER_MESSENGER_SERVER_H

int queue;
int maxRooms = 5;
long canal[5][5];
int maxUsers = 5;
long users[5];

char history1[100][1024];
char history2[100][1024];
char history3[100][1024];
char history4[100][1024];
char history5[100][1024];

int counters[5] = {0, 0, 0, 0, 0};

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
void saveMessage(int i, char* message);



#endif //INTER_SERVER_MESSENGER_SERVER_H
