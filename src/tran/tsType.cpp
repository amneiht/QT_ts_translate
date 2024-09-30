#include "tsmessage.h"

TsContent::TsContent(xmlNodePtr node)
{
    this->node = node;
}

TsMessage::Type TsContent::getType() const
{
    return m_type;
}

//################# TsSring ############
TsString::TsString(xmlNodePtr snode)
    : TsContent(snode)
{}

void TsString::setContent(const QString &content)
{
    QByteArray qba = content.toLocal8Bit();
    char *fct = qba.data();
    xmlNodeSetContent(node, (xmlChar *) fct);
}

QString TsString::getContent() const
{
    if (node->children == nullptr)
        return "";
    return (const char *) node->children->content;
}

//######################## TsNumerus ##############
TsNumerus::TsNumerus(xmlNodePtr snode)
    : TsContent(snode)
{
    auto child = snode->children;
    int count = 2;
    while (count > 0 && child != nullptr) {
        if (strcmp((char *) child->name, "numerusform") == 0) {
            count--;
            if (count == 1)
                m_base = child;
            else
                m_numerus = child;
        }
        child = child->next;
    }
    // check numeres because qt is stupid
    if (count == 2) {
        m_base = xmlNewChild(snode, nullptr, (const xmlChar *) "numerusform", nullptr);
    }
    if (count > 0) {
        m_numerus = xmlNewChild(snode, nullptr, (const xmlChar *) "numerusform", nullptr);
    }
}

QString TsNumerus::getBase() const
{
    if (m_base) {
        auto pchar = xmlNodeGetContent(m_base);
        return (char *) pchar;
    } else
        return "";
}

void TsNumerus::setBase(const QString &data)
{
    if (m_base) {
        QByteArray qba = data.toLocal8Bit();
        char *fct = qba.data();
        xmlNodeSetContent(m_base, (xmlChar *) fct);
    }
}

QString TsNumerus::getNumerus() const
{
    if (m_numerus) {
        auto pchar = xmlNodeGetContent(m_numerus);
        return (char *) pchar;
    } else
        return "";
}

void TsNumerus::setNumerus(const QString &data)
{
    if (m_numerus) {
        QByteArray qba = data.toLocal8Bit();
        char *fct = qba.data();
        xmlNodeSetContent(m_numerus, (xmlChar *) fct);
    }
}
