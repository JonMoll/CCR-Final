#ifndef CLIENT_H
#define CLIENT_H

#include "client_functions.h"

using namespace std;

void Client()
{
    // DATOS DEL SOCKET --------------------------------------------------

    struct sockaddr_in stSockAddr;
    ClientInfo client_info;
    int Res;

    // CREANDO EL SOCKET --------------------------------------------------

    int SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (-1 == SocketFD)
    {
        perror("[ Connection ] Error, can not create socket");
        exit(EXIT_FAILURE);
    }

    // INGRESANDO DATOS PARA LA CONEXION --------------------------------------------------

    cout << "[ Connection ] My IP: "; cin >> client_info.my_ip;
    cout << "[ Connection ] NAT IP (Local 127.0.0.1): "; cin >> client_info.nat_ip;
    cout << "[ Connection ] Port: "; cin >> client_info.port;

    // CONFIGURANDO Y INICIANDO EL SOCKET --------------------------------------------------

    memset(&stSockAddr, 0, sizeof(struct sockaddr_in));

    stSockAddr.sin_family = AF_INET;
    stSockAddr.sin_port = htons(client_info.port);
    Res = inet_pton(AF_INET, client_info.nat_ip.c_str(), &stSockAddr.sin_addr);

    if (0 > Res)
    {
        perror("[ Connection ] Error, first parameter is not a valid address family");
        close(SocketFD);
        exit(EXIT_FAILURE);
    }

    else if (0 == Res)
    {
        perror("[ Connection ] Error, second parameter does not contain valid ipaddress");
        close(SocketFD);
        exit(EXIT_FAILURE);
    }

    if (-1 == connect(SocketFD, (const struct sockaddr *)&stSockAddr, sizeof(struct sockaddr_in)))
    {
        perror("[ Connection ] Error, connect failed");
        close(SocketFD);
        exit(EXIT_FAILURE);
    }

    cout << green << "[ Connection ] OK" << reset << endl;
    PrintDivision();

    // INICIANDO EL CHAT --------------------------------------------------

    if (0 > SocketFD)
    {
        perror("[ Connection ] Error, accept failed");
        close(SocketFD);
        exit(EXIT_FAILURE);
    }

    // CREANDO Y INICIANDO LAS THREADS --------------------------------------------------

    bool online = true;

    thread t_send(ClientSEND_THREAD, SocketFD, ref(online), &client_info);
    thread t_read(ClientREAD_THREAD, SocketFD, ref(online), &client_info);

    t_send.detach();
    t_read.detach();

    do
    {
        // WAITING TO CLOSE CONNECTION
    }
    while (online != false);

    shutdown(SocketFD, SHUT_RDWR);
    close(SocketFD);
}

#endif
