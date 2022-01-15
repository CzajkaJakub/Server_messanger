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

int maxUsers = 5;
long users[5];


_Noreturn void serverIsWorking();

void checkRegistrations();

int main(){
    serverIsWorking();
}

_Noreturn void serverIsWorking() {
    while (1) {
        checkRegistrations();
        sleep(3);
    }
}

void checkRegistrations() {
    queue = msgget(serverKey, 0644 | IPC_CREAT);
    if(msgrcv(queue, &serverRegistrationToReceive, sizeof(serverRegistrationToReceive.message), clientRegistrationToSendToServerType, IPC_NOWAIT) != -1){
        long tempUserId = strtol(serverRegistrationToReceive.message, NULL, 0);
        serverRegistrationToSendToClient.type = clientRegistrationToSendToClientType;


        for(int i = 0; i < maxUsers; i++) {
            if (users[i] == tempUserId) {
                strcpy(serverRegistrationToSendToClient.message, "Unavailable");
                printf("%s - user id is %s - registration failed\n", serverRegistrationToReceive.message,
                       serverRegistrationToSendToClient.message);
                msgsnd(queue, &serverRegistrationToSendToClient, sizeof(serverRegistrationToSendToClient.message), 0);
                return;
            }
        }

        for(int i = 0; i < maxUsers; i++){
            if(users[i] == 0){
                users[i] = tempUserId;
                strcpy(serverRegistrationToSendToClient.message, "Available");
                printf("%s - user id is %s - registration successful\n", serverRegistrationToReceive.message, serverRegistrationToSendToClient.message);
                msgsnd(queue, &serverRegistrationToSendToClient, sizeof(serverRegistrationToSendToClient.message), 0);
                return;
            }
        }

        strcpy(serverRegistrationToSendToClient.message, "There is no place for you :(\n");
        printf("%s - user id is %s - registration failed - no places for fuckers\n", serverRegistrationToReceive.message,
               serverRegistrationToSendToClient.message);
        msgsnd(queue, &serverRegistrationToSendToClient, sizeof(serverRegistrationToSendToClient.message), 0);
        return;



    }
}
