QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QMAKE_LFLAGS_RELEASE += -static -static-libgcc

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Forms/authentication.cpp \
    Forms/registration.cpp \
    Utilities/RSA.cpp \
    Utilities/SHA.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    Forms/authentication.h \
    Forms/registration.h \
    Utilities/RSA.h \
    Utilities/SHA.h \
    Utilities/debug.h \
    mainwindow.h

FORMS += \
    Forms/authentication.ui \
    Forms/registration.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES +=
RC_FILE = resourse.rc

DISTFILES += \
    resourse.rc