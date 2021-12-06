#pragma once

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MenuWidget;
class ModeLearningWidget;
class ModeTestingWidget;
class ModeTranslateWidget;

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

    void slot_toModeTest();

    void slot_toModeSelfTest();

private:
    Ui::MainWindow *ui;
    MenuWidget* myMenuWidget;
    ModeLearningWidget* modeLearningWidget;
    ModeTestingWidget* modeTestingWidget;
    ModeTranslateWidget* modeTranslateWidget;
};

