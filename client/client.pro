TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += C++11

LIBS += -pthread

SOURCES += main.cpp

HEADERS += \
    client.h \
    client_functions.h \
    socket_functions.h \
    protocol_functions.h \
    protocols.h \
    client_info.h \
    client_server.h \
    client_server_functions.h
