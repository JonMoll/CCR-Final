#include "nat.h"
#include "protocols.h"
#include "protocol_functions.h"

using namespace std;

mutex NAT_MUTEX;

void NatREAD_THREAD(int socketFD, Nat* ptr_nat)
{
    char* buffer = new char[ buffer_size ];
    int n = 1;
    bool online = true;

    while (online)
    {
        n = read(socketFD, buffer, 1);
        if (n < 0) perror("[ Read ] Error, reading from socket");
        char action = buffer[0];

        if (action == 'E')
        {
            n = read(socketFD, buffer, 15);
            if (n < 0) perror("[ Action E ] Error, reading from socket");
            string ip = Read_STR(buffer, 0, 15);

            ptr_nat->clients.emplace_back(ip, socketFD);

            cout << "[ NAT ] New connection: " << ip << endl;
            ptr_nat->PrintDivision();
        }

        else if (action == 'A')
        {
            ptr_nat->PrintMyInfo();
            cout << "[ Action A ] Read" << endl;

            int source_port;
            int source_nat_port;
            int destination_nat_port;
            string source_ip;
            string destination_nat_ip;
            int entry_id;
            char type_entry;

            n = read(socketFD, buffer, 4);
            if (n < 0) perror("[ Action A ] Error, reading from socket");
            source_port = Read_INT(buffer, 0, 4);

            n = read(socketFD, buffer, 4);
            if (n < 0) perror("[ Action A ] Error, reading from socket");
            source_nat_port = Read_INT(buffer, 0, 4);

            n = read(socketFD, buffer, 4);
            if (n < 0) perror("[ Action A ] Error, reading from socket");
            destination_nat_port = Read_INT(buffer, 0, 4);

            n = read(socketFD, buffer, 15);
            if (n < 0) perror("[ Action A ] Error, reading from socket");
            source_ip = Read_STR(buffer, 0, 15);

            n = read(socketFD, buffer, 15);
            if (n < 0) perror("[ Action A ] Error, reading from socket");
            destination_nat_ip = Read_STR(buffer, 0, 15);

            n = read(socketFD, buffer, 1);
            if (n < 0) perror("[ Action A ] Error, reading from socket");
            type_entry = buffer[0];

            int error = ptr_nat->AddEntry(destination_nat_ip,
                                          destination_nat_port,
                                          ptr_nat->ip,
                                          source_nat_port,
                                          source_ip,
                                          source_port,
                                          entry_id,
                                          type_entry);

            if (error == NO_ERRORS)
            {
                cout << "[ Action A ] OK" << endl << endl;

                int client_index = ptr_nat->SearchClientSocketFD(socketFD);
                ptr_nat->clients[client_index]._entries_id.push_back(entry_id);

                ptr_nat->PrintTable();
                ptr_nat->PrintDivision();
            }
            else
            {
                ptr_nat->PrintError(error);

                ptr_nat->PrintTable();
                ptr_nat->PrintDivision();
            }
        }

        else if (action == 'D')
        {
            ptr_nat->PrintMyInfo();
            cout << "[ Action D ] Read" << endl;

            string ip;
            int entry_id;

            n = read(socketFD, buffer, 15);
            if (n < 0) perror("[ Action D ] Error, reading from socket");
            ip = Read_STR(buffer, 0, 15);

            n = read(socketFD, buffer, 1);
            if (n < 0) perror("[ Action D ] Error, reading from socket");
            entry_id = Read_INT(buffer, 0, 1);

            int error = ptr_nat->DeleteEntry(entry_id, socketFD);

            if (error == NO_ERRORS)
            {
                cout << "[ Action D ] OK" << endl << endl;

                int client_index = ptr_nat->SearchClientIP(ip);
                int entry_index = ptr_nat->clients[client_index].SearchEntry(entry_id);
                ptr_nat->clients[client_index]._entries_id.erase(ptr_nat->clients[client_index]._entries_id.begin()+entry_index);

                ptr_nat->PrintTable();
                ptr_nat->PrintDivision();
            }
            else
            {
                ptr_nat->PrintError(error);

                ptr_nat->PrintTable();
                ptr_nat->PrintDivision();
            }
        }

        else if (action == 'M')
        {
            ptr_nat->PrintMyInfo();
            cout << "[ Action M ] Read" << endl;

            string ip = "";
            int message_size = 0;
            string message = "";

            n = read(socketFD, buffer, 15);
            if (n < 0) perror("[ Action M ] Error, reading from socket");
            ip = Read_STR(buffer, 0, 15);

            n = read(socketFD, buffer, 4);
            if (n < 0) perror("[ Action M ] Error, reading from socket");
            message_size = Read_INT(buffer, 0, 4);

            n = read(socketFD, buffer, message_size);
            if (n < 0) perror("[ Action M ] Error, reading from socket");
            message = Read_STR(buffer, 0, message_size);

            // REENVIAR MENSAJE --------------------------------------------------

            int index_receiver = ptr_nat->SearchClientIP(ip);

            if (index_receiver == CLIENT_NOT_EXIST)
            {
                cout << "[ Action M ] The IP " << ip << " is not connected to the NAT" << endl;
            }
            else
            {
                int socketFD_receiver = ptr_nat->clients[index_receiver]._socketFD;

                int index_sender = ptr_nat->SearchClientSocketFD(socketFD);
                string ip_sender = ptr_nat->clients[index_sender]._ip;

                int packageSize = 0;
                ActionM_PACKAGE(buffer, ip_sender, message, packageSize);

                n = write(socketFD_receiver, buffer, packageSize);
                if (n < 0) perror("[ Action M ] Error, write in socket");
                else cout << "[ Action M ] OK" << endl;
            }

            ptr_nat->PrintDivision();
        }
    }
}

