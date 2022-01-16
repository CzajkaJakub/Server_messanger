//
// Created by Jakub on 25/12/2021.
//

#include <stdio.h>
#include <sys/msg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include "../headers/server.h"
#include "../headers/configuration.h"


int main(){
    printf("Logs: \n");
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
            case 19 : sendMessageToAllUsersInRoom(); break;
            default: printf("Other data type"); fflush(stdout); break;
        }
        sleep(2);
    }
}
long getIdFromMessage(){
    return strtol(serverRequestToReceive.message, NULL, 0);
}
long checkRequest() {
    msgrcv(queue, &serverRequestToReceive, sizeof(serverRequestToReceive.message), 0, 0);
    printf("Received request : type %ld, message %s\n", serverRequestToReceive.type, serverRequestToReceive.message);
    fflush(stdout);
    return serverRequestToReceive.type;
}
void registerNewUser() {
    if(checkAvailabilityOfIdAndSave()){
        sendRequestToClient(clientRegistrationToReceiveFromServerType, "Available");
        printf("%s - user id is %s - registration successful\n", serverRequestToReceive.message, serverMessageToSendToClient.message);
        fflush(stdout);
    } else {
        sendRequestToClient(clientRegistrationToReceiveFromServerType, "Unavailable");
        printf("%s - user id is %s - registration failed\n", serverRequestToReceive.message, serverMessageToSendToClient.message);
        fflush(stdout);
    }

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
void showAllRooms() {
    sendRequestToClient(getIdFromMessage(), "\n'Room 1'\n'Room 2'\n'Room 3'\n'Room 4'\n'Room 5'\n");
}
void showAllAccount() {
    char allAccountsMessage[1024] = "";
    for(int i = 0; i < maxUsers; i++){
        if(users[i] != 0){
            char tempId[100];
            sprintf(tempId, "\nUser's id : %ld", users[i]);
            strcat(allAccountsMessage, tempId);
        }
    }
    sendRequestToClient(getIdFromMessage(), allAccountsMessage);
}
void showAllRoomsWithUsers() {

    char message[1024];
    for(int i = 0; i < maxRooms; i++){
        char room[256];
        sprintf(room, "\nRoom %d : \n\tUsers : ", i + 1);
        for(int j = 0; j < maxUsers; j++){
            if(canal[i][j] != 0){
                char tempUserId[10];
                sprintf(tempUserId, " %ld ", canal[i][j]);
                strcat(room, tempUserId);
            }
        }
        strcat(message, room);
    }

    sendRequestToClient(getIdFromMessage(), message);

}
void sendMessageToUser() {
    printf("%d typ\n", sendMessageToUserInRoomType);
    sleep(1);
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char time[50];
    char message[1024];
    long senderId = getIdFromMessage();
    sprintf(time, "%d-%02d-%02d %02d:%02d:%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    msgrcv(queue, &serverRequestToReceive, sizeof(serverRequestToReceive.message), sendMessageToUserInRoomType, 0);
    long userToSendId = getIdFromMessage();
    sleep(1);

    msgrcv(queue, &serverRequestToReceive, sizeof(serverRequestToReceive.message), sendMessageToUserInRoomType, 0);
    sprintf(message, "%s : User %ld send a message : %s   <- PRIVATE MESSAGE\n", time, senderId, serverRequestToReceive.message);


    for(int i = 0; i < maxRooms; i++){
        for(int j = 0; j < maxUsers; j++){
            if(canal[i][j] == senderId){
                for(int k = 0; k < maxUsers; k++){
                    if(canal[i][k] == userToSendId){
                        sendRequestToClient(userToSendId, message);
                        return;
                    }
                }
            }
        }
    }
    sendRequestToClient(senderId, "Message hasn't been delivered, you are not in any room or user with typed id doesn't exist\n");
}
void sendMessageToAllUsersInRoom() {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char time[50];
    char message[1024];
    long senderId = getIdFromMessage();
    sprintf(time, "%d-%02d-%02d %02d:%02d:%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    msgrcv(queue, &serverRequestToReceive, sizeof(serverRequestToReceive.message), sendMessageToAllUsersInRoomType, 0);
    sprintf(message, "%s : User %ld send a message %s", time, senderId, serverRequestToReceive.message);


    for(int i = 0; i < maxRooms; i++){
        for(int j = 0; j < maxUsers; j++){
            if(canal[i][j] == senderId){
                for(int k = 0; k < maxUsers; k++){
                    if(canal[i][k] != 0 && canal[i][k] != senderId){
                        sendRequestToClient(canal[i][k], message);
                        sleep(1);
                    }
                }
                return;
            }
        }
    }
    sendRequestToClient(senderId, "Message hasn't been delivered - you are not in any room\n");
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
void removeUserFromRoom(long userId) {
    for(int i = 0; i < maxRooms; i++){
        for(int j = 0; j < maxUsers; j++){
            if(canal[i][j] == userId){
                canal[i][j] = 0;
            }
        }
    }
}
void deregisterUserFromTheServer() {
    for(int i = 0; i < maxUsers; i++){
        if(users[i] == getIdFromMessage()){
            users[i] = 0;
        }
    }
    removeUserFromRoom(getIdFromMessage());
    sendRequestToClient(getIdFromMessage(), "You have been disconnected\n");
}
void registerUserToRoom() {
    long tempId = getIdFromMessage();
    removeUserFromRoom(tempId);
    sendRequestToClient(tempId, "Select the room number : 'Room 1 (1)', 'Room 2 - (2)', 'Room 3 - (3)', 'Room 4 - (4)', 'Room 5 - (5)'");
    sleep(1);
    receiveUsersDecision(tempId);
    addUserToRoom(tempId);
}
void receiveUsersDecision(long userId) {
    msgrcv(queue, &serverRequestToReceive, sizeof(serverRequestToReceive.message), userId + 100, 0);
    fflush(stdout);
}
void sendRequestToClient(long type, const char *message) {
    serverMessageToSendToClient.type = type;
    strcpy(serverMessageToSendToClient.message, message);
    msgsnd(queue, &serverMessageToSendToClient, sizeof(serverMessageToSendToClient.message), IPC_NOWAIT);
}


