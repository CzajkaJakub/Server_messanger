//
// Created by Jakub on 25/12/2021.
//


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>

#include "../headers/client.h"



long userId;
int serverKey = 555768;   // takie samo jak u servera (publiczny klucz kolejki)
int queue;                      // publiczna kolejka
bool registered = false;         // stan rejestracji


//typy wiadomosci musza byc takie same jak u servera
int clientRegistrationToSendToServerType = 11;
int clientRegistrationToReceiveFromServerType = 12;
int showAllAccountsType = 13;
long privateTypeToReceiveMessagesFromServer;


void deregisterFromTheServer();

int main(){

    clearConsole();

    while (1) {
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


void registerNewUser() {
    printf("Please put unique number, it will be your user id\n");

    //take id from user and convert it to long and check availability of id in server
    userId = readIdFromUser();
    clearConsole();
    changeRequestType(clientRegistrationToSendToServerType);
    setIdAsMessage();
    sendRequestToServer();

    if(checkAvailabilityOfId()){
        registeredStatus();
    } else {
        unregisteredStatus();
        clearConsole();
        printf("Registration failed\n");
    }
}

_Noreturn void userMenu() {
    while (registered) {
        printf("Welcome in user menu, choose an action\n"
               "S - Show all accounts registered in server\n"
               "Q - Deregister from server\n");

        switch (readResponseFromUser()) {
            case 'S' :
                showAllAccounts(); // todo
                break;
            default :
                printf("Wrong data, please try again\n\n");
                break;
            case 'Q' :
                deregisterFromTheServer();  // todo
        }
    }
}


char readResponseFromUser() {   // zwraca duza litere odpowiedzi
    char response;
    scanf(" %c", &response);
    fflush(stdin);
    response = (char)toupper((int)response);
    return response;
}

long readIdFromUser() {  // uwaga na 0       // zwraca id usera typu long
    char temp[30];
    scanf(" %s", temp);
    fflush(stdin);
    return strtol(temp, NULL, 0);
}

// sprawdza czy id jest mozliwe po stronie servera

bool checkAvailabilityOfId() {
    msgrcv(queue, &requestFromServerToReceive, sizeof(requestFromServerToReceive.message), clientRegistrationToReceiveFromServerType, 0);

    if(strcmp(requestFromServerToReceive.message, "Available") == 0){
        return true;
    } else if (strcmp(requestFromServerToReceive.message, "Unavailable") == 0){
        return false;
    } else {
        printf("There is no empty places, sorry\n");
        return false;
    }

}


// ustawia status na niezarejestrowany
void unregisteredStatus() {
    userId = 0;
    registered = false;
    privateTypeToReceiveMessagesFromServer = 0;
}

// ustawia status na zarejestrowany
void registeredStatus() {
    privateTypeToReceiveMessagesFromServer = userId;
    registered = true;
    printf("Registration successful, your private id : %ld\n", userId);
}

//czysci konsole
void clearConsole() {
    //system("clear");
}

//zmienia typ requesta
void changeRequestType(int type) {
    requestToSendToServer.type = type;
}

//ustawia message na id
void setIdAsMessage() {
    char tempId[100];
    sprintf(tempId, "%ld", userId);
    strcpy(requestToSendToServer.message, tempId);
}

//wysylanie requesta na server
void sendRequestToServer() {
    queue = msgget(serverKey, 0644 | IPC_CREAT);
    msgsnd(queue, &requestToSendToServer, sizeof(requestToSendToServer.message) , 0);

    // do wyjebania
    printf("%s - wiadomosc, %ld - typ wiadomosci\n", requestToSendToServer.message, requestToSendToServer.type);
}


void showAllAccounts() {
    changeRequestType(showAllAccountsType);
    setIdAsMessage();
    sendRequestToServer();
}

// wyrejestruj // todo
void deregisterFromTheServer() {

}


