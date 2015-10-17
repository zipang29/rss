#include <QCoreApplication>
#include "IO.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    IO io;
    io.readFeeds();

    return a.exec();
}
