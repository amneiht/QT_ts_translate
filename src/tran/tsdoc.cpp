#include "tsdoc.h"
#include "tscontext.h"

#include <QDebug>
#include "xmlhelper.h"
#include <libxml/tree.h>
#include <string.h>

TsDoc::TsDoc() {}

TsDoc::~TsDoc()
{
    if (doc) {
        emit sourceChange();
        xmlFreeDoc(doc);
    }
    qDebug() << "cloear document";
}

bool TsDoc::setSource(const QString &path)
{
    if (doc) {
        emit sourceChange();
        xmlFreeDoc(doc);
    }
    doc = xmlParseFile(path.toStdString().c_str());
    if (doc == nullptr)
        return false;

    return true;
}

void TsDoc::save(const QString &path)
{
    Q_UNUSED(path);
    //    xmlSaveFile(path.toStdString().c_str(), doc);
    xmlSaveFormatFileEnc(path.toStdString().c_str(), doc, "UTF-8", 1);
}

std::shared_ptr<TsContext> TsDoc::firstContext() const
{
    auto child = xmlDocGetRootElement(doc);
    child = child->children;
    while (child != nullptr) {
        if (strcmp((const char *) child->name, "context") == 0) {
            std::shared_ptr<TsContext> node = std::shared_ptr<TsContext>(new TsContext(child, this));
            return node;
        }
        child = child->next;
    }
    return nullptr;
}

QList<std::shared_ptr<TsContext> > TsDoc::listContext() const
{
    QList<std::shared_ptr<TsContext> > slist;
    // ts/context
    auto child = xmlDocGetRootElement(doc);
    child = child->children;
    while (child != nullptr) {
        if (strcmp((const char *) child->name, "context") == 0) {
            std::shared_ptr<TsContext> node = std::shared_ptr<TsContext>(new TsContext(child, this));
            slist.append(node);
        }
        child = child->next;
    }
    return slist;
}

static bool findCon(xmlNodePtr node, void *data)
{
    if (strcmp((const char *) node->name, "context") != 0)
        return false;

    const char *name = (const char *) data;
    QString tname = Xmlhelper::getNodeName(node);
    return (tname == name);
}
const std::shared_ptr<TsContext> TsDoc::findContext(const QString &name)
{
    std::shared_ptr<TsContext> ret = nullptr;
    QByteArray qba = name.toLocal8Bit();
    char *fname = qba.data();
    auto child = xmlDocGetRootElement(doc);
    auto node = Xmlhelper::findChild(child, findCon, fname);
    if (node) {
        ret = std::shared_ptr<TsContext>(new TsContext(node, this));
    }
    return ret;
}

void TsDoc::copyFrom(const TsDoc *data)
{
    auto list = data->listContext();
    std::shared_ptr<TsContext> context = nullptr;
    for (auto old_con : list) {
        if (context != nullptr)
            context = context->next();
        QString name = old_con->getName();
        if (context) {
            // so sanh
            if (name == context->getName()) {
                // copy context
                context->copyFrom(old_con.get());
                continue;
            }
        }
        // not match name
        context = this->findContext(name);
        if (context) {
            //copy
            context->copyFrom(old_con.get());
        } else {
            qDebug() << "No context name:" << name;
        }
    }
}

std::shared_ptr<TsDoc> TsDoc::create(const QString &path)
{
    std::shared_ptr<TsDoc> ret = std::make_shared<TsDoc>();
    ret->setSource(path);
    return ret;
}
