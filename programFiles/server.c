//
// Created by Jakub on 25/12/2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/msg.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#include "../headers/server.h"

int serverKey = 555768;
int queue;

int clientRegistrationToSendToServerType = 11;
int clientRegistrationToSendToClientType = 12;
int showAllAccountsType = 13;


int maxUsers = 5;
long users[5];


_Noreturn void serverIsWorking();

void checkRegistrations();

void checkShowAll();

int main(){
    serverIsWorking();
}

_Noreturn void serverIsWorking() {
    while (1) {
        checkRegistrations();
        checkShowAll();
        sleep(3);
    }
}

void checkRegistrations() {
    queue = msgget(serverKey, 0644 | IPC_CREAT);
    if(msgrcv(queue, &serverRequestToReceive, sizeof(serverRequestToReceive.message), clientRegistrationToSendToServerType, IPC_NOWAIT) != -1){
        long tempUserId = strtol(serverRequestToReceive.message, NULL, 0);
        serverMessageToSendToClient.type = clientRegistrationToSendToClientType;


        for(int i = 0; i < maxUsers; i++) {
            if (users[i] == tempUserId) {
                strcpy(serverMessageToSendToClient.message, "Unavailable");
                printf("%s - user id is %s - registration failed\n", serverRequestToReceive.message,
                       serverMessageToSendToClient.message);
                msgsnd(queue, &serverMessageToSendToClient, sizeof(serverMessageToSendToClient.message), 0);
                return;
            }
        }

        for(int i = 0; i < maxUsers; i++){
            if(users[i] == 0){
                users[i] = tempUserId;
                strcpy(serverMessageToSendToClient.message, "Available");
                printf("%s - user id is %s - registration successful\n", serverRequestToReceive.message, serverMessageToSendToClient.message);
                msgsnd(queue, &serverMessageToSendToClient, sizeof(serverMessageToSendToClient.message), 0);
                return;
            }
        }

        strcpy(serverMessageToSendToClient.message, "There is no place for you :(\n");
        printf("%s - user id is %s - registration failed - no places for fuckers\n", serverRequestToReceive.message,
               serverMessageToSendToClient.message);
        msgsnd(queue, &serverMessageToSendToClient, sizeof(serverMessageToSendToClient.message), 0);
        return;
    }
}


void checkShowAll() {
    queue = msgget(serverKey, 0644 | IPC_CREAT);
    if (msgrcv(queue, &serverRequestToReceive, sizeof(serverRequestToReceive.message), showAllAccountsType,
               IPC_NOWAIT) != -1) {
        printf("odebralem requesta o wyswietlenie uzytkownikow do %s\n", serverRequestToReceive.message);

        char wiad[1024];

        for(int i=0; i < maxUsers; i++){
            char tempuser[10];
            sprintf(tempuser, "%ld", users[i]);
            strcat(wiad, tempuser);
        }

        printf("Wiadomosc : %s", wiad);

//        strcpy(serverMessageToSendToClient.message, );
//        printf("%s - user id is %s - registration failed\n", serverRequestToReceive.message,
//               serverMessageToSendToClient.message);
//        msgsnd(queue, &serverMessageToSendToClient, sizeof(serverMessageToSendToClient.message), 0);
//        return;
    }
}
