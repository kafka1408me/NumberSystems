#include "finishtest.h"
#include "ui_finishtest.h"
#include "functions.h"

FinishTest::FinishTest(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FinishTest)
{
    ui->setupUi(this);

    // Соединяем сигнал клика от кнопки со слотом-обработчиком
    connect(ui->okBtn, &QPushButton::clicked, this, &FinishTest::slot_ok);
}

FinishTest::~FinishTest()
{
    delete ui;
}

void FinishTest::setTime(quint64 sec)
{
    QString timeStr = getTimeStrFromSec(sec);
    ui->timeLbl->setText(timeStr);
}

void FinishTest::setValues(int countTasks, int countRight)
{
    int countErrors = countTasks - countRight;  // Количество ошибок
    ui->countTasksLbl->setText(QString::number(countTasks));
    ui->rightLbl->setText(QString::number(countRight));
    ui->errorsLbl->setText(QString::number(countErrors));
}

void FinishTest::slot_ok()
{
    emit signal_ok();
}
