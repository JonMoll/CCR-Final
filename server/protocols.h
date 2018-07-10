#ifndef PROTOCOLS_H
#define PROTOCOLS_H

#include "includes.h"

string IntToStr(int number, int sizeStr)
{
    string output = "";
    string number_str = to_string(number);

    if (number_str.size() > sizeStr)
    {
        perror("ERROR in 'IntToStr', number_str.size() is greater than sizeStr");
        return output;
    }

    for (int i = 0; i < ( sizeStr - number_str.size() ); i++)
        output += "0";

    output += number_str;

    return output;
}

// INSERTAR ENTERO EN EL BUFFER --------------------------------------------------

void Insert_INT(char* buffer, int beginIndex, int number, int bits)
{
    string number_STR = to_string(number);
    int i, j;

    for (i = beginIndex; i < ( beginIndex + (bits - number_STR.size()) ); i++)
        buffer[i] = '0';

    for(j = 0; j < number_STR.size(); j++, i++)
        buffer[i] = number_STR[j];
}

// LEER ENTERO EN EL BUFFER --------------------------------------------------

int Read_INT(char* buffer, int beginIndex, int bits)
{
    string number_STR = "";
    for (int i = beginIndex; i < (beginIndex + bits); i++)
        number_STR += buffer[i];

    return atoi(number_STR.c_str());
}

// INSERTAR CADENA EN EL BUFFER --------------------------------------------------

void Insert_STR(char* buffer, int beginIndex, string text, int bits)
{
    int i, j;

    for (i = beginIndex; i < ( beginIndex + (bits - text.size()) ); i++)
        buffer[i] = ' ';

    for(j = 0; j < text.size(); j++, i++)
        buffer[i] = text[j];
}

// LEER CADENA EN EL BUFFER --------------------------------------------------

string Read_STR(char* buffer, int beginIndex, int bits)
{
    string text = "";
    for (int i = beginIndex; i < bits; i++)
        if (buffer[i] != ' ')
            text += buffer[i];

    return text;
}

//***************************************************
//---------------------PACKS-------------------------
//***************************************************

//---------------------L-LOGIN-----------------------
string ActionL_PACKAGE(string nickname)
{
    cout << endl << "[Action L] PACKAGE: STARTING" << endl;

    string output = "";
    output += IntToStr(nickname.size(), 4); // 4B: Size of nickname
    output += "L"; // 1B: Action = L
    output += nickname; // nB: Nickname

    cout << "[Action L] PACKAGE: FINISHED" << endl << endl;

    return output;
}
//--------------ENVIAR LISTA DE USUARIOS---------------
void ActionP_PACKAGE(char* buffer, string message, int& packageSize) // ENVIAR MENSAJE A UN CLIENTE
{
    int beginIndex = 0;
    buffer[beginIndex] = 'P'; // 1B: ACCION
    beginIndex++;

    Insert_INT(buffer, beginIndex, message.size(), 4); // 4B: TAMANO DEL MENSAJE
    beginIndex += 4;

    Insert_STR(buffer, beginIndex, message,message.size()); // nB: MENSAJE

    packageSize = beginIndex + message.size();
}
//--------------ENVIAR LISTA DE USUARIOS---------------
void ActionC_PACKAGE(char* buffer, string message, int& packageSize) // ENVIAR MENSAJE A UN CLIENTE
{
    int beginIndex = 0;
    buffer[beginIndex] = 'C'; // 1B: ACCION
    beginIndex++;

    Insert_INT(buffer, beginIndex, message.size(), 4); // 4B: TAMANO DEL MENSAJE
    beginIndex += 4;

    Insert_STR(buffer, beginIndex, message,message.size()); // nB: MENSAJE

    packageSize = beginIndex + message.size();
}
//--------------ENVIAR RESPUESTA DE CONECCION---------------
void ActionN_PACKAGE(char* buffer, string response, int& packageSize) // ENVIAR MENSAJE A UN CLIENTE
{
    int beginIndex = 0;
    buffer[0] = 'N'; // 1B: ACCION
    beginIndex++;
    Insert_STR(buffer,beginIndex,response,response.size());
    packageSize = 2;
}
//------------------------ENVIAR DATOS-----------------------

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

void ActionV_PACKAGE(char* buffer,
                     int& packageSize)
{
    int currentIndex = 0;
    buffer[currentIndex] = 'V';
    currentIndex += 1;
    packageSize = currentIndex;
}

//***************************************************
//---------------------UNPACKS-----------------------
//***************************************************



#endif // PROTOCOLS_H
