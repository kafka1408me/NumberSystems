#pragma once

#include <QMainWindow>
#include "modetestingwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

// Предварительное объявление классов (forward declaration)
class MenuWidget;
class ModeLearningWidget;
class ModeTranslateWidget;
class StartTest;
class FinishTest;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void slot_exit();

    void slot_toMenu();

    void slot_toModeLearning();

    void slot_toModeTranslate();

    void slot_toModeSelfTest();

    void slot_toModeTestStart();

    void slot_toModeTest(int countTasks);

    void slot_resultOfOneTest(ResultTest resultTest);

    void slot_finishTest();

private:
    Ui::MainWindow *ui;
    MenuWidget* myMenuWidget;
    ModeLearningWidget* modeLearningWidget;
    ModeTestingWidget* modeTestingWidget;
    ModeTranslateWidget* modeTranslateWidget;
    StartTest* startTest;
    FinishTest* finishTest;
    int countTasks;
    int countTasksRemained;
    int countRightTasks;
};

