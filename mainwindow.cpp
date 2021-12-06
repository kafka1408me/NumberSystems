#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "modelearningwidget.h"
#include "modetestingwidget.h"
#include "modetranslatewidget.h"
#include "menuwidget.h"
#include "starttest.h"
#include "finishtest.h"

// Макросы строк, описывающих текущий режим в приложении
#define STR_MENU             "Меню"
#define STR_MODE_LEARNING    "Режим обучения"
#define STR_MODE_TRANSLATE   "Режим перевода"
#define STR_MODE_TEST        "Режим тестирования"
#define STR_MODE_SELF_TEST   "Режим самотестирования"

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    myMenuWidget(new MenuWidget),
    modeLearningWidget(new ModeLearningWidget),
    modeTestingWidget(new ModeTestingWidget),
    modeTranslateWidget(new ModeTranslateWidget),
    startTest(new StartTest),
    finishTest(new FinishTest),
    countTasks(-1),
    countTasksRemained(-1),
    countRightTasks(-1)
{
    ui->setupUi(this);

    // Добавляем виджеты в стек виджетов
    // (стек виджетов отображает только один из виджетов)
    ui->stackedWidget->addWidget(myMenuWidget);
    ui->stackedWidget->addWidget(modeLearningWidget);
    ui->stackedWidget->addWidget(modeTestingWidget);
    ui->stackedWidget->addWidget(modeTranslateWidget);
    ui->stackedWidget->addWidget(startTest);
    ui->stackedWidget->addWidget(finishTest);

    connect(ui->exitBtn, &QPushButton::clicked, this, &MainWindow::slot_exit);
    connect(ui->menuBtn, &QPushButton::clicked, this, &MainWindow::slot_toMenu);
    connect(myMenuWidget, &MenuWidget::signal_toModeLearning, this, &MainWindow::slot_toModeLearning);
    connect(myMenuWidget, &MenuWidget::signal_toModeTranslate, this, &MainWindow::slot_toModeTranslate);
    connect(myMenuWidget, &MenuWidget::signal_toModeSelfTest, this, &MainWindow::slot_toModeSelfTest);
    connect(myMenuWidget, &MenuWidget::signal_toModeTest, this, &MainWindow::slot_toModeTestStart);

    connect(startTest, &StartTest::signal_startTest, this, &MainWindow::slot_toModeTest);
    connect(finishTest, &FinishTest::signal_ok, this, &MainWindow::slot_toMenu);

    // Открываем меню
    slot_toMenu();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slot_exit()
{
    // Завершение работы программы
    qApp->quit();
}

void MainWindow::slot_toMenu()
{
    ui->modeLbl->setText(STR_MENU);
    ui->menuBtn->hide();

    // Если slot_toMenu вызван из режима тестирования,
    // то нужно завершить корректно режим тестирования
    if(countTasksRemained > 0)
    {
        // Больше не обрабатываем сигнал о результате выполнения задания в режиме тестирования
        disconnect(modeTestingWidget, &ModeTestingWidget::signal_resultTest, this, &MainWindow::slot_resultOfOneTest);
        countTasksRemained = 0;
    }

    ui->stackedWidget->setCurrentWidget(myMenuWidget);
}

void MainWindow::slot_toModeLearning()
{
    ui->modeLbl->setText(STR_MODE_LEARNING);
    ui->menuBtn->show();
    modeLearningWidget->preparePage();
    ui->stackedWidget->setCurrentWidget(modeLearningWidget);
}

void MainWindow::slot_toModeTranslate()
{
    ui->modeLbl->setText(STR_MODE_TRANSLATE);
    ui->menuBtn->show();
    modeTranslateWidget->preparePage();
    ui->stackedWidget->setCurrentWidget(modeTranslateWidget);
}

void MainWindow::slot_toModeSelfTest()
{
    ui->modeLbl->setText(STR_MODE_SELF_TEST);
    ui->menuBtn->show();
    modeTestingWidget->preparePage();
    ui->stackedWidget->setCurrentWidget(modeTestingWidget);
}

void MainWindow::slot_toModeTestStart()
{
    ui->modeLbl->setText(STR_MODE_TEST);
    ui->menuBtn->show();
    ui->stackedWidget->setCurrentWidget(startTest);
}

void MainWindow::slot_toModeTest(int countTasks)
{
    this->countTasks = countTasks;
    countTasksRemained = countTasks;
    countRightTasks = 0;

    // Начинаем обработку сигнала, сообщающего о результате выполнения задания теста
    connect(modeTestingWidget, &ModeTestingWidget::signal_resultTest, this, &MainWindow::slot_resultOfOneTest);
    modeTestingWidget->preparePage();
    ui->stackedWidget->setCurrentWidget(modeTestingWidget);
}

void MainWindow::slot_resultOfOneTest(ResultTest resultTest)
{
    // Если задание решено правильно
    if(resultTest == ResultTest::Right)
    {
        ++countRightTasks;
    }
    // Если все задачи были выполнены
    if(--countTasksRemained == 0)
    {
        // Больше не обрабатываем сигнал о результате выполнения задания в режиме тестирования
        disconnect(modeTestingWidget, &ModeTestingWidget::signal_resultTest, this, &MainWindow::slot_resultOfOneTest);
        countTasksRemained = 0;
        slot_finishTest();  // Открываем окно завершения теста
    }
}

void MainWindow::slot_finishTest()
{
    finishTest->setValues(countTasks, countRightTasks);
    ui->stackedWidget->setCurrentWidget(finishTest);
}

