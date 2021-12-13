#include "authorizewidget.h"
#include "ui_authorizewidget.h"
#include "namevalidator.h"
#include "filehandler.h"

// Стиль поля ввода, сообщающий об ошибке
const QString badLineEditStyle = "border: 1px solid red;";
// Стиль по умолчанию поля ввода
const QString defaultLineEditStyle = "border: 1px solid #acd1f5;";

AuthorizeWidget::AuthorizeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AuthorizeWidget)
{
    ui->setupUi(this);

    // Создаем объект валидатора для валидации ввода имени и фамилии
    NameValidator* nameValidator = new NameValidator(this);

    // Скрываем надпись-предупреждение
    ui->warningLbl->hide();

    ui->nameLineEdit->setStyleSheet(defaultLineEditStyle);
    ui->surnameLineEdit->setStyleSheet(defaultLineEditStyle);

    // Устанавливаем максимальную ширину ввода
    ui->nameLineEdit->setMaxLength(maxNameLength);
    ui->surnameLineEdit->setMaxLength(maxNameLength);

    ui->nameLineEdit->setValidator(nameValidator);
    ui->surnameLineEdit->setValidator(nameValidator);

    connect(ui->nextBtn, &QPushButton::clicked, this, &AuthorizeWidget::slot_tryAuthorize);

    // Устанавливаем фиксированный размер виджета
    setFixedSize(424, 220);

    setWindowTitle("Авторизация");
}

AuthorizeWidget::~AuthorizeWidget()
{
    delete ui;
}

void AuthorizeWidget::slot_show()
{
    // Скрываем надпись-предупреждение
    ui->warningLbl->hide();

    // Очищаем поля ввода
    ui->nameLineEdit->clear();
    ui->surnameLineEdit->clear();

    ui->nameLineEdit->setStyleSheet(defaultLineEditStyle);
    ui->surnameLineEdit->setStyleSheet(defaultLineEditStyle);

    show();
}

void AuthorizeWidget::slot_tryAuthorize()
{
    // hasAcceptableInput возвр. true, если введенный текст
    // удовлетворяет валидации
    bool isNameGood = ui->nameLineEdit->hasAcceptableInput();
    bool isSurnameGood = ui->surnameLineEdit->hasAcceptableInput();

    // Если и имя, и фамилия допустимые
    if(isNameGood && isSurnameGood)
    {
        QString name = ui->nameLineEdit->text();
        QString surname = ui->surnameLineEdit->text();

        auto user = FileHandler::authorizeUser(name, surname);

        emit signal_authorize(user);
        // Скрываем окно
        hide();
    }
    else
    {
        // Показываем надпись-предупреждение
        ui->warningLbl->show();

        // Устанавливаем для полей ввода "плохой" стиль,
        // если текст в них не удовлетворяет валидации

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
