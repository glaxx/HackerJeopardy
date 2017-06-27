#-------------------------------------------------
#
# Project created by QtCreator 2015-07-20T19:25:17
#
#-------------------------------------------------

QT       += core gui multimedia multimediawidgets serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HJeopardy
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    addeditquestiondialog.cpp \
    playscreen.cpp \
    category.cpp \
    question.cpp \
    buttonhelper.cpp \
    serialsettingsdialog.cpp \
    playerhelper.cpp \
    questiontreemodel.cpp

HEADERS  += mainwindow.h \
    addeditquestiondialog.h \
    playscreen.h \
    category.h \
    question.h \
    buttonhelper.h \
    serialsettingsdialog.h \
    playerhelper.h \
    questiontreemodel.h

FORMS    += mainwindow.ui \
    addeditquestiondialog.ui \
    playscreen.ui \
    serialsettingsdialog.ui

DISTFILES += \
    ideas

RESOURCES += \
    resources.qrc
