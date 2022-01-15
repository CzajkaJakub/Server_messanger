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


// typy
int showAllAccountsType = 13;


int maxUsers = 5;
long users[5];


_Noreturn void startWorking();
long checkRequest();
void registerNewUser();
void changeTypeOfMessageToSend(long type);
bool checkAvailabilityOfIdAndSave();
void sendRequestToClient();
void changeMessageInRequest(const char *message);
void showAllAccount();
long getIdFromMessage();

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
            default: printf("Other data type"); fflush(stdout); break;
        }

        sleep(3);
    }
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
    printf("wszystkie konta wypisz");
    changeTypeOfMessageToSend(getIdFromMessage());
    changeMessageInRequest("wszystkie konta");
    sendRequestToClient();
}




//void checkShowAll() {
//    queue = msgget(serverKey, 0644 | IPC_CREAT);
//    if (msgrcv(queue, &serverRequestToReceive, sizeof(serverRequestToReceive.message), showAllAccountsType,
//               IPC_NOWAIT) != -1) {
//        printf("odebralem requesta o wyswietlenie uzytkownikow do %s\n", serverRequestToReceive.message);
//
//        char wiad[1024];
//
//        for(int i=0; i < maxUsers; i++){
//            char tempuser[10];
//            sprintf(tempuser, "%ld", users[i]);
//            strcat(wiad, tempuser);
//        }
//
//        printf("Wiadomosc : %s", wiad);
//
////        strcpy(serverMessageToSendToClient.message, );
////        printf("%s - user id is %s - registration failed\n", serverRequestToReceive.message,
////               serverMessageToSendToClient.message);
////        msgsnd(queue, &serverMessageToSendToClient, sizeof(serverMessageToSendToClient.message), 0);
////        return;
//    }
//}
