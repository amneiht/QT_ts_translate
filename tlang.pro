QT += core network

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#debug {
#    CONFIG+=sanitizer
#    CONFIG+=sanitize_address
#}


LIBS += -lxml2
INCLUDEPATH += /usr/include/libxml2 ./src
SOURCES += \
        main.cpp \
        src/gooletrans/doctran.cpp \
        src/gooletrans/translate.cpp \
        src/tran/tsType.cpp \
        src/tran/tscontext.cpp \
        src/tran/tsdoc.cpp \
        src/tran/tsmessage.cpp \
        src/tran/xmlhelper.cpp

# RESOURCES += qml.qrc


TRANSLATIONS += assets/lang/en.ts

OTHER_FILES += assets/lang/en.ts
# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    src/gooletrans/doctran.h \
    src/gooletrans/translate.h \
    src/tran/ts.h \
    src/tran/tscontext.h \
    src/tran/tsdoc.h \
    src/tran/tsmessage.h \
    src/tran/xmlhelper.h
