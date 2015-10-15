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


unix:!macx: LIBS += -L$$PWD/kyotocabinet-1.2.76/ -lkyotocabinet

INCLUDEPATH += $$PWD/kyotocabinet-1.2.76
DEPENDPATH += $$PWD/kyotocabinet-1.2.76

unix:!macx: PRE_TARGETDEPS += $$PWD/kyotocabinet-1.2.76/libkyotocabinet.a
