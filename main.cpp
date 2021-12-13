#include <QApplication>
#include "mainwindow.h"
#include "authorizewidget.h"
#include "filehandler.h"
#include <QDebug>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    FileHandler::authorizeUser("Игорь", "Гайденко");
//    auto kafka = FileHandler::authorizeUser("Франц", "Стрикленд");

//    FileHandler::addTime(kafka.userId, 60);
//    FileHandler::addTime(kafka.userId, 100);

//    FileHandler::addTests(kafka.userId, 10, 3);

//    kafka = FileHandler::authorizeUser("Франц", "Стрикленд");
//    qDebug() << "time = " << kafka.secInApp;
//    qDebug() << "countTests = " << kafka.countTests;
//    qDebug() << "rightTests = " << kafka.countRightTasks;

    MainWindow w;
    AuthorizeWidget auth;

    QObject::connect(&auth, &AuthorizeWidget::signal_authorize, &w, &MainWindow::slot_show);
    QObject::connect(&w, &MainWindow::signal_unlogin, &auth, &AuthorizeWidget::slot_show);

    auth.show();

    return a.exec();
}
