QT += core xml network
QT -= gui

TARGET = parser
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    Item.cpp \
    Parser.cpp \
    ListItems.cpp \
    Constantes.cpp \
    Tika.cpp

HEADERS += \
    Item.h \
    Parser.h \
    ListItems.h \
    Constantes.h \
    Tika.h
