#ifndef INCLUDES_H
#define INCLUDES_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <thread>
#include <unistd.h>
#include <iostream>
//#include <file.h>
#include <vector>
#include <mutex>
//#include <protocols.h>

using namespace std;
//GLOBAL VARIABLES
mutex MAP_MUTEX;
mutex CLIENT_MUTEX;

const std::string reset("\033[0m");
const std::string red("\033[0;31m");
const std::string green("\033[0;32m");
const std::string blue("\033[0;34m");
const std::string yellow("\033[0;33m");
const std::string magenta("\033[0;35m");

#endif // INCLUDES_H
