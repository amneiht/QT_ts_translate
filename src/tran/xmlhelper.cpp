#include "xmlhelper.h"
#include <cstring>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlmemory.h>
#include <libxml/xpath.h>
#include <string>

QString Xmlhelper::getNodeName(xmlNodePtr note)
{
    auto nname = findChildWithName(note, "name");
    if (nname == nullptr)
        return "";
    QString ret((char *) nname->children->content);
    return ret;
}

xmlNodePtr Xmlhelper::findChildWithName(xmlNodePtr note, const QString &name)
{
    auto child = note->children;
    QByteArray qba = name.toLocal8Bit();
    char *fname = qba.data();
    while (child != nullptr) {
        if (strcmp((const char *) child->name, fname) == 0) {
            return child;
        }
        child = child->next;
    }
    return nullptr;
}

QList<xmlNodePtr> Xmlhelper::getChildListWithName(xmlNodePtr note, const QString &name)
{
    QList<xmlNodePtr> slist;
    QByteArray qba = name.toLocal8Bit();
    char *fname = qba.data();
    // ts/context
    auto child = note->children;
    while (child != nullptr) {
        if (strcmp((const char *) child->name, fname) == 0) {
            slist.append(child);
        }
        child = child->next;
    }
    return slist;
}

static xmlAttrPtr _findAtribute(xmlNodePtr note, const QString &name)
{
    QByteArray qba = name.toLocal8Bit();
    char *fname = qba.data();

    return xmlHasProp(note, (xmlChar *) fname);
}
QString Xmlhelper::getAttributeValue(xmlNodePtr note, const QString &name)
{
    //    auto attr = note->properties;
    //    QByteArray qba = name.toLocal8Bit();
    //    char *fname = qba.data();
    //    QString ret="";
    //    while (attr) {
    //        if (strcmp((const char *) attr->name, fname) == 0) {
    //            ret = (char *) attr->children->content;
    //        }
    //        attr = attr->next;
    //    }
    QString ret = "";
    auto attr = _findAtribute(note, name);
    if (attr) {
        ret = (char *) attr->children->content;
    }
    return ret;
}

void Xmlhelper::setAttribute(xmlNodePtr note, const QString &name, const QString &value)
{
    auto attr = _findAtribute(note, name);
    if (value == "") {
        if (attr) {
            xmlRemoveProp(attr);
        }
    } else {
        xmlSetProp(note,
                   (xmlChar *) name.toStdString().c_str(),
                   (xmlChar *) value.toStdString().c_str());
    }
}

xmlNodePtr Xmlhelper::findChild(xmlNodePtr ptr, XmlFildChild cb, void *data)
{
    auto child = ptr->children;
    while (child != nullptr) {
        if (cb(child, data)) {
            return child;
        }
        child = child->next;
    }
    return nullptr;
}
