#ifndef PROTOCOLS_H
#define PROTOCOLS_H

#include "protocol_functions.h"
#include <string>

using namespace std;

void ActionE_PACKAGE(char* buffer,
                     string ip,
                     int& packageSize)
{
    int currentIndex = 0;

    buffer[currentIndex] = 'E';
    currentIndex += 1;

    Insert_STR(buffer, currentIndex, ip, 15);
    currentIndex += 15;

    packageSize = currentIndex;
}

void ActionM_PACKAGE(char* buffer,
                     string destination_ip,
                     string message,
                     int& packageSize)
{
    int currentIndex = 0;

    buffer[currentIndex] = 'M';
    currentIndex += 1;

    Insert_STR(buffer, currentIndex, destination_ip, 15);
    currentIndex += 15;

    Insert_INT(buffer, currentIndex, message.size(), 4);
    currentIndex += 4;

    Insert_STR(buffer, currentIndex, message, message.size());
    currentIndex += message.size();

    packageSize = currentIndex;
}

void ActionL_PACKAGE(char* buffer,
                     string destination_ip,
                     string message,
                     int& packageSize)
{
    int currentIndex = 0;

    buffer[currentIndex] = 'L';
    currentIndex += 1;

    Insert_STR(buffer, currentIndex, destination_ip, 15);
    currentIndex += 15;

    Insert_INT(buffer, currentIndex, message.size(), 4);
    currentIndex += 4;

    Insert_STR(buffer, currentIndex, message, message.size());
    currentIndex += message.size();

    packageSize = currentIndex;
}

void ActionA_PACKAGE(char* buffer,
                     int source_port,
                     int source_nat_port,
                     int destination_nat_port,
                     string source_ip,
                     string destination_nat_ip,
                     char type_entry,
                     int& packageSize)
{
    int currentIndex = 0;

    buffer[currentIndex] = 'A';
    currentIndex += 1;

    Insert_INT(buffer, currentIndex, source_port, 4);
    currentIndex += 4;

    Insert_INT(buffer, currentIndex, source_nat_port, 4);
    currentIndex += 4;

    Insert_INT(buffer, currentIndex, destination_nat_port, 4);
    currentIndex += 4;

    Insert_STR(buffer, currentIndex, source_ip, 15);
    currentIndex += 15;

    Insert_STR(buffer, currentIndex, destination_nat_ip, 15);
    currentIndex += 15;

    buffer[currentIndex] = type_entry;
    currentIndex += 1;

    packageSize = currentIndex;
}

void ActionD_PACKAGE(char* buffer,
                     string ip,
                     int entry_id,
                     int& packageSize)
{
    int currentIndex = 0;

    buffer[currentIndex] = 'D';
    currentIndex += 1;

    Insert_STR(buffer, currentIndex, ip, 15);
    currentIndex += 15;

    Insert_INT(buffer, currentIndex, entry_id, 1);
    currentIndex += 1;

    packageSize = currentIndex;
}

void ActionL_PACKAGE(char* buffer,
                     string nickname,
                     int& packageSize)
{
    int currentIndex = 0;

    buffer[currentIndex] = 'L';
    currentIndex += 1;

    Insert_INT(buffer, currentIndex, nickname.size(), 4);
    currentIndex += 4;

    Insert_STR(buffer, currentIndex, nickname, nickname.size());
    currentIndex += nickname.size();

    packageSize = currentIndex;

}
void ActionC_PACKAGE(char* buffer,
                     string nickname,
                     int& packageSize)
{
    int currentIndex = 0;

    buffer[currentIndex] = 'C';
    currentIndex += 1;

    Insert_INT(buffer, currentIndex, nickname.size(), 4);
    currentIndex += 4;

    Insert_STR(buffer, currentIndex, nickname, nickname.size());
    currentIndex += nickname.size();

    packageSize = currentIndex;

}
void ActionP_PACKAGE(char* buffer,
                     int& packageSize)
{
    int currentIndex = 0;
    buffer[currentIndex] = 'P';
    currentIndex += 1;
    packageSize = currentIndex;
}

void ActionQ_PACKAGE(char* buffer,
                     int port,
                     string ip,
                     string nickname,
                     int& packageSize)
{
    int currentIndex = 0;

    buffer[currentIndex] = 'Q';
    currentIndex += 1;

    Insert_INT(buffer, currentIndex, port, 4);
    currentIndex += 4;

    Insert_STR(buffer, currentIndex, ip, 15);
    currentIndex += 15;

    Insert_INT(buffer, currentIndex, nickname.size(), 4);
    currentIndex += 4;

    Insert_STR(buffer, currentIndex, nickname, nickname.size());
    currentIndex += nickname.size();

    packageSize = currentIndex;
}

#endif
