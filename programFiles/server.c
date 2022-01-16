//
// Created by Jakub on 25/12/2021.
//

#include <stdio.h>
#include <sys/msg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../headers/server.h"

int serverKey = 3334444;
int queue;

int clientRegistrationToSendToServerType = 11;
int clientRegistrationToSendToClientType = 12;
int showAllAccountsType = 13;
int deregisterFromServerType = 14;
int registerToRoomType = 15;
int showAllRoomsType = 16;
int showAllUsersInAllRooms = 17;

int sendMessageToUserInRoomType = 18;
int sendMessageToAllUsersInRoomType = 19; // todo





//pokoje

int maxRooms = 5;
long canal[5][5];



int maxUsers = 5;
long users[5];


void registerUserToRoom();
void removeUserFromRoom(long userId);
void receiveUsersDecision(long userId);

void addUserToRoom(long id);

void showAllRooms();

void showAllRoomsWithUsers();

void sendMessageToUser();

int main(){
    startWorking();
}

_Noreturn void startWorking() {
    queue = msgget(serverKey, 0644 | IPC_CREAT);
    while (1) {
        long requestType = checkRequest();

        switch (requestType) {
            case 11 : registerNewUser(); break;
            case 13 : showAllAccount(); break;
            case 14 : deregisterUserFromTheServer(); break;
            case 15 : registerUserToRoom(); break;
            case 16 : showAllRooms(); break;
            case 17 : showAllRoomsWithUsers(); break;
            case 18 : sendMessageToUser(); break;
            default: printf("Other data type"); fflush(stdout); break;
        }

        sleep(3);
    }
}

void sendMessageToUser() {
    long senderId;

    for(int i = 0; i < maxUsers; i++){
        if(getIdFromMessage() == users[i]){
            senderId = users[i];
        }
    }

    msgrcv(queue, &serverRequestToReceive, sizeof(serverRequestToReceive.message), sendMessageToUserInRoomType, 0);
    long userToSendId = getIdFromMessage();
    msgrcv(queue, &serverRequestToReceive, sizeof(serverRequestToReceive.message), sendMessageToUserInRoomType, 0);

    char message[1024];
    sprintf(message, "User %ld send a message %s", senderId, serverRequestToReceive.message);

    printf("%ld sends to do %ld a message : %s\n", senderId, userToSendId, message);
    fflush(stdout);

    changeTypeOfMessageToSend(userToSendId);
    changeMessageInRequest(message);
    sendRequestToClient();
}

void showAllRoomsWithUsers() {
    changeTypeOfMessageToSend(getIdFromMessage());
    char message[1024];

    for(int i = 0; i < maxRooms; i++){

        char room[256] = "\nRoom ";
        char tempI[2];
        sprintf(tempI, "%d", i + 1);
        strcat(room, tempI);
        strcat(room, " Users :");

        for(int j = 0; j < maxUsers; j++){
            char tempUserId[10];
            sprintf(tempUserId, " %ld ", canal[i][j]);
            strcat(room, tempUserId);
        }
        strcat(message, room);
    }

    changeMessageInRequest(message);
    sendRequestToClient();

}

void showAllRooms() {
    changeTypeOfMessageToSend(getIdFromMessage());
    changeMessageInRequest("\n'Room 1'\n'Room 2'\n'Room 3'\n'Room 4'\n'Room 5'\n");
    sendRequestToClient();
}


long checkRequest() {
    msgrcv(queue, &serverRequestToReceive, sizeof(serverRequestToReceive.message), 0, 0);
    printf("Otrzymano requesta typ %ld, wiadomosc %s\n", serverRequestToReceive.type, serverRequestToReceive.message);
    fflush(stdout);
    return serverRequestToReceive.type;
}

void registerNewUser() {
    changeTypeOfMessageToSend(clientRegistrationToSendToClientType);
    if(checkAvailabilityOfIdAndSave()){
        changeMessageInRequest("Available");
        printf("%s - user id is %s - registration successful\n", serverRequestToReceive.message, serverMessageToSendToClient.message);
    } else {
        changeMessageInRequest("Unavailable");
        printf("%s - user id is %s - registration failed\n", serverRequestToReceive.message, serverMessageToSendToClient.message);
    }
    sendRequestToClient();
    fflush(stdout);
}


bool checkAvailabilityOfIdAndSave() {
    long tempUserId = getIdFromMessage();

    for (int i = 0; i < maxUsers; i++) {
        if (users[i] == tempUserId) {
            return false;
        }
    }

    for(int i = 0; i < maxUsers; i++){
            if(users[i] == 0){
                users[i] = tempUserId;
                return true;
            }
        }

    return false;
}

long getIdFromMessage(){
    return strtol(serverRequestToReceive.message, NULL, 0);
}



void changeTypeOfMessageToSend(long type) {
    serverMessageToSendToClient.type = type;
}

void changeMessageInRequest(const char *message) {
    strcpy(serverMessageToSendToClient.message, message);
}

void sendRequestToClient() {
    msgsnd(queue, &serverMessageToSendToClient, sizeof(serverMessageToSendToClient.message), IPC_NOWAIT);
}

void showAllAccount() {
    changeTypeOfMessageToSend(getIdFromMessage());

    char allAccountsMessage[1024] = "";
    for(int i = 0; i < maxUsers; i++){
        if(users[i] != 0){
            char tempId[100];
            sprintf(tempId, "\nUser's id : %ld", users[i]);
            strcat(allAccountsMessage, tempId);
        }
    }

    changeMessageInRequest(allAccountsMessage);
    sendRequestToClient();
}

void deregisterUserFromTheServer() {
    changeTypeOfMessageToSend(getIdFromMessage());

    for(int i = 0; i < maxUsers; i++){
        if(users[i] == getIdFromMessage()){
            users[i] = 0;
        }
    }
    removeUserFromRoom(getIdFromMessage());

    changeMessageInRequest("You have been disconnected");
    sendRequestToClient();
}

void registerUserToRoom() {
    long tempId = getIdFromMessage();
    removeUserFromRoom(tempId);
    changeTypeOfMessageToSend(tempId);
    changeMessageInRequest("Select the room number : 'Room 1 - 1', 'Room 2 - 2', 'Room 3 - 3', 'Room 4 - 4', 'Room 5 - 5'");

    printf("Wysylam do %ld - %s", serverMessageToSendToClient.type, serverMessageToSendToClient.message);
    msgsnd(queue, &serverMessageToSendToClient, sizeof(serverMessageToSendToClient.message), 0);
    sleep(1);
    receiveUsersDecision(tempId);
    addUserToRoom(tempId);
}


void removeUserFromRoom(long userId) {
    for(int i = 0; i < maxRooms; i++){
        for(int j = 0; j < maxUsers; j++){
            if(canal[i][j] == userId){
                canal[i][j] = 0;
            }
        }
    }
}

void receiveUsersDecision(long userId) {
    msgrcv(queue, &serverRequestToReceive, sizeof(serverRequestToReceive.message), userId + 100, 0);
    fflush(stdout);
}

void addUserToRoom(long id) {
    long roomToJoin = strtol(serverRequestToReceive.message, NULL, 0) - 1;

    for(int i = 0; i < maxUsers; i++) {
        if (canal[roomToJoin][i] == 0) {
            canal[roomToJoin][i] = id;
            break;
        }
    }
}


