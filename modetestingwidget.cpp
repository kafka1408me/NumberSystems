#include "modetestingwidget.h"
#include "ui_modetestingwidget.h"
#include "functions.h"
//#include <QDebug>
#include "numbersysteminputvalidator.h"

// Строковые значения цветов
#define COLOR_NUMBER   "#2e5dd0"
#define COLOR_FROM     "#e28e3c"
#define COLOR_TO       "#519e28"
#define COLOR_RIGHT    "#459f09"
#define COLOR_ERROR    "#ff4f0a"
#define COLOR_WARNING  "#d27c00"


// Строка-предупреждение, если пользователь нажал на "Далее", но не ввел ответ
#define WARNING_SKIP_STR COLOR_STR(COLOR_WARNING, "Вы не ответили на вопрос. Нажмите далее для перехода к следующему заданию или напишите ответ.")

ModeTestingWidget::ModeTestingWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ModeTestingWidget),
    answer("")
{
    // Регистрируем перечисление, чтобы использовать его значения
    // для передачи в сигналах
    qRegisterMetaType<ResultTest>("ResultTest");

    ui->setupUi(this);

    ui->minNumSpinBox->setMaximum(ui->maxNumSpinBox->value() - 1);

    ui->numberLineEdit->setValidator(MyValidator);

    connect(ui->maxNumSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &ModeTestingWidget::slot_maxValueChanged);
    connect(ui->nextBtn, &QPushButton::clicked, this, &ModeTestingWidget::slot_nextTest);
}

ModeTestingWidget::~ModeTestingWidget()
{
    delete ui;
}

void ModeTestingWidget::preparePage()
{
    timeTestStart = getCurrentTime();
    next();
}

quint64 ModeTestingWidget::getTestTimeSec() const
{
    return getDiffTime(timeTestStart);
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

    // Если поясняющая строка не установлена или установлена строка-предупреждение
    if(resultStr.isEmpty() || isWarningSkip)
    {
        QString inputStr = ui->numberLineEdit->text();  // Ответ пользователя

        if(inputStr.isEmpty()) // Если пользователь ничего не ввел
        {
            // Установлена строка-предупреждение
            if(isWarningSkip)
            {
                next();  // Генерируем новое задание
                emit signal_resultTest(ResultTest::Skip);
                return;
            }
            resultStr = WARNING_SKIP_STR;
        }
        else if(inputStr == answer)  // Пользователь ввел правильный ответ
        {
            resultStr = COLOR_STR(COLOR_RIGHT, "Правильно! Нажмите \"Далее\" для продолжения.");
            emit signal_resultTest(ResultTest::Right);
        }
        else  // Пользователь ввел неверный ответ
        {
            resultStr = COLOR_STR(COLOR_ERROR, "Ошибка! Верный ответ: %1. Нажмите \"Далее\" для продолжения.");
            resultStr = resultStr.arg(answer);
            emit signal_resultTest(ResultTest::Error);
        }

        ui->resultLbl->setText(resultStr);
    }
    else  // Установлена поясняющая строка, но не строка-предупреждение
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

 //   qDebug() << val << " " << valueOfNumberSystem(fromNumberSystem) << " " << valueOfNumberSystem(toNumberSystem) << " " << answer;
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
