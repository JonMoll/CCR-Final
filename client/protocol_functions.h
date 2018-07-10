#ifndef PROTOCOL_FUNCTIONS_H
#define PROTOCOL_FUNCTIONS_H

using namespace std;

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

#endif
