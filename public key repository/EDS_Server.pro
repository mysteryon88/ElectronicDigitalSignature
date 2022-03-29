QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

QMAKE_LFLAGS_RELEASE += -static -static-libgcc

QT += network

QT += sql

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        database.cpp \
        main.cpp \
        server.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    database.h \
    server.h

RESOURCES +=
RC_FILE = resourse.rc

DISTFILES += \
    resourse.rc
