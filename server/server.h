#ifndef SERVER_H
#define SERVER_H

#include "includes.h"
#include "server_functions.h"

using namespace std;

void HostService()
{
    // DATOS DEL SOCKET --------------------------------------------------

    struct sockaddr_in stSockAddr;
    int port = 6789;

    // CREANDO EL SOCKET --------------------------------------------------

    int SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (-1 == SocketFD)
    {
        perror("ERROR: Can not create socket");
        exit(EXIT_FAILURE);
    }

    // INGRESANDO DATOS PARA LA CONEXION --------------------------------------------------
    cout << blue << "CONETION DATA"<< endl;
    cout << "Port: " << reset;
    cout << green << port << reset << endl;


    // CONFIGURANDO Y INICIANDO EL SOCKET --------------------------------------------------

    memset(&stSockAddr, 0, sizeof(struct sockaddr_in));

    stSockAddr.sin_family = AF_INET;
    stSockAddr.sin_port = htons(port);
    stSockAddr.sin_addr.s_addr = INADDR_ANY;

    if (-1 == bind(SocketFD, (const struct sockaddr *)&stSockAddr, sizeof(struct sockaddr_in)))
    {
        perror("ERROR: Bind failed");
        close(SocketFD);
        exit(EXIT_FAILURE);
    }

    if (-1 == listen(SocketFD, 10))
    {
        perror("ERROR: Listen failed");
        close(SocketFD);
        exit(EXIT_FAILURE);
    }

    // INICIANDO EL SERVIDOR --------------------------------------------------

    vector< pair< string, int > > m_clients; //nickname - soketdescriptor
    vector< pair< int, string > > m_clients_data; //puerto-ip

    cout << "----- SERVER ONLINE -----" << endl;

    for(;;)
    {

        thread t_ServerThread(ServerThread, SocketFD, ref(m_clients));
        t_ServerThread.detach();
        do
        {
           // SERVER RUNING
        }
        while (true);

    }

    close(SocketFD);
}

#endif // SERVER_H
