#include "finishtest.h"
#include "ui_finishtest.h"

FinishTest::FinishTest(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FinishTest)
{
    ui->setupUi(this);

    connect(ui->okBtn, &QPushButton::clicked, this, &FinishTest::slot_ok);
}

FinishTest::~FinishTest()
{
    delete ui;
}

void FinishTest::setValues(int countTasks, int countRight)
{
    int countErrors = countTasks - countRight;
    ui->countTasksLbl->setText(QString::number(countTasks));
    ui->rightLbl->setText(QString::number(countRight));
    ui->errorsLbl->setText(QString::number(countErrors));
}

void FinishTest::slot_ok()
{
    emit signal_ok();
}
