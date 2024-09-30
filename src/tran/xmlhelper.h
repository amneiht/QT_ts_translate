#ifndef XMLHELPER_H
#define XMLHELPER_H

#include <QObject>
#include <libxml/tree.h>
#include <list>
#include <string>

typedef bool (*XmlFildChild)(xmlNodePtr node, void *data);
class Xmlhelper
{
public:
    /* get name in
    <context>
        <name>ActivateAppSipAccountWithEmail</name>
    </context>
    */
    static QString getNodeName(xmlNodePtr note);

    static xmlNodePtr findChildWithName(xmlNodePtr note, const QString &name);
    static QList<xmlNodePtr> getChildListWithName(xmlNodePtr note, const QString &name);

    // atributed
    static QString getAttributeValue(xmlNodePtr note, const QString &name);
    static void setAttribute(xmlNodePtr note, const QString &name, const QString &value);
    // find data in chile note
    static xmlNodePtr findChild(xmlNodePtr ptr, XmlFildChild cb, void *data = nullptr);
};

#endif // XMLHELPER_H
