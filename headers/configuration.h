//
// Created by Patrycja on 03/01/2022.
//

#ifndef INTER_SERVER_MESSENGER_CONFIGURATION_H
#define INTER_SERVER_MESSENGER_CONFIGURATION_H

#include <stdbool.h>

//configuration
int serverKey = 3334444;

int clientRegistrationToSendToServerType = 11;
int clientRegistrationToReceiveFromServerType = 12;
int showAllAccountsType = 13;
int deregisterFromServerType = 14;
int registerToRoomType = 15;
int showAllRoomsType = 16;
int showAllUsersInAllRooms = 17;
int sendMessageToUserInRoomType = 18;
int sendMessageToAllUsersInRoomType = 19;

#endif //INTER_SERVER_MESSENGER_CONFIGURATION_H
