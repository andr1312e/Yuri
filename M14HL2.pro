QT       += core gui network widgets

CONFIG += c++11

TARGET = "PROTOKOL_C_M14XJI2"
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    messageCreators/firmwarecommansmaker.cpp \
    messageCreators/statemessagegetter.cpp \
    messageCreators/statemessagesender.cpp \
    presenter/firmwarepresenter.cpp \
    presenter/statepresenter.cpp \
    tcpsocket.cpp \
    ui/connectionwidget.cpp \
    ui/firmwarewidget.cpp \
    ui/infowidget.cpp \
    ui/statewidget.cpp \
    ui/widgets/firmwaresourceviewer.cpp \
    ui/widgets/sourcecodewidget.cpp \
    widget.cpp

HEADERS += \
    constantsandfunctions.h \
    messageCreators/firmwarecommansmaker.h \
    messageCreators/statemessagegetter.h \
    messageCreators/statemessagesender.h \
    presenter/firmwarepresenter.h \
    presenter/statepresenter.h \
    tcpsocket.h \
    ui/connectionwidget.h \
    ui/firmwarewidget.h \
    ui/infowidget.h \
    ui/statewidget.h \
    ui/widgets/firmwaresourceviewer.h \
    ui/widgets/sourcecodewidget.h \
    widget.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