void Nat_THREAD(int socketFD, Nat* ptr_nat)
{
    while (true)
    {
        int ConnectFD;
        ConnectFD = accept(socketFD, NULL, NULL);

        thread t_read(NatREAD_THREAD, ConnectFD, ptr_nat);
        t_read.detach();
    }
}

Client::Client(string ip, int socketFD)
{
    _ip = ip;
    _socketFD = socketFD;
}

int Client::SearchEntry(int entry_id)
{
    for (unsigned int i = 0; i < _entries_id.size(); i++)
        if (_entries_id[i] == entry_id)
            return i;

    return ENTRY_NOT_EXIST;
}

Nat::Nat()
{
    nat_table = new NatTable();
    ip = "127.0.0.1";

    available_entryID.push_back(0);
    available_entryID.push_back(1);
    available_entryID.push_back(2);
    available_entryID.push_back(3);
    available_entryID.push_back(4);
    available_entryID.push_back(5);
    available_entryID.push_back(6);
    available_entryID.push_back(7);
    available_entryID.push_back(8);
    available_entryID.push_back(9);
}

void Nat::StartNat()
{
    // DATOS DEL SOCKET --------------------------------------------------

    struct sockaddr_in stSockAddr;

    // CREANDO EL SOCKET --------------------------------------------------

    SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (-1 == SocketFD)
    {
        perror("[ Connection ] Error, can not create socket");
        exit(EXIT_FAILURE);
    }

    // INGRESANDO DATOS PARA LA CONEXION --------------------------------------------------

    cout << "[ Connection ] My IP (Local 127.0.0.1): "; cin >> ip;
    cout << "[ Connection ] Port: "; cin >> port;

    // CONFIGURANDO Y INICIANDO EL SOCKET --------------------------------------------------

    memset(&stSockAddr, 0, sizeof(struct sockaddr_in));

    stSockAddr.sin_family = AF_INET;
    stSockAddr.sin_port = htons(port);
    stSockAddr.sin_addr.s_addr = INADDR_ANY;

    if (-1 == bind(SocketFD, (const struct sockaddr *)&stSockAddr, sizeof(struct sockaddr_in)))
    {
        perror("[ Connection ] Error, bind failed");
        close(SocketFD);
        exit(EXIT_FAILURE);
    }

    if (-1 == listen(SocketFD, 10))
    {
        perror("[ Connection ] Error, listen failed");
        close(SocketFD);
        exit(EXIT_FAILURE);
    }

    // INICIANDO EL SERVIDOR --------------------------------------------------

    cout << green << "[ Connection ] OK" << reset << endl;
    PrintDivision();

    for(;;)
    {
        thread t_server(Nat_THREAD, SocketFD, this);
        t_server.detach();

        do
        {
            // WAITING TO CLOSE CONNECTION
        }
        while (true);
    }

    close(SocketFD);
}

int Nat::AddEntry(string public_ip, int public_port,
                  string nat_ip, int nat_port,
                  string private_ip, int private_port,
                  int& entry_id, char type_entry)
{
    if (nat_table->_rows.size() < nat_table->_rows_limit)
    {
        entry_id = GenerateEntryID();

        nat_table->InsertRow(public_ip, public_port,
                             nat_ip, nat_port,
                             private_ip, private_port,
                             entry_id, type_entry);

        return NO_ERRORS;
    }
    else
    {
        return ERROR_TABLE_FULL;
    }
}

int Nat::DeleteEntry(int entry_id, int socketFD)
{
    int client_index = SearchClientSocketFD(socketFD);

    if (client_index != CLIENT_NOT_EXIST)
    {
        int entry_id_index = clients[client_index].SearchEntry(entry_id);

        if (entry_id_index != ENTRY_NOT_EXIST)
        {
            nat_table->DeleteRow(entry_id);
            available_entryID.push_back(entry_id);

            return NO_ERRORS;
        }
        else
        {
            return ERROR_ENTRY_ANOTHER_CLIENT;
        }
    }
    else
    {
        return ERROR_DELETE_ENTRY;
    }
}

