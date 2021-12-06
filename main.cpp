#include <QApplication>
#include <QDebug>
#include "mainwindow.h"
#include "window.h"
#include "modetranslatewidget.h"
#include "modetestingwidget.h"
#include "functions.h"
#include "modelearning.h"
#include <QTimer>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ModeLearning w;
    w.show();

//    QTimer::singleShot(200, [&](){ w.drawTranslateAlgorithm("10", NumberSystem::Eight, NumberSystem::Ten);});;
    qDebug() << translateNumber("3", NumberSystem::Two, NumberSystem::Ten);

    return a.exec();
}
