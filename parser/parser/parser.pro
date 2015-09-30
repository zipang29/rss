QT += core xml
QT -= gui

TARGET = parser
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    Item.cpp \
    Parser.cpp

HEADERS += \
    Item.h \
    Parser.h
