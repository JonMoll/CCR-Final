#ifndef CLIENT_INFO_H
#define CLIENT_INFO_H

#include <string>

using namespace std;

struct ClientInfo
{
    string my_ip;
    int port;
    string nat_ip;

    ClientInfo()
    {
        my_ip = "192.192.192.192";
        port = 1456;
        nat_ip = "127.0.0.1";
    }
};

#endif
