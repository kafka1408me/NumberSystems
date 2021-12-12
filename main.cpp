#include <QApplication>
#include "mainwindow.h"
#include "authorizewidget.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;

    AuthorizeWidget auth;
    auth.show();

    return a.exec();
}
