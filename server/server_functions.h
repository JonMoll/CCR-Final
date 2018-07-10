#ifndef SERVER_FUNCTIONS_H
#define SERVER_FUNCTIONS_H

#include "includes.h"
#include "protocols.h"

vector<pair<string,string>> connections;


void PrintUsers(vector< pair< string, int > >& clients)
{

    for (int i=0; i<clients.size(); i++)
    {
        MAP_MUTEX.lock();
        cout << yellow << clients[i].first << reset <<endl;
        MAP_MUTEX.unlock();
    }
}

void GetData(int socket, pair< int, string >& data)
{

    //el envio de la data es automatico a la respuesta anterior [ACTION N] del servidor
    //busy waiting
    char* buffer = new char[256];
    int n;
    n = read(socket, buffer, 1);
    if (n < 0) perror("ERROR: Reading from socket");

    char action = buffer[0];
    if(action== 'D'){

        int user_port;
        string user_ip;
        //--------------------------------------------------
        n = read(socket, buffer, 4);
        if (n < 0) perror("ERROR: Reading from socket");

        user_port = Read_INT(buffer, 0, 4);
        cout << "user port: " << user_port << endl;
        //--------------------------------------------------

        user_ip = "127.0.0.1";
        //--------------------------------------------------

        data.first=user_port;
        data.second=user_ip;
    }
    else {
        //error, mala accion
    }
}

