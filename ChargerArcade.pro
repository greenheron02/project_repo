#-------------------------------------------------
#
# Project created by QtCreator 2024-09-26T13:28:32
#
#-------------------------------------------------

QT       += core gui network
LIBS += -LC:\Users\wesri\Downloads\SDL2-devel-2.30.10-mingw\SDL2-2.30.10\x86_64-w64-mingw32\lib -lSDL2
INCLUDEPATH += C:\Users\wesri\Downloads\SDL2-devel-2.30.10-mingw\SDL2-2.30.10\x86_64-w64-mingw32\include
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ChargerArcade
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        comm.cpp \
        gamepad.cpp \
        main.cpp \
        screen.cpp

HEADERS += \
        gamepad.h \
        screen.h

FORMS += \
        screen.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    screen.qrc

DISTFILES += \
    testfile.txt
