#ifndef CLIENT_SERVER_FUNCTIONS_H
#define CLIENT_SERVER_FUNCTIONS_H

#include "client_info.h"
#include "socket_functions.h"
#include "protocols.h"
using namespace std;
mutex SERVER_MUTEX;

int puerto_que_abrira=8888;
string nickname_client="jeff";
string ip_glabal="192.168.0.0";

void ClientSSEND_THREAD(int socketFD, bool& online, ClientInfo* client_info)
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
        cout << "[ Do ] Action " << 0 << ": LOGING" << endl
             << "[ Do ] Action " << 1 << ": VER USUARIOS REGISTRADOS" << endl
             << "[ Do ] Action " << 2 << ": INICIAR CONEXION CON ALGUIEN" << endl
             << "[ Do ] Action " << 7 << ": SETEAR ip" << endl
             << "[ Do ] Action " << 8 << ": SETEAR PUERTO" << endl <<endl;


        cout << "[ Do ] Action: "; cin >> option;

        if (option == 0)
        {
            string nickname;

            PrintDivision();
            cout << "[ Action L ] Su nickname: "; cin >> nickname;
            nickname_client = nickname;

            packageSize = 0;
            ActionL_PACKAGE(buffer,nickname,packageSize);

            n = write(socketFD, buffer, packageSize);

            if (n < 0) perror("[ Action A ] Error, write in socket");
            else cout << "[ Action A ] OK" << endl;
        }

        else if (option == 1)
        {
            PrintDivision();

            packageSize = 0;
            ActionP_PACKAGE(buffer, packageSize);

            n = write(socketFD, buffer, packageSize);

            if (n < 0) perror("[ Action D ] Error, write in socket");
            else cout << "[ Action D ] OK" << endl;
        }

        else if (option == 2)
        {
            string nickname;

            PrintDivision();
            cout << "[ Action C ] nickname de la persona con quien desea conectar: "; cin >> nickname;

            packageSize = 0;
            ActionC_PACKAGE(buffer,nickname,packageSize);

            n = write(socketFD, buffer, packageSize);

            if (n < 0) perror("[ Action C ] Error, write in socket");
            else cout << "[ Action C ] OK" << endl;


        }
        else if(option==8){
            cout << "[ SET ] setee su ip: ";
            cin>>ip_glabal;
        }
        else if(option==8){
            cout << "[ SET ] setee su puerto: ";
            cin>>puerto_que_abrira;
        }

        else if (option == OPTION_CLOSE)
        {
            online = false;
            break;
        }

        else
        {
            cout << "[ Message ] Error, invalid option";
        }
    }
    online = false;
}

void ClientSREAD_THREAD(int socketFD, bool& online, ClientInfo* client_info)
{
    char* buffer = new char[ buffer_size ];
    int n;

    while (online)
    {
        n = read(socketFD, buffer, 1);
        if (n < 0) perror("[ Read ] Error, reading from socket");

        char action = buffer[0];

        if (action == 'P')
        {
            int message_size;
            string message;

            n = read(socketFD, buffer, 4);
            if (n < 0) perror("[ Action M ] Error, reading from socket");
            message_size = Read_INT(buffer, 0, 4);

            n = read(socketFD, buffer, message_size);
            if (n < 0) perror("[ Action M ] Error, reading from socket");
            message = Read_STR(buffer, 0, message_size);

            PrintDivision();
            cout << blue << "LISTA DE USUARIOS" << reset << endl;
            cout << "[ Action P ] " << ": " << message << endl;
            PrintDivision();
        }
        else if(action == 'V')
        {
            int puerto = puerto_que_abrira;
            string ip="192.168.0.1";
            PrintDivision();
            cout<<blue << "[ Action V ] Datos de conexion(entrada en nata) "<<reset<<endl;
            cout<<blue << "[ Action V ] ip: "<<ip <<reset<<endl;
            cout<<blue << "[ Action V ] puerto: "<<puerto <<reset<<endl;
            //cout<< "[ Action V ] puerto : "; cin>>puerto;
            //cout<< "[ Action V ] ip: "; cin>>ip;

            int packageSize = 0;
            ActionQ_PACKAGE(buffer,puerto,ip,nickname_client,packageSize);
            n = write(socketFD, buffer, packageSize);
            if (n < 0) perror("[ Action V ] Error, write in socket");
            else cout << "[ Action V ] OK" << endl;

        }
        else if(action == 'Q')
        {
            int nickname_size;
            string nickname;
            int port;
            string ip;

            n = read(socketFD, buffer, 4);//PORT
            if (n < 0) perror("[ Action M ] Error, reading from socket");
            port = Read_INT(buffer, 0, 4);

            n = read(socketFD, buffer, 15);//IP
            if (n < 0) perror("[ Action M ] Error, reading from socket");
            ip = Read_STR(buffer, 0, 15);

            n = read(socketFD, buffer, 4);//nickname
            if (n < 0) perror("[ Action M ] Error, reading from socket");
            nickname_size = Read_INT(buffer, 0, 4);

            n = read(socketFD, buffer, nickname_size);//nicknamesize
            if (n < 0) perror("[ Action M ] Error, reading from socket");
            nickname = Read_STR(buffer, 0, nickname_size);

            PrintDivision();
            cout << blue << "DATOS DEL OTRO USUARIO" << reset << endl;
            cout << "[ Action D ] " << "El cliente: " << nickname << endl;
            cout << "[ Action D ] " << "abre puerto: " << port << endl;
            cout << "[ Action D ] " << "abre ip: " << ip << endl;
            PrintDivision();
            online=false;
        }


    }
}

#endif // CLIENT_SERVER_FUNCTIONS_H
