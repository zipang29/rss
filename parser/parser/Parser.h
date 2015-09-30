#ifndef PARSER_H
#define PARSER_H

#include <QDomElement>
#include <QUrl>

class Parser
{
    public:
        Parser();
        void parse(QUrl url);
        void readItem(const QDomElement & item);

    private:

};

#endif // PARSER_H
