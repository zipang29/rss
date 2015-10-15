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
    IO.cpp \
    Constantes.cpp \
    Tika.cpp

HEADERS += \
    Item.h \
    Parser.h \
    ListItems.h \
    Constantes.h \
    IO.h \
    Tika.h

unix|win32: LIBS += -L$$PWD/kcwin32/lib/ -lkyotocabinet

INCLUDEPATH += $$PWD/kcwin32/include
DEPENDPATH += $$PWD/kcwin32/include
