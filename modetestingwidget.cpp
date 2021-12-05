#include "modetestingwidget.h"
#include "ui_modetestingwidget.h"
#include "functions.h"
#include <QDebug>
#include "numbersysteminputvalidator.h"

#define COLOR_NUMBER   "#2e5dd0"
#define COLOR_FROM     "#e28e3c"
#define COLOR_TO       "#519e28"
#define COLOR_RIGHT    "#459f09"
#define COLOR_ERROR    "#ff4f0a"
#define COLOR_WARNING  "#d27c00"

#define COLOR_STR(color, text) "<font color=\"" color "\">" text "</font>"
#define COLOR_STR_BOLD(color, text) COLOR_STR(color, "<b>" text "</b>")

#define WARNING_SKIP_STR COLOR_STR(COLOR_WARNING, "Вы не ответили на вопрос. Нажмите далее для перехода к следующему заданию или напишите ответ.")

ModeTestingWidget::ModeTestingWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ModeTestingWidget),
    answer("")
{
    qRegisterMetaType<ResultTest>("ResultTest");

    ui->setupUi(this);

    ui->minNumSpinBox->setMaximum(ui->maxNumSpinBox->value() - 1);

    ui->numberLineEdit->setValidator(MyValidator);

    connect(ui->minNumSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &ModeTestingWidget::slot_minValueChanged);
    connect(ui->maxNumSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &ModeTestingWidget::slot_maxValueChanged);
    connect(ui->nextBtn, &QPushButton::clicked, this, &ModeTestingWidget::slot_nextTest);

    next();
}

ModeTestingWidget::~ModeTestingWidget()
{
    delete ui;
}

void ModeTestingWidget::slot_minValueChanged(int value)
{
    Q_UNUSED(value);
}

void ModeTestingWidget::slot_maxValueChanged(int value)
{
    int minNumValue = ui->minNumSpinBox->value();
    if(minNumValue >= value)
    {
        ui->minNumSpinBox->setValue(0);
    }
    ui->minNumSpinBox->setMaximum(value - 1);
}

void ModeTestingWidget::slot_nextTest()
{
    QString resultStr = ui->resultLbl->text();

    bool isWarningSkip = resultStr == WARNING_SKIP_STR;

    if(resultStr.isEmpty() || isWarningSkip)
    {
        QString inputStr = ui->numberLineEdit->text();

        if(inputStr.isEmpty())
        {
            if(isWarningSkip)
            {
                next();
                emit signal_resultTest(ResultTest::Skip);
                return;
            }
            resultStr = WARNING_SKIP_STR;
        }
        else if(inputStr == answer)
        {
            resultStr = COLOR_STR(COLOR_RIGHT, "Правильно! Нажмите \"Далее\" для продолжения.");
            emit signal_resultTest(ResultTest::Right);
        }
        else
        {
            resultStr = COLOR_STR(COLOR_ERROR, "Ошибка! Верный ответ: %1. Нажмите \"Далее\" для продолжения.");
            resultStr = resultStr.arg(answer);
            emit signal_resultTest(ResultTest::Error);
        }

        ui->resultLbl->setText(resultStr);
    }
    else
    {
        next();
    }
}

void ModeTestingWidget::next()
{
    int minValue = ui->minNumSpinBox->value();
    int maxValue = ui->maxNumSpinBox->value();
    auto val = generateRandomNumber(minValue, maxValue);
    NumberSystem fromNumberSystem = generateRandomNumberSystem();
    NumberSystem toNumberSystem = generateRandomNumberSystem(fromNumberSystem);

    setTaskText(val, fromNumberSystem, toNumberSystem);

    ui->numberLineEdit->clear();
    MyValidator->setNumberSystem(toNumberSystem);

    QString valStr = QString::number(val, valueOfNumberSystem(fromNumberSystem));
    answer = translateNumber(valStr, fromNumberSystem, toNumberSystem);

    ui->resultLbl->clear();

    qDebug() << val << " " << valueOfNumberSystem(fromNumberSystem) << " " << valueOfNumberSystem(toNumberSystem) << " " << answer;
}

void ModeTestingWidget::setTaskText(unsigned value, NumberSystem fromNumberSystem, NumberSystem toNumberSystem)
{
    int from = valueOfNumberSystem(fromNumberSystem);
    int to = valueOfNumberSystem(toNumberSystem);

    QString valStr = QString::number(value, from);

    QString text = QString("Переведите число " COLOR_STR_BOLD(COLOR_NUMBER, "%1")
                   " из " COLOR_STR_BOLD(COLOR_FROM, "%2") "-ой системы счисления"
                   " в " COLOR_STR_BOLD(COLOR_TO, "%3") "-ую систему счисления").
            arg(valStr, QString::number(from), QString::number(to));

    ui->taskLbl->setText(text);
}
