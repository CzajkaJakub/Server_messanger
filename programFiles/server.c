//
// Created by Jakub on 25/12/2021.
//

#include "../headers/server.h"
#include <stdio.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "../headers/configuration.h"


int main() {
    readServerSettings();

    if(fork() == 0){

    }else{
        registerNewUsers();
    }
}



void readServerSettings() {
    serverId = msgget(serverKey, 0644 | IPC_CREAT);
    registrationServer.type = registrationType;
    responseServer.type = responseType;
}

_Noreturn void registerNewUsers() {
    while(1){
        //get registrations
        msgrcv(serverId, &registrationServer, 1024, registrationType, 0);
        if(checkAvailabilityOfId()){
            saveUserIdInServerMemory();
        }
    }
}


int checkAvailabilityOfId() {
    for(int i = 0; i < maxUsers; i++){
        if(users[i] == strtol(registrationServer.message, NULL, 0)){
            strcpy(responseServer.message, "Unavailable");
            printf("%s - user id is %s - registration failed\n", registrationServer.message, responseServer.message);
            msgsnd(serverId, &responseServer, strlen(responseServer.message)+1, 0);
            return false;
        }
    }
    return true;
}

void saveUserIdInServerMemory() {
    for(int i = 0; i < maxUsers; i++){
        if(users[i] == 0){
            users[i] = strtol(registrationServer.message, NULL, 0);
            strcpy(responseServer.message, "Available");
            printf("%s - user id is %s - registration successful\n", registrationServer.message, responseServer.message);
            msgsnd(serverId, &responseServer, strlen(responseServer.message)+1, 0);
            return;
        }
    }
}

