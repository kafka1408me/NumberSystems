#include <QApplication>
#include "mainwindow.h"
#include "authorizewidget.h"
#include "filehandler.h"
#include <QDebug>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    AuthorizeWidget auth;

    // При успешной авторизации открываем главное окно приложения
    QObject::connect(&auth, &AuthorizeWidget::signal_authorize, &w, &MainWindow::slot_show);
    // При разлогине пользователя открываем окно авторизации
    QObject::connect(&w, &MainWindow::signal_unlogin, &auth, &AuthorizeWidget::slot_show);

    auth.show();

    return a.exec();
}
