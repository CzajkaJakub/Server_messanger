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

_Noreturn void startWorking();
long checkRequest();
void registerNewUser();
void changeTypeOfMessageToSend(long type);
bool checkAvailabilityOfIdAndSave();
void sendRequestToClient();
void changeMessageInRequest(const char *message);
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


#endif //INTER_SERVER_MESSENGER_SERVER_H
