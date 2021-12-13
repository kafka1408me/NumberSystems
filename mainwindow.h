#pragma once

/*************************************************
 *
 * MainWindow - главный класс приложения. Это виджет,
 * в котором отображаются другие виджеты. Также
 * MainWindow управляет переходами между окнами
 * приложения.
 *
 *************************************************/

#include <QMainWindow>
#include "modetestingwidget.h"  // для использовния ResultTest
#include "userdata.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

// Предварительное объявление классов (forward declaration)
// Для создания указателей на объекты этих классов.
// Здесь можно не подключать заголовочные файлы классов,
// поскольку для создания указателя не требуется знать определение класса
// (но это определение необходимо при создании объекта класса!)
class MenuWidget;
class ModeLearningWidget;
class ModeTranslateWidget;
class StartTest;
class FinishTest;
class StatisticsWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

signals:
    /**
     * @brief signal_unlogin - сигнал, сообщающий о выходе
     * пользователя из системы (вызывается при возвращении
     * пользователя к окну авторизации)
     */
    void signal_unlogin();

public slots:
    /**
     * @brief slot_show - слот для открытия окна виджета
     * @param userData - данные авторизовавшегося пользователя
     */
    void slot_show(const UserData& userData);

private slots:
    /**
     * @brief slot_exit - слот, вызывающий завершение программы
     */
    void slot_exit();

    /**
     * @brief slot_toMenu - слот, вызывающий переход в меню
     */
    void slot_toMenu();

    /**
     * @brief slot_toModeLearning - слот, вызывающий переход в режим обучения
     */
    void slot_toModeLearning();

    /**
     * @brief slot_toModeTranslate - слот, вызывающий переход в режим перевода
     */
    void slot_toModeTranslate();

    /**
     * @brief slot_toModeSelfTest - слот, активирующий режим самотестирования
     */
    void slot_toModeSelfTest();

    /**
     * @brief slot_toModeTestStart - слот, активирующий окно старта режима тестирования
     */
    void slot_toModeTestStart();

    /**
     * @brief slot_toModeTest - слот, открывающий виджет выполнения теста
     * @param countTasks - количество задач
     */
    void slot_toModeTest(int countTasks);

    /**
     * @brief slot_resultOfOneTest - слот, реагирующий на результат
     * выполнения тестировочной задачи
     * @param resultTest - результат выполнения задачи
     */
    void slot_resultOfOneTest(ResultTest resultTest);

    /**
     * @brief slot_finishTest - слот, завершающий тест
     * и открывающий окно результатов тестирования
     */
    void slot_finishTest();

    /**
     * @brief slot_toStatistics - слот, открывающий таблицу статистики
     */
    void slot_toStatistics();

    /**
     * @brief slot_saveUserTime - слот сохранения времени
     * пользователя в текущей сессии
     */
    void slot_saveUserTime();

private:
    Ui::MainWindow *ui;
    MenuWidget* myMenuWidget;
    ModeLearningWidget* modeLearningWidget;
    ModeTestingWidget* modeTestingWidget;
    ModeTranslateWidget* modeTranslateWidget;
    StartTest* startTest;
    FinishTest* finishTest;
    StatisticsWidget* statisticsWidget;
    int countTasks;              // Количество задач для тестирования
    int countTasksRemained;      // Осталось выполнить задач
    int countRightTasks;         // Количество правильно решенных задач
    UserIdType userId;           // Идентификатор авторизованного пользователя
    TimePoint startSessionTime;  // Временная точка начала сессии пользователя
};