void Server_Listener(int client_socket, vector< pair< string, int > >& clients)
{
    char* buffer = new char[256];
    int n;
    bool online = true;

    while (online)
    {

        // LEYENDO LA ACCION --------------------------------------------------

        n = read(client_socket, buffer, 1);
        if (n < 0) perror("ERROR: Reading from socket");

        char action = buffer[0];
        cout << "Action " << action << endl;

        // EJECUTANDO LA ACCION --------------------------------------------------
        // Action L: Login to game -------------------------
        if (action == 'L')
        {
            /*
            int user_port;
            string user_ip;
            //--------------------------------------------------
            n = read(client_socket, buffer, 4);
            if (n < 0) perror("ERROR: Reading from socket");

            user_port = Read_INT(buffer, 0, 4);
            cout << "user port: " << user_port << endl;
            //--------------------------------------------------
            n = read(client_socket, buffer, 15);
            if (n < 0) perror("ERROR: Reading from socket");

            user_ip = Read_STR(buffer, 0, 15);
            cout << "user ip: " << user_ip << endl;
            //--------------------------------------------------
            */
            n = read(client_socket, buffer, 4);
            if (n < 0) perror("ERROR: Reading from socket");

            int nickname_size = Read_INT(buffer, 0, 4);
            //--------------------------------------------------
            n = read(client_socket, buffer, nickname_size);
            if (n < 0) perror("ERROR: Reading from socket");

            string usernick = Read_STR(buffer, 0, nickname_size);
            PrintUsers(clients);

            //New User
            CLIENT_MUTEX.lock();
            clients.push_back(make_pair(usernick,client_socket));
            CLIENT_MUTEX.unlock();

        }
        else if (action == 'O')
        {
            n = read(client_socket, buffer, 4);
            if (n < 0) perror("ERROR: Reading from socket");

            int nickname_size = Read_INT(buffer, 0, 4);
            //--------------------------------------------------
            n = read(client_socket, buffer, nickname_size);
            if (n < 0) perror("ERROR: Reading from socket");

            string usernick = Read_STR(buffer, 0, nickname_size);
            cout << "Client nickname: " << usernick << endl;

            //New User
            CLIENT_MUTEX.lock();
            //buscamos a nuestro usuario
            int usuario = 0;
            for (int i=0; i<clients.size(); i++)
            {
                if(clients[i].second == client_socket)
                {
                    usuario=i;
                }
            }
            clients.erase(clients.begin()+usuario);
            CLIENT_MUTEX.unlock();
        }

        // ACCION P: LISTA DE USUARIOS CONECTADOS--------------------------------------------------
        else if (action == 'P')
        {
            // JUNTANDO LISTA DE USUARIOS --------------------------------------------------

            string list_users = "";

            for (int i = 0; i < clients.size(); i++)
                list_users += clients[i].first + " ";

            // RESPONDIENDO --------------------------------------------------

            int packageSize = 0;
            ActionP_PACKAGE(buffer, list_users, packageSize);
            cout <<buffer << endl;

            n = write(client_socket, buffer, packageSize);
            if (n < 0) perror("[ Write ][ Action P ] Error, write in socket");

            cout << endl << "[ Read ][ Action P ] OK" << endl;

        }
        else if (action == 'C')
        {
            n = read(client_socket, buffer, 4);
            if (n < 0) perror("ERROR: Reading from socket");

            int nickname_size = Read_INT(buffer, 0, 4);
            //--------------------------------------------------
            n = read(client_socket, buffer, nickname_size);
            if (n < 0) perror("ERROR: Reading from socket");

            string usernick = Read_STR(buffer, 0, nickname_size);

            CLIENT_MUTEX.lock();
            //buscamos a nuestro usuario
            pair< string, int > usuario_destino;
            pair< string, int > usuario_solitante;
            int ok=0;
            for (int i=0; i<clients.size(); i++)
            {
                if(clients[i].first == usernick)
                {
                    usuario_destino=clients[i];
                }
                if(clients[i].second == client_socket)
                {
                    usuario_solitante=clients[i];
                    ok++;
                }
            }
            CLIENT_MUTEX.unlock();

            cout << blue<< "INICIANDO CONEXION DE LOS SIGUIENTES USUARIOS" << reset<<endl;
            cout << yellow<< usuario_solitante.first << reset<<endl;
            cout << yellow<< usuario_destino.first << reset<<endl;
            connections.push_back(make_pair(usuario_solitante.first,usuario_destino.first));
            bool sol_response=false;

            if(ok == 1) //existe ENTONCES SE INTERCAMBIAN DATOS
            {
                int packageSize = 0;                                


                //*************************
                //socitar data
                cout<<"sssssssssss"<<endl;
                ActionV_PACKAGE(buffer,packageSize);
                n = write(usuario_solitante.second, buffer, packageSize);

                if (n < 0) perror("[ solicitud ] Error, write in socket");
                else cout << "[ solicitud ] OK" << endl;

                cout<<"sssssssssss"<<endl;
                packageSize=0;
                ActionV_PACKAGE(buffer,packageSize);
                n = write(usuario_destino.second, buffer, packageSize);

                if (n < 0) perror("[ solicitud ] Error, write in socket");
                else cout << "[ solicitud ] OK" << endl;
                //*************************

                //t_data_des.join();
                //t_data_sol.join();
                //ya tenemos la data de ambos, ahora la intercambiamos
                //******************************

            }

        }
        else if (action == 'Q')
        {
            // JUNTANDO RECIVIR Y ENTREGAR --------------------------------------------------


            // RESPONDIENDO --------------------------------------------------

            int nickname_size;
            string nickname;
            int port;
            string ip;

            n = read(client_socket, buffer, 4);//PORT
            if (n < 0) perror("[ Action M ] Error, reading from socket");
            port = Read_INT(buffer, 0, 4);

            n = read(client_socket, buffer, 15);//IP
            if (n < 0) perror("[ Action M ] Error, reading from socket");
            ip = Read_STR(buffer, 0, 15);

            n = read(client_socket, buffer, 4);//nickname
            if (n < 0) perror("[ Action M ] Error, reading from socket");
            nickname_size = Read_INT(buffer, 0, 4);

            n = read(client_socket, buffer, nickname_size);//nicknamesize
            if (n < 0) perror("[ Action M ] Error, reading from socket");
            nickname = Read_STR(buffer, 0, nickname_size);

            string usuario_otro;
            int otro_socket;
            for (int i=0; i<connections.size(); i++)
            {
                if(connections[i].first == nickname) usuario_otro=connections[i].second;
                if(connections[i].second == nickname) usuario_otro=connections[i].first;
            }
            for (int i=0; i<clients.size(); i++)
            {
                if(clients[i].first == usuario_otro)
                {
                    otro_socket=clients[i].second;
                }

            }
            cout<<blue<<"el usuario " << nickname<<" le envia a "<< usuario_otro<<" sis datos pueto,ip "<< port<<" "<<ip<<reset<<endl;
            int packageSize = 0;
            ActionQ_PACKAGE(buffer, port, ip, nickname, packageSize);
            n = write(otro_socket, buffer, packageSize);
            if (n < 0) perror("[ solicitud ] Error, write in socket");
            else cout << "[ solicitud ] OK" << endl;

            cout << endl << "[ Read ][ Action D ] OK" << endl;

        }

    }
}

void ServerThread(int server_socket, vector< pair< string, int > >& clients)
{
    do{
        int ConnectFD;
        ConnectFD = accept(server_socket, NULL, NULL);
        thread esp(Server_Listener, ConnectFD, ref(clients));
        esp.detach();
    }while(true);
}



#endif // SERVER_FUNCTIONS_H
