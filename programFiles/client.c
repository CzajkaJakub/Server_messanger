//
// Created by Jakub on 25/12/2021.
//


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/msg.h>
#include <string.h>

#include "../headers/client.h"



void clearConsole();
char readResponseFromUser();
void registerNewUser();
long readIdFromUser();
bool sendUserIdToServer();

_Noreturn void userMenu();

long userId;
int serverKey = 555768;
int queue;
bool registered = false;

int clientRegistrationToSendToServerType = 11;
int clientRegistrationToReceiveFromServerType = 12;

int main(){
    clearConsole();
    printf("Welcome in inter-server communicator, choose an action\n"
           "R - Register to server\n"
           "Q - Quit the program\n");

    while (1) {
        switch (readResponseFromUser()) {
            case 'R' : registerNewUser(); break;
            default : printf("Wrong data, please try again\n\n");
                printf("Choose an action\n"
                       "R - Register to server\n"
                       "Q - Quit the program\n");
                break;

            case 'Q' :
                return 0;
        }

        if(registered){
            userMenu();
        }
    }
}


void registerNewUser() {
    printf("Please put unique number, it will be your user id\n");

    //take id from user and convert it to long and check availability of id in server
    userId = readIdFromUser();

    if(sendUserIdToServer()){
        printf("Registration successful, your private id : %ld", userId);
    } else {
        userId = 0;
        printf("Registration failed");
    }

}



void clearConsole() {
    system("clear");
}

char readResponseFromUser() {
    char response;
    scanf(" %c", &response);
    fflush(stdin);
    response = (char)toupper((int)response);
    return response;
}

long readIdFromUser() {  // uwaga na 0
    char temp[30];
    scanf(" %s", temp);
    fflush(stdin);
    return strtol(temp, NULL, 0);
}

bool sendUserIdToServer() {
    clientRegistrationToSendToServer.type = clientRegistrationToSendToServerType;
    char tempId[100];
    sprintf(tempId, "%ld", userId);
    strcpy(clientRegistrationToSendToServer.message, tempId);
    queue = msgget(serverKey, 0644 | IPC_CREAT);
    msgsnd(queue, &clientRegistrationToSendToServer, sizeof(clientRegistrationToSendToServer.message) , 0);

    msgrcv(queue, &clientRegistrationToReceiveFromServer, sizeof(clientRegistrationToReceiveFromServer.message), clientRegistrationToReceiveFromServerType, 0);

    if(strcmp(clientRegistrationToReceiveFromServer.message, "Available") == 0){
        return true;
    } else if (strcmp(clientRegistrationToReceiveFromServer.message, "Unavailable") == 0){
        return false;
    } else {
        printf("There is no empty places, sorry\n");
        return false;
    }

}

_Noreturn void userMenu() {
    while (1) {
        printf("jestes w menu");
    }
}









