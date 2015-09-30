QT += core xml
QT -= gui

TARGET = parser
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    Parser.cpp

HEADERS += \
    Parser.h

