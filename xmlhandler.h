#ifndef XMLHANDLER_H
#define XMLHANDLER_H
#include <QtXml>
#include <mainwindow.h>

class XMLHandler
{
public:
    XMLHandler();
    bool write(QString & path, map<QString, QString> values);
    map<QString, QString> read(QString & path);
    map<QString, QString> listElements(QDomElement root);
};

#endif // XMLHANDLER_H
