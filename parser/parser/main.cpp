#include <QCoreApplication>
#include <Parser.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Parser p;

    return a.exec();
}

