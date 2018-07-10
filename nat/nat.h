#ifndef NAT_H
#define NAT_H

#include <stdio.h>
#include <iomanip> // print table
#include "nattable.h"
#include "socket_functions.h"

const int ENTRY_NOT_EXIST = -1;
const int CLIENT_NOT_EXIST = -1;
const int TRASLATE_NOT_EXIST = -1;
const int TRASLATE_CLIENT_NOT_EXIST = -2;

struct Client
{
    string _ip;
    int _socketFD;
    vector<int> _entries_id;

    Client(string ip, int socketFD);
    int SearchEntry(int entry_id);
};

class Nat
{
    private:
        NatTable* nat_table;
        int port;
        int SocketFD;

        vector<int> available_entryID;

        int GenerateEntryID();

    public:
        vector<Client> clients;
        string ip;

        Nat();
        void StartNat();
        int AddEntry(string public_ip, int public_port,
                     string nat_ip, int nat_port,
                     string private_ip, int private_port,
                     int& entry_id, char type_entry);
        int DeleteEntry(int entry_id, int socketFD);
        int SearchClientSocketFD(int socketFD);
        int SearchClientIP(string ip);
        int TraslateAdress(string public_ip, int public_port);
        void PrintTable();
        void PrintDivision();
        void PrintError(int error);
        void PrintMyInfo();
};

#endif
