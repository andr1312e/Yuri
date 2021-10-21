QT       += core gui network widgets

CONFIG += c++11

TARGET = "PROTOKOL_C_M14XJI2"
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    messagesetter.cpp \
    messagesgetter.cpp \
    tcpsocket.cpp \
    widget.cpp

HEADERS += \
    constantsandfunctions.h \
    messagesetter.h \
    messagesgetter.h \
    tcpsocket.h \
    widget.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
