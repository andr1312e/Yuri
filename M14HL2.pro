QT       += core network widgets serialport

CONFIG += c++latest
VERSION = 3.3.1
DEFINES += APP_VERSION=\\\"$$VERSION\\\"
TARGET = "YUstirovojchnyj_komplekt"
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
CONFIG += no_keywords
SOURCES += \
    connections/datahandler.cpp \
    main.cpp \
    mainwindow.cpp \
    messageCreators/firmwarecommansmaker.cpp \
    messageCreators/statemessagegetter.cpp \
    messageCreators/statemessagesender.cpp \
    presenter/firmwarepresenter.cpp \
    presenter/statepresenter.cpp \
    connections\serialhandler.cpp \
    connections\tcphandler.cpp \
    ui/connectionwidget.cpp \
    ui/firmwareflasherwidget.cpp \
    ui/firmwarelogwidget.cpp \
    ui/firmwarewidget.cpp \
    ui/infowidget.cpp \
    ui/statewidget.cpp

HEADERS += \
    connections/datahandler.h \
    constantsandfunctions.h \
    mainwindow.h \
    messageCreators/firmwarecommansmaker.h \
    messageCreators/statemessagegetter.h \
    messageCreators/statemessagesender.h \
    presenter/firmwarepresenter.h \
    presenter/statepresenter.h \
    connections\serialhandler.h \
    connections\tcphandler.h \
    ui/connectionwidget.h \
    ui/firmwareflasherwidget.h \
    ui/firmwarelogwidget.h \
    ui/firmwarewidget.h \
    ui/infowidget.h \
    ui/statewidget.h

# Default rules for deployment.
#qnx: target.path = /tmp/$${TARGET}/bin
#else: unix:!android: target.path = /opt/$${TARGET}/bin
#!isEmpty(target.path): INSTALLS += target

#DISTFILES +=
