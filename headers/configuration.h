//
// Created by Patrycja on 03/01/2022.
//

#ifndef INTER_SERVER_MESSENGER_CONFIGURATION_H
#define INTER_SERVER_MESSENGER_CONFIGURATION_H

#include <stdbool.h>

//configuration
int serverKey = 199928;
int registrationType = 5;
int responseType = 6;
int serverId;

// client configuration
long userId;
bool registered;

// server configuration
const int maxUsers = 100;
long users[100];

#endif //INTER_SERVER_MESSENGER_CONFIGURATION_H
