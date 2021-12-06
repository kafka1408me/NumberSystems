QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    finishtest.cpp \
    functions.cpp \
    main.cpp \
    mainwindow.cpp \
    menuwidget.cpp \
    modelearningwidget.cpp \
    modetestingwidget.cpp \
    modetranslatewidget.cpp \
    numbersysteminputvalidator.cpp \
    preparerpage.cpp \
    starttest.cpp \
    window.cpp

HEADERS += \
    NumberParameters.h \
    finishtest.h \
    functions.h \
    mainwindow.h \
    menuwidget.h \
    modelearningwidget.h \
    modetestingwidget.h \
    modetranslatewidget.h \
    numbersysteminputvalidator.h \
    preparerpage.h \
    starttest.h \
    window.h

FORMS += \
    finishtest.ui \
    mainwindow.ui \
    menuwidget.ui \
    modelearningwidget.ui \
    modetestingwidget.ui \
    modetranslatewidget.ui \
    starttest.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
