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
int serverKey = 3334444;   // takie samo jak u servera (publiczny klucz kolejki)
int queue;                      // publiczna kolejka
bool registered = false;         // stan rejestracji


//typy wiadomosci musza byc takie same jak u servera
int clientRegistrationToSendToServerType = 11;
int clientRegistrationToReceiveFromServerType = 12;
int showAllAccountsType = 13;
int deregisterFromServerType = 14;
int registerToRoomType = 15;
int showAllRoomsType = 16;
int showAllUsersInAllRooms = 17;
int sendMessageToUserInRoomType = 18;
int sendMessageToAllUsersInRoomType = 19;


long privateTypeToReceiveMessagesFromServer;

void sendMessageToAllUsersInRoom();

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
    userId = readCorrectId();
    clearConsole();
    changeRequestType(clientRegistrationToSendToServerType);
    setIdAsMessage();
    sendRequestToServer();

    if(checkAvailabilityOfIdAndSave()){
        registeredStatus();
    } else {
        unregisteredStatus();
        clearConsole();
        printf("Registration failed\n");
    }
}

void userMenu() {
    if(fork() == 0){
        startReceivingMessagesFromServer();
    } else {
        while (registered) {
            printf("Welcome in user menu, choose an action\n"
                   "S - Show all accounts registered in server\n"
                   "R - Register to a room\n"
                   "T - Show all rooms\n"
                   "G - Show all rooms with users\n"
                   "A - Send a message to some user\n"
                   "B - Send a message to all users in room\n"
                   "Q - Deregister from server\n");

            switch (readResponseFromUser()) {
                case 'S' :
                    showAllAccounts();
                    break;
                default :
                    printf("Wrong data, please try again\n\n");
                    break;
                case 'Q' :
                    deregisterFromTheServer();
                    break;
                case 'T' :
                    showAllRooms(); break;
                case 'G' :
                    showAllRoomsWithUsers();
                    break;
                case 'A' :
                    sendMessageToUser();
                    break;
                case 'B' :
                    sendMessageToAllUsersInRoom();
                    break;
                case 'R':
                    registerToRoom();
                    break;
            }
        }
    }
}

void sendMessageToAllUsersInRoom() {
    char message[1024];

    printf("\nType a message : ");
    scanf("%[^\n]", message);
    fflush(stdin);


    changeRequestType(sendMessageToAllUsersInRoomType);

    setIdAsMessage();
    sendRequestToServer();

    changeMessageInRequest(message);
    sendRequestToServer();
}

void sendMessageToUser() {
    char tempUserIdToSendMessage[1024];
    char message[1024];

    printf("Type a user id to send: ");
    scanf("%s", tempUserIdToSendMessage);
    fflush(stdin);

    printf("\nType a message : ");
    scanf("%[^\n]", message);
    fflush(stdin);


    changeRequestType(sendMessageToUserInRoomType);

    setIdAsMessage();
    sendRequestToServer();

    changeMessageInRequest(tempUserIdToSendMessage);
    sendRequestToServer();

    changeMessageInRequest(message);
    sendRequestToServer();
}

void changeMessageInRequest(const char *message) {
    strcpy(requestToSendToServer.message, message);
}


void showAllRoomsWithUsers() {
    changeRequestType(showAllUsersInAllRooms);
    setIdAsMessage();
    sendRequestToServer();
}

void showAllRooms() {
    changeRequestType(showAllRoomsType);
    setIdAsMessage();
    sendRequestToServer();
}

_Noreturn void startReceivingMessagesFromServer() {
    while (1) {
        msgrcv(queue, &requestFromServerToReceive, sizeof(requestFromServerToReceive.message), privateTypeToReceiveMessagesFromServer, 0);
        sleep(1);
        printf("\n\nYou have received a message from server : \n%s\n", requestFromServerToReceive.message);
        fflush(stdout);
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

long readCorrectId() {
    long tempUserId = readIdFromUser();
    printf("%ld", tempUserId);
    if(tempUserId < 1000 || tempUserId > 9999){
        printf("Your id must be four numbers ! \n");
        return readCorrectId();
    } else {
        return tempUserId;
    }
}

// sprawdza czy id jest mozliwe po stronie servera

bool checkAvailabilityOfIdAndSave() {
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
void changeRequestType(long type) {
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
    msgsnd(queue, &requestToSendToServer, sizeof(requestToSendToServer.message) , IPC_NOWAIT);

    // do wyjebania
    printf("%s - wiadomosc, %ld - typ wiadomosci\n", requestToSendToServer.message, requestToSendToServer.type);
}


void showAllAccounts() {
    changeRequestType(showAllAccountsType);
    setIdAsMessage();
    sendRequestToServer();
}

// wyrejestruj
void deregisterFromTheServer() {
    changeRequestType(deregisterFromServerType);
    setIdAsMessage();
    printf("%s - %ld\n", requestToSendToServer.message, requestToSendToServer.type);
    sendRequestToServer();
    unregisteredStatus();
}

void registerToRoom() {
    changeRequestType(registerToRoomType);
    setIdAsMessage();
    sendRequestToServer();

    changeRequestType(privateTypeToReceiveMessagesFromServer + 100);
    char response[1024];
    scanf(" %s", response);
    fflush(stdin);
    strcpy(requestToSendToServer.message, response);
    sendRequestToServer();
}


