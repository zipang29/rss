#include <QCoreApplication>
#include <Parser.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Parser p(QUrl("http://www.france24.com/en/timeline/rss"));

    return a.exec();
}

