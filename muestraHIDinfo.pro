#-------------------------------------------------
#
# Project created by QtCreator 2014-11-13T19:46:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = muestraHIDinfo
TEMPLATE = app

INCLUDEPATH += -I/usr/include/libusb-1.0
unix:!macx: LIBS += -lusb-1.0
#LIBS += -lusb-1.0

SOURCES += main.cpp\
        mostrador.cpp \
    gestorhid.cpp \
    HIDAPI/hid.c

HEADERS  += mostrador.h \
    gestorhid.h

FORMS    += mostrador.ui

RESOURCES += \
    HIDAPI.qrc

#unix:!macx: LIBS += -lusb
#unix:!macx: LIBS += -lusb-1.0



