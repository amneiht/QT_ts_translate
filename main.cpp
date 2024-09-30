#include <QDebug>
#include <QGuiApplication>
#include <QString>
#include <QTranslator>
#include "gooletrans/doctran.h"
#include "tran/ts.h"
#include <memory.h>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
#if 1
    // tao docment voi ts file da co
    auto tm = TsDoc::create(
        "/home/amneiht/app/workspace/code/qt/linphone/linphone-app_5.2.2/assets/languages/en.ts");

    qDebug() << "Load data complete";
    DocTran doc;

    // dich sang tieng viet
    doc.translateDoc(tm, "/tmp/test1.xml");
#endif
    return app.exec();
}
