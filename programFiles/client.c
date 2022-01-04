//
// Created by Jakub on 25/12/2021.
//

#include "../headers/client.h"
#include "../headers/configuration.h"
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>


int main() {
    readSettings();

    while (1){
        printf("Welcome in inter-server communicator, choose an action\n"
               "R - Register to server\n"
               "Q - Quit the program\n");

        switch (readResponseFromUser()) {
            case 'R' : registerNewUser(); break;
            default : printf("Wrong data, please try again\n\n"); break;
            case 'Q' : return 0;
        }

        if(registered){
            userMenu();
        }
    }
}

void readSettings() {
    serverId = msgget(serverKey, 0644 | IPC_CREAT);
    clientRegistration.type = registrationType;
    clientResponse.type = responseType;
    registered = false;
}

void registerNewUser() {
    printf("Please put unique number, it will be your user id\n");

    //take id from user and convert it to long and check availability of id in server
    userId = checkAvailabilityOfId(readIdFromUser());

    if(userId){
        printf("You have registered successful\n");
        printf("Your unique id => %ld\n", userId);
        registered = true;
        return;
    } else {
        printf("Your id is unavailable, please try again\n\n");
        registered = false;
        return;
    }
}


char readResponseFromUser() {
    char response;
    scanf("%c", &response);
    fflush(stdin);
    response = (char)toupper((int)response);
    return response;
}



long readIdFromUser(){
    char temp[30];
    scanf("%s", temp);
    fflush(stdin);
    return strtol(temp, NULL, 0);
}


long checkAvailabilityOfId(long tempId) {
    char tempIdString[50];
    sprintf(tempIdString, "%ld", tempId);
    strcpy(clientRegistration.message, tempIdString);
    msgsnd(serverId, &clientRegistration, strlen(clientRegistration.message)+1, IPC_NOWAIT);
    msgrcv(serverId, &clientResponse, 1024, responseType, 0);
    if(strcmp(clientResponse.message, "Available") == 0){
        registered = true;
        return tempId;
    } else {
       registered = false;
        return 0;
    }
}

void userMenu() {
    bool logged = true;

    while (logged){
        printf("Choose an action\n"
               "Q - Quit the program\n");

        switch (readResponseFromUser()) {
            case 'Q' : logged = false; break;
            default : printf("Wrong data, please try again\n\n"); break;

        }
    }

    registered = false;
    userId = 0;
}


