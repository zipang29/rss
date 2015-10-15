#ifndef TIKA_H
#define TIKA_H

#include <QObject>
#include <QProcess>
#include <QNetworkAccessManager>
#include "Item.h"

class Tika : public QObject
{
    Q_OBJECT
public:
    static Tika* getInstance();
    static void destroy();

    void processItem(Item* item);

private:
    static Tika* instance;
    QProcess* tika_server;
    QNetworkAccessManager* accessManager;

    Tika();

    void detectLanguage(Item* item);
    void downloadLink(Item* item);

private slots:
    void setLanguage();

    void convertDocument();
    void parseDocument();

signals:
    void completed(Item* item);
};

#endif // TIKA_H
