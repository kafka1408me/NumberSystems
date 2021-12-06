#pragma once

/*************************************************
 *
 * StartTest - класс виджета, отображающий
 * информацию перед началом тестирования
 *
 *************************************************/

#include <QWidget>

namespace Ui {
class StartTest;
}

class StartTest : public QWidget
{
    Q_OBJECT
public:
    explicit StartTest(QWidget *parent = nullptr);

    ~StartTest();

signals:
    /**
     * @brief signal_startTest - сигнал, сообщающий, сколько задач
     * будет в тесте. Реакция на этот сигнал - начало теста.
     * @param countTasks - количество задач
     */
    void signal_startTest(int countTasks);

private slots:
    /**
     * @brief slot_startTest - слот, вызываемый, когда пользователь
     * нажал на кнопку "Начать тест"
     */
    void slot_startTest();

private:
    Ui::StartTest *ui;
};


