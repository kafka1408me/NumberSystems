#include "authorizewidget.h"
#include "ui_authorizewidget.h"
#include "namevalidator.h"
#include "filehandler.h"

const QString badLineEditStyle = "border: 1px solid red;";
const QString defaultLineEditStyle = "border: 1px solid #acd1f5;";

AuthorizeWidget::AuthorizeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AuthorizeWidget)
{
    ui->setupUi(this);

    NameValidator* nameValidator = new NameValidator(this);

    ui->warningLbl->hide();

    ui->nameLineEdit->setStyleSheet(defaultLineEditStyle);
    ui->surnameLineEdit->setStyleSheet(defaultLineEditStyle);

    ui->nameLineEdit->setMaxLength(maxNameLength);
    ui->surnameLineEdit->setMaxLength(maxNameLength);

    ui->nameLineEdit->setValidator(nameValidator);
    ui->surnameLineEdit->setValidator(nameValidator);

    connect(ui->nextBtn, &QPushButton::clicked, this, &AuthorizeWidget::slot_tryAuthorize);

    setFixedSize(424, 220);

    setWindowTitle("Авторизация");
}

AuthorizeWidget::~AuthorizeWidget()
{
    delete ui;
}

void AuthorizeWidget::slot_show()
{
    ui->warningLbl->hide();

    ui->nameLineEdit->clear();
    ui->surnameLineEdit->clear();

    ui->nameLineEdit->setStyleSheet(defaultLineEditStyle);
    ui->surnameLineEdit->setStyleSheet(defaultLineEditStyle);

    show();
}

void AuthorizeWidget::slot_tryAuthorize()
{
    bool isNameGood = ui->nameLineEdit->hasAcceptableInput();
    bool isSurnameGood = ui->surnameLineEdit->hasAcceptableInput();

    if(isNameGood && isSurnameGood)
    {
        QString name = ui->nameLineEdit->text();
        QString surname = ui->surnameLineEdit->text();

        auto user = FileHandler::authorizeUser(name, surname);

        emit signal_authorize(user);
        hide();
    }
    else
    {
        ui->warningLbl->show();

        if(!isNameGood)
        {
            ui->nameLineEdit->setStyleSheet(badLineEditStyle);
        }
        else
        {
            ui->nameLineEdit->setStyleSheet(defaultLineEditStyle);
        }

        if(!isSurnameGood)
        {
            ui->surnameLineEdit->setStyleSheet(badLineEditStyle);
        }
        else
        {
            ui->surnameLineEdit->setStyleSheet(defaultLineEditStyle);
        }
    }
}
