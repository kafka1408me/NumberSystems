#include "starttest.h"
#include "ui_starttest.h"

StartTest::StartTest(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StartTest)
{
    ui->setupUi(this);

    connect(ui->startTestBtn, &QPushButton::clicked, this, &StartTest::slot_startTest);
}

StartTest::~StartTest()
{
    delete ui;
}

void StartTest::slot_startTest()
{
    int countTasks = ui->countTasksSpinBox->value();
    emit signal_startTest(countTasks);
}
