#-------------------------------------------------
#
# Project created by QtCreator 2016-11-08T23:45:50
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MainWindowExperiments
TEMPLATE = app

SOURCES += main.cpp \
    congratswindow.cpp\
    puzzleform.cpp \
    scoremanager.cpp


HEADERS  += \
    congratswindow.h\
    scoremanager.h \
    puzzleform.h \
    ../build-MainWindowExperiments-Desktop_Qt_5_7_0_MSVC2015_64bit-Debug/ui_puzzleform.h


FORMS    += \
    congratswindow.ui\
    puzzleform.ui


RESOURCES += \
    scoreboard.qrc
