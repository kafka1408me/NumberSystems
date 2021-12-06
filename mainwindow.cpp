#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "modelearningwidget.h"
#include "modetestingwidget.h"
#include "modetranslatewidget.h"
#include "menuwidget.h"

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
    modeTranslateWidget(new ModeTranslateWidget)
{
    ui->setupUi(this);

    ui->stackedWidget->addWidget(myMenuWidget);
    ui->stackedWidget->addWidget(modeLearningWidget);
    ui->stackedWidget->addWidget(modeTestingWidget);
    ui->stackedWidget->addWidget(modeTranslateWidget);

    connect(ui->exitBtn, &QPushButton::clicked, this, &MainWindow::slot_exit);
    connect(ui->menuBtn, &QPushButton::clicked, this, &MainWindow::slot_toMenu);
    connect(myMenuWidget, &MenuWidget::signal_toModeLearning, this, &MainWindow::slot_toModeLearning);
    connect(myMenuWidget, &MenuWidget::signal_toModeTranslate, this, &MainWindow::slot_toModeTranslate);
    connect(myMenuWidget, &MenuWidget::signal_toModeTest, this, &MainWindow::slot_toModeTest);
    connect(myMenuWidget, &MenuWidget::signal_toModeSelfTest, this, &MainWindow::slot_toModeSelfTest);

    slot_toMenu();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slot_exit()
{
    qApp->quit();
}

void MainWindow::slot_toMenu()
{
    ui->modeLbl->setText(STR_MENU);
    ui->menuBtn->hide();

    ui->stackedWidget->setCurrentWidget(myMenuWidget);
}

void MainWindow::slot_toModeLearning()
{
    ui->modeLbl->setText(STR_MODE_LEARNING);
    ui->menuBtn->show();
    ui->stackedWidget->setCurrentWidget(modeLearningWidget);
}

void MainWindow::slot_toModeTranslate()
{
    ui->modeLbl->setText(STR_MODE_TRANSLATE);
    ui->menuBtn->show();
    ui->stackedWidget->setCurrentWidget(modeTranslateWidget);
}

void MainWindow::slot_toModeTest()
{
    ui->modeLbl->setText(STR_MODE_TEST);
    ui->menuBtn->show();
    ui->stackedWidget->setCurrentWidget(modeTestingWidget);
}

void MainWindow::slot_toModeSelfTest()
{
    ui->modeLbl->setText(STR_MODE_SELF_TEST);
    ui->menuBtn->show();
    ui->stackedWidget->setCurrentWidget(modeTestingWidget);
}

