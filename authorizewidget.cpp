#include "authorizewidget.h"
#include "ui_authorizewidget.h"

AuthorizeWidget::AuthorizeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AuthorizeWidget)
{
    ui->setupUi(this);

    ui->nameLineEdit->setMaxLength(maxNameLength);
    ui->surnameLineEdit->setMaxLength(maxNameLength);

    setWindowTitle("Авторизация");
}

AuthorizeWidget::~AuthorizeWidget()
{
    delete ui;
}

void AuthorizeWidget::slot_show()
{
    ui->nameLineEdit->clear();
    ui->surnameLineEdit->clear();
    show();
}
