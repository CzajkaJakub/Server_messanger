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
#include "../headers/configuration.h"


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
    sendRegistrationForm();

    if(checkAvailabilityOfIdAndSave()){
        clearConsole();
        registeredStatus();
    } else {
        unregisteredStatus();
        clearConsole();
        printf("Registration failed\n");
    }
}
long readCorrectId() {
    long tempUserId = readIdFromUser();
    if(tempUserId < 1000 || tempUserId > 9999){
        printf("Your id must be four numbers ! \n");
        return readCorrectId();
    } else {
        return tempUserId;
    }
}
long readIdFromUser() {
    char temp[30];
    scanf(" %s", temp);
    fflush(stdin);
    return strtol(temp, NULL, 0);
}
void sendRegistrationForm() {
    setIdAsMessage();
    sendRequestToServer(clientRegistrationToSendToServerType);
}
bool checkAvailabilityOfIdAndSave() {
    msgrcv(queue, &requestFromServerToReceive, sizeof(requestFromServerToReceive.message), clientRegistrationToReceiveFromServerType, 0);
    if(strcmp(requestFromServerToReceive.message, "Available") == 0){ return true;}
    else if (strcmp(requestFromServerToReceive.message, "Unavailable") == 0){ return false; }
    else { printf("There is no empty places, sorry\n"); return false; }
}
void unregisteredStatus() {
    userId = 0;
    registered = false;
    privateTypeToReceiveMessagesFromServer = 0;
}
void registeredStatus() {
    privateTypeToReceiveMessagesFromServer = userId;
    registered = true;
}
char readResponseFromUser() {
    char response;
    scanf(" %c", &response);
    fflush(stdin);
    response = (char)toupper((int)response);
    return response;
}
void clearConsole() {
    system("clear");
}
void setIdAsMessage() {
    char tempId[100];
    sprintf(tempId, "%ld", userId);
    strcpy(requestToSendToServer.message, tempId);
}
void userMenu() {
    if(fork() == 0){
        startReceivingMessagesFromServer();
    } else {
        printf("Registration successful, your private id : %ld\n", userId);
        while (registered) {
            clearConsole();
            printf("\nWelcome in user menu, choose an action\n"
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
_Noreturn void startReceivingMessagesFromServer() {
    while (1) {
        msgrcv(queue, &requestFromServerToReceive, sizeof(requestFromServerToReceive.message), privateTypeToReceiveMessagesFromServer, 0);
        sleep(1);
        printf("\nYou have received a message from server : \n%s\n", requestFromServerToReceive.message);
        fflush(stdout);
    }
}
void showAllAccounts() {
    setIdAsMessage();
    sendRequestToServer(showAllAccountsType);
}
void showAllRooms() {
    setIdAsMessage();
    sendRequestToServer(showAllRoomsType);
}
void showAllRoomsWithUsers() {
    setIdAsMessage();
    sendRequestToServer(showAllUsersInAllRooms);
}
void registerToRoom() {
    setIdAsMessage();
    sendRequestToServer(registerToRoomType);
    char response[1024];
    scanf(" %s", response);
    fflush(stdin);
    strcpy(requestToSendToServer.message, response);
    sendRequestToServer(privateTypeToReceiveMessagesFromServer + 100);
}
void deregisterFromTheServer() {
    setIdAsMessage();
    sendRequestToServer(deregisterFromServerType);
    unregisteredStatus();
}
void sendMessageToAllUsersInRoom() {
    char message[1024];
    printf("\nType a message : ");
    scanf("%[^\n]", message);
    fflush(stdin);

    setIdAsMessage();
    sendRequestToServer(sendMessageToAllUsersInRoomType);

    changeMessageInRequest(message);
    sendRequestToServer(sendMessageToAllUsersInRoomType);
}
void sendMessageToUser() {
    char tempUserIdToSendMessage[1024];
    char message[1024];
    printf("Type a user id to send: ");
    sprintf(tempUserIdToSendMessage, "%ld", readCorrectId());
    printf("\nType a message : ");
    scanf("%[^\n]", message);
    fflush(stdin);

    setIdAsMessage();
    sendRequestToServer(sendMessageToUserInRoomType);
    sleep(1);

    changeMessageInRequest(tempUserIdToSendMessage);
    sendRequestToServer(sendMessageToUserInRoomType);
    sleep(1);

    changeMessageInRequest(message);
    sendRequestToServer(sendMessageToUserInRoomType);
}
void sendRequestToServer(long messageType) {
    queue = msgget(serverKey, 0644 | IPC_CREAT);
    changeRequestType(messageType);
    msgsnd(queue, &requestToSendToServer, sizeof(requestToSendToServer.message) , IPC_NOWAIT);
}
void changeMessageInRequest(const char *mess) {
    strcpy(requestToSendToServer.message, mess);
}
void changeRequestType(long type) {
    requestToSendToServer.type = type;
}