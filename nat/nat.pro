TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += C++11

LIBS += -pthread

SOURCES += main.cpp \
    nattable.cpp \
    nat.cpp

HEADERS += \
    nat.h \
    socket_functions.h \
    protocol_functions.h \
    nattable.h \
    protocols.h
