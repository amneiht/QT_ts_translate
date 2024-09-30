#include "tscontext.h"
#include <QDebug>
#include "tsdoc.h"
#include "tsmessage.h"
#include "xmlhelper.h"

TsContext::TsContext(xmlNodePtr node, const TsDoc *doc)
{
    this->node = node;
    if (doc) {
        QObject::connect(doc, &TsDoc::sourceChange, this, &TsContext::onSourceChanged);
        par = doc;
    }
}

TsContext::~TsContext()
{
    emit this->sourceChange();
}

QString TsContext::getName() const
{
    if (node)
        return Xmlhelper::getNodeName(node);
    return "";
}

std::shared_ptr<TsMessage> TsContext::firstMessage() const
{
    if (node) {
        auto slist = Xmlhelper::getChildListWithName(node, "message");
        for (auto ptr : slist) {
            auto mess = std::make_shared<TsMessage>(ptr, this->par);
            return mess;
        }
    }
    return nullptr;
}

QList<std::shared_ptr<TsMessage> > TsContext::getMessageList() const
{
    QList<std::shared_ptr<TsMessage> > ret;
    if (node) {
        auto slist = Xmlhelper::getChildListWithName(node, "message");
        for (auto ptr : slist) {
            auto mess = std::make_shared<TsMessage>(ptr, this->par);
            ret.append(mess);
        }
    }
    return ret;
}

static bool _findMess(xmlNodePtr node, void *data)
{
    const char *name = (const char *) data;
    TsMessage mess(node);
    return (mess.getSource() == name);
}

std::shared_ptr<TsMessage> TsContext::findMess(const QString &name) const
{
    std::shared_ptr<TsMessage> ret = nullptr;
    QByteArray qba = name.toLocal8Bit();
    char *fname = qba.data();
    auto mnode = Xmlhelper::findChild(this->node, _findMess, fname);
    if (mnode) {
        ret = std::make_shared<TsMessage>(mnode, this->par);
    }
    return ret;
}

std::shared_ptr<TsContext> TsContext::next() const
{
    std::shared_ptr<TsContext> ret = nullptr;
    auto nptr = node->next;
    while (nptr != nullptr) {
        if (strcmp((const char *) nptr->name, "context") == 0) {
            ret = std::make_shared<TsContext>(nptr, this->par);
            return ret;
        }
        nptr = nptr->next;
    }
    return nullptr;
}
static void copyMess(TsMessage *mess, const TsMessage *from)
{
    // check typw
    if (mess->getType() != from->getType()) {
        qDebug() << "Type not match in" << mess->getSource();
        return;
    }
    auto p1 = mess->getContent();
    auto p2 = from->getContent();
    if (mess->getType() == TsMessage::String) {
        auto m1 = std::static_pointer_cast<TsString>(p1);
        auto m2 = std::static_pointer_cast<TsString>(p2);
        m1->setContent(m2->getContent());

    } else {
        std::shared_ptr<TsNumerus> m1 = std::static_pointer_cast<TsNumerus>(p1);
        std::shared_ptr<TsNumerus> m2 = std::static_pointer_cast<TsNumerus>(p2);
        m1->setNumerus(m2->getNumerus());
        m1->setBase(m2->getBase());
    }
    mess->setFinished(true);
}
void TsContext::copyFrom(const TsContext *context)
{
    auto clist = context->getMessageList();
    std::shared_ptr<TsMessage> ctl = nullptr;
    for (auto bctx : clist) {
        if (ctl)
            ctl = ctl->next();
        QString source = bctx->getSource();
        if (ctl) {
            if (source == ctl->getSource()) {
                // copy mess
                copyMess(ctl.get(), bctx.get());
                continue;
            }
        }
        ctl = this->findMess(source);
        if (ctl) {
            //copy mess
            copyMess(ctl.get(), bctx.get());
        } else {
            qDebug() << "No message:" << source;
        }
    }
}

void TsContext::onSourceChanged()
{
    if (par) {
        QObject::disconnect(par, &TsDoc::sourceChange, this, &TsContext::onSourceChanged);
    }
    emit this->sourceChange();
    par = nullptr;
    node = nullptr;
    this->deleteLater();
}
