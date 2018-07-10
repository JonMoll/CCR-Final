#ifndef CLIENT_FUNCTIONS_H
#define CLIENT_FUNCTIONS_H

#include "client_info.h"
#include "socket_functions.h"
#include "protocols.h"

using namespace std;

mutex CLIENT_MUTEX;

const int EMPTY_OPTION = -1;
const int OPTION_ADD_ENTRY = 0;
const int OPTION_DELETE_ENTRY = 1;
const int OPTION_SEND_MESSAGE = 2;
const int OPTION_CLOSE = 9;

void ClientSEND_THREAD(int socketFD, bool& online, ClientInfo* client_info)
{
    char* buffer = new char[ buffer_size ];
    int n;
    int option = EMPTY_OPTION;

    int packageSize = 0;
    ActionE_PACKAGE(buffer, client_info->my_ip, packageSize);

    n = write(socketFD, buffer, packageSize);

    if (n < 0) perror("[ Action E ] Error, write in socket");
    else cout << "[ Action E ] OK" << endl << endl;

    while (online)
    {
        PrintDivision();
        cout << "[ Do ] Action " << OPTION_ADD_ENTRY << ": Add an entry to the NAT table" << endl
             << "[ Do ] Action " << OPTION_DELETE_ENTRY << ": Delete an entry of the NAT table" << endl
             << "[ Do ] Action " << OPTION_SEND_MESSAGE << ": Send a message to specific IP" << endl
             << "[ Do ] Action " << OPTION_CLOSE << ": Close connection" << endl << endl;

        cout << "[ Do ] Action: "; cin >> option;

        if (option == OPTION_ADD_ENTRY)
        {
            int source_port = client_info->port;
            int source_nat_port = client_info->port;
            int destination_nat_port = -1;
            string source_ip = client_info->my_ip;
            string destination_nat_ip = "";
            char type_entry;

            PrintDivision();
            cout << "[ Action A ] Destination NAT IP: "; cin >> destination_nat_ip;
            cout << "[ Action A ] Destination NAT port: "; cin >> destination_nat_port;
            cout << "[ Action A ] Type of entry (u/c): "; cin >> type_entry;

            packageSize = 0;
            ActionA_PACKAGE(buffer,
                            source_port,
                            source_nat_port,
                            destination_nat_port,
                            source_ip,
                            destination_nat_ip,
                            type_entry,
                            packageSize);

            n = write(socketFD, buffer, packageSize);

            if (n < 0) perror("[ Action A ] Error, write in socket");
            else cout << "[ Action A ] OK" << endl;
        }

        else if (option == OPTION_DELETE_ENTRY)
        {
            int entry_id;

            PrintDivision();
            cout << "[ Action D ] Entry ID: "; cin >> entry_id;

            packageSize = 0;
            ActionD_PACKAGE(buffer, client_info->my_ip, entry_id, packageSize);

            n = write(socketFD, buffer, packageSize);

            if (n < 0) perror("[ Action D ] Error, write in socket");
            else cout << "[ Action D ] OK" << endl;
        }

        else if (option == OPTION_SEND_MESSAGE)
        {
            string ip = "";
            string message = "";

            PrintDivision();
            cout << "[ Action M ] IP: "; cin >> ip;
            cout << "[ Action M ] Message: "; cin >> message;

            packageSize = 0;
            ActionM_PACKAGE(buffer, ip, message, packageSize);

            n = write(socketFD, buffer, packageSize);

            if (n < 0) perror("[ Action M ] Error, write in socket");
            else cout << "[ Action M ] OK" << endl;
        }

        else if (option == OPTION_CLOSE)
        {
            break;
        }

        else
        {
            cout << "[ Message ] Error, invalid option";
        }
    }
}

void ClientREAD_THREAD(int socketFD, bool& online, ClientInfo* client_info)
{
    char* buffer = new char[ buffer_size ];
    int n;

    while (online)
    {
        n = read(socketFD, buffer, 1);
        if (n < 0) perror("[ Read ] Error, reading from socket");

        char action = buffer[0];

        if (action == 'M')
        {
            string ip;
            int message_size;
            string message;

            n = read(socketFD, buffer, 15);
            if (n < 0) perror("[ Action M ] Error, reading from socket");
            ip = Read_STR(buffer, 0, 15);

            n = read(socketFD, buffer, 4);
            if (n < 0) perror("[ Action M ] Error, reading from socket");
            message_size = Read_INT(buffer, 0, 4);

            n = read(socketFD, buffer, message_size);
            if (n < 0) perror("[ Action M ] Error, reading from socket");
            message = Read_STR(buffer, 0, message_size);

            PrintDivision();
            cout << "[ Action M ] " << ip << ": " << message << endl;
            PrintDivision();
        }
    }
}

#endif
