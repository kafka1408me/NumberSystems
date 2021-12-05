#include "modetranslatewidget.h"
#include "ui_modetranslatewidget.h"
#include "functions.h"
#include "numbersysteminputvalidator.h"
#include <QDebug>

ModeTranslateWidget::ModeTranslateWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ModeTranslateWidget)
{
    ui->setupUi(this);

    ui->numberSystemFromCmbx->addItems(numberSystems);
    ui->numberSystemToCmbx->addItems(numberSystems);

    setNumberSystems();

    ui->numberLineEdit->setValidator(MyValidator);

    connect(ui->translateBtn, &QPushButton::clicked, this, &ModeTranslateWidget::slot_translate);
 //   connect(ui->numberSystemFromCmbx, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_currentIndexFromChanged(int)));

    connect(ui->numberSystemFromCmbx, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ModeTranslateWidget::slot_currentIndexFromChanged);
}

ModeTranslateWidget::~ModeTranslateWidget()
{
    delete ui;
}

void ModeTranslateWidget::slot_translate()
{
    NumberSystem from = NumberSystem(ui->numberSystemFromCmbx->currentIndex());
    NumberSystem to = NumberSystem(ui->numberSystemToCmbx->currentIndex());
    QString fromStr = ui->numberLineEdit->text();

    QString result = translateNumber(fromStr, from, to);
    ui->resultNumLbl->setText(result);
}

void ModeTranslateWidget::slot_currentIndexFromChanged(int index)
{
    MyValidator->setNumberSystem(NumberSystem(index));
    QString str = ui->numberLineEdit->text();
    int pos = 0;
    QValidator::State state = MyValidator->validate(str, pos);

    if(state == QValidator::State::Invalid)
    {
        MyValidator->fixup(str);
        ui->numberLineEdit->setText(str);
    }
}

void ModeTranslateWidget::setNumberSystems(NumberSystem from, NumberSystem to)
{
    ui->numberSystemFromCmbx->setCurrentIndex(int(from));
    ui->numberSystemToCmbx->setCurrentIndex(int(to));
}
