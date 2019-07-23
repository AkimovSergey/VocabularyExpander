#-------------------------------------------------
#
# Project created by QtCreator 2019-02-12T04:28:48
#
#-------------------------------------------------
CONFIG += c++17
QT     += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VocabularyExpander
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
    coach.cpp \
    main.cpp \
    trainerwindow.cpp \
    word.cpp \
    trayicon.cpp \
    globals.cpp \
    deliveryboy.cpp \
    settings.cpp \
    dictionary.cpp \
    player.cpp \
    newword.cpp \
    addnewword.cpp

HEADERS += \
    coach.h \
    trainerwindow.h \
    word.h \
    trayicon.h \
    globals.h \
    deliveryboy.h \
    settings.h \
    dictionary.h \
    player.h \
    newword.h \
    addnewword.h

FORMS += \
    trainerwindow.ui \
    dictionary.ui \
    settings.ui \
    newword.ui \
    addnewword.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

LIBS += \
    $$PWD\external_libs\curl\lib/libcurl.lib \
    -lws2_32 \
    -ladvapi32 \
    -lcrypt32 \
    -lwldap32 \
    -lnormaliz
