QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#QMAKE_LFLAGS_RELEASE += -static -static-libgcc

QT += network

#Qt6 for QTextCodec
#QT += core5compat

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Forms/pass.cpp \
    Forms/registration.cpp \
    Utilities/RC4.cpp \
    Utilities/RSA.cpp \
    Utilities/SHA.cpp \
    Utilities/client.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    Forms/pass.h \
    Forms/registration.h \
    Utilities/RC4.h \
    Utilities/RSA.h \
    Utilities/SHA.h \
    Utilities/client.h \
    Utilities/debug.h \
    mainwindow.h

FORMS += \
    Forms/pass.ui \
    Forms/registration.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES +=
RC_FILE = resourse.rc

DISTFILES += \
    Utilities/main.ico \
    resourse.rc
