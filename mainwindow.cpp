#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "modelearningwidget.h"
#include "modetestingwidget.h"
#include "modetranslatewidget.h"
#include "menuwidget.h"
#include "starttest.h"
#include "finishtest.h"
#include "filehandler.h"
#include "statisticswidget.h"
#include <QDebug>

// Макросы строк, описывающих текущий режим в приложении
#define STR_MENU             "Меню"
#define STR_MODE_LEARNING    "Режим обучения"
#define STR_MODE_TRANSLATE   "Режим перевода"
#define STR_MODE_TEST        "Режим тестирования"
#define STR_MODE_SELF_TEST   "Режим самотестирования"
#define STR_MODE_STATISTICS  "Статистика пользователей"

const QString menuText = "Меню";
const QString anotherUserText = "Другой пользователь";

const QString userFullNameColor = "#228af2";

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    myMenuWidget(new MenuWidget),
    modeLearningWidget(new ModeLearningWidget),
    modeTestingWidget(new ModeTestingWidget),
    modeTranslateWidget(new ModeTranslateWidget),
    startTest(new StartTest),
    finishTest(new FinishTest),
    statisticsWidget(new StatisticsWidget),
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
    ui->stackedWidget->addWidget(statisticsWidget);

    connect(ui->exitBtn, &QPushButton::clicked, this, &MainWindow::slot_exit);
    connect(ui->menuBtn, &QPushButton::clicked, this, &MainWindow::slot_toMenu);
    connect(myMenuWidget, &MenuWidget::signal_toModeLearning, this, &MainWindow::slot_toModeLearning);
    connect(myMenuWidget, &MenuWidget::signal_toModeTranslate, this, &MainWindow::slot_toModeTranslate);
    connect(myMenuWidget, &MenuWidget::signal_toModeSelfTest, this, &MainWindow::slot_toModeSelfTest);
    connect(myMenuWidget, &MenuWidget::signal_toModeTest, this, &MainWindow::slot_toModeTestStart);
    connect(myMenuWidget, &MenuWidget::signal_toStatistics, this, &MainWindow::slot_toStatistics);

    connect(startTest, &StartTest::signal_startTest, this, &MainWindow::slot_toModeTest);
    connect(finishTest, &FinishTest::signal_ok, this, &MainWindow::slot_toMenu);

    // Соединяем сигнал завершения приложения со слотом сохранения времени сессии пользователя
    connect(qApp, &QCoreApplication::aboutToQuit, this, &MainWindow::slot_saveUserTime);

    setWindowTitle("Системы счисления");

    // Открываем меню
    slot_toMenu();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slot_show(const UserData &userData)
{
    userId = userData.userId;
    QString userFullName = userData.getFullName();
    ui->userNameLbl->setText(COLOR_STR_BOLD("#042d55", "Пользователь: ") + addColorToString(userFullNameColor, userFullName));

    startSessionTime = getCurrentTime();

    show();
}

void MainWindow::slot_exit()
{
    // Завершение работы программы
    qApp->quit();
}

void MainWindow::slot_toMenu()
{
    if(ui->menuBtn->text() == anotherUserText)
    {
        slot_saveUserTime();
        emit signal_unlogin();
        hide();
        return;
    }

    ui->modeLbl->setText(STR_MENU);

    ui->menuBtn->setText(anotherUserText);

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
    ui->menuBtn->setText(menuText);
    modeLearningWidget->preparePage();
    ui->stackedWidget->setCurrentWidget(modeLearningWidget);
}

void MainWindow::slot_toModeTranslate()
{
    ui->modeLbl->setText(STR_MODE_TRANSLATE);
    ui->menuBtn->setText(menuText);
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
    ui->menuBtn->setText(menuText);
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
    // Сохраняем в файл информацию о завершенных заданиях
    FileHandler::addTests(userId, countTasks, countRightTasks);
    // Получаем время выполнения теста
    auto timeTestSec = modeTestingWidget->getTestTimeSec();
    // Устанавливаем время в виджет завершения теста
    finishTest->setTime(timeTestSec);
    ui->stackedWidget->setCurrentWidget(finishTest);
}

void MainWindow::slot_toStatistics()
{
    ui->menuBtn->setText(menuText);
    slot_saveUserTime();
    statisticsWidget->preparePage();
    ui->stackedWidget->setCurrentWidget(statisticsWidget);
}

void MainWindow::slot_saveUserTime()
{
    // Сохраняем время в том случае, когда виджет MainWindow
    // виден (он не виден, когда виден виджет авторизации)
    if(this->isVisible())
    {
        quint64 sec = getDiffTime(startSessionTime);

        FileHandler::addTime(userId, sec);

        startSessionTime = getCurrentTime();
    }
}