int Nat::SearchClientSocketFD(int socketFD)
{
    for (unsigned int i = 0; i < clients.size(); i++)
        if (clients[i]._socketFD == socketFD)
            return i;

    return CLIENT_NOT_EXIST;
}

int Nat::SearchClientIP(string ip)
{
    for (unsigned int i = 0; i < clients.size(); i++)
        if (clients[i]._ip == ip)
            return i;

    return CLIENT_NOT_EXIST;
}

int Nat::TraslateAdress(string public_ip, int public_port)
{
    for (unsigned int i = 0; i < nat_table->_rows.size(); i++)
    {
        if (nat_table->_rows[i]._public_ip == public_ip &&
            nat_table->_rows[i]._public_port == public_port)
        {
            string private_ip = nat_table->_rows[i]._private_ip;

            int client_index = SearchClientIP(private_ip);

            if (client_index != CLIENT_NOT_EXIST)
            {
                return client_index;
            }
            else
            {
                return TRASLATE_CLIENT_NOT_EXIST;
            }
        }
    }

    return TRASLATE_NOT_EXIST;
}

void Nat::PrintDivision()
{
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    string division = "";
    string division_element = "=";

    for (unsigned int i = 0; i < w.ws_col; i++)
        division += division_element;

    cout << endl << division << endl << endl;
}

void Nat::PrintTable()
{
    char separator = ' ';
    int width_ip = 17;
    int width_int = 13;
    int width_str = 13;
    int width_table = 0;

    cout << left << setw(width_ip) << setfill(separator) << "Public IP"; width_table += width_ip;
    cout << left << setw(width_int) << setfill(separator) << "Public Port"; width_table += width_int;
    cout << left << setw(width_ip) << setfill(separator) << "NAT IP";  width_table += width_ip;
    cout << left << setw(width_int) << setfill(separator) << "NAT Port";  width_table += width_int;
    cout << left << setw(width_ip) << setfill(separator) << "Private IP";  width_table += width_ip;
    cout << left << setw(width_int) << setfill(separator) << "Private Port";  width_table += width_int;
    cout << left << setw(width_int) << setfill(separator) << "Entry ID";  width_table += width_int;
    cout << left << setw(width_str) << setfill(separator) << "Type entry";  width_table += width_str;
    cout << endl;

    string horizontal_line = "";
    for (unsigned int i = 0; i < width_table; i++)
        horizontal_line += "-";

    cout << horizontal_line << endl;

    for (unsigned int i = 0; i < nat_table->_rows.size(); i++)
    {
        cout << left << setw(width_ip) << setfill(separator) << nat_table->_rows[i]._public_ip;
        cout << left << setw(width_int) << setfill(separator) << nat_table->_rows[i]._public_port;
        cout << left << setw(width_ip) << setfill(separator) << nat_table->_rows[i]._nat_ip;
        cout << left << setw(width_int) << setfill(separator) << nat_table->_rows[i]._nat_port;
        cout << left << setw(width_ip) << setfill(separator) << nat_table->_rows[i]._private_ip;
        cout << left << setw(width_int) << setfill(separator) << nat_table->_rows[i]._private_port;
        cout << left << setw(width_int) << setfill(separator) << nat_table->_rows[i]._entry_id;
        cout << left << setw(width_str) << setfill(separator) << nat_table->_rows[i]._type_entry;
        cout << endl;
    }
    cout << horizontal_line << endl;
    cout << "clientes" << endl;
    for (int k =0;k<clients.size();k++)
    {
        cout << "client ip" << clients[k]._ip << endl;
    }
}

int Nat::GenerateEntryID()
{
    if (available_entryID.size() > 0)
    {
        int entryID = available_entryID[0];
        available_entryID.erase(available_entryID.begin());

        return entryID;
    }

    else
        return -1;
}

void Nat::PrintMyInfo()
{
    cout << "[ My IP: " << ip << " ][ Port open: " << port << " ]" << endl;
}

void Nat::PrintError(int error)
{
    if (error == ERROR_ADD_ENTRY)
        cout << endl << "[ NAT ] Could not add an entry to the NAT table" << endl << endl;

    else if (error == ERROR_TABLE_FULL)
        cout << endl << "[ NAT ] No more entries can be added to the NAT table" << endl<< endl;

    else if (error == ERROR_ENTRY_NOT_EXIST)
        cout << endl << "[ NAT ] The Entry ID does not exist" << endl << endl;

    else if (error == ERROR_ENTRY_ANOTHER_CLIENT)
        cout << endl << "[ NAT ] The ID of the entry to be deleted was added by another Client" << endl << endl;

    else if (error == ERROR_DELETE_ENTRY)
        cout << endl << "[ NAT ] Could not delete an entry of the NAT table" << endl << endl;

    else
        cout << endl << "[ NAT ] Error" << endl << endl;
}
