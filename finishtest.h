#pragma once

/*************************************************
 *
 * FinishTest - класс виджета для отображения,
 * используемый для сообщения пользователю в конце
 * теста, сколько заданий всего было, сколько
 * выполнено правильно и сколько ошибок
 *
 *************************************************/

#include <QWidget>

namespace Ui {
class FinishTest;
}

class FinishTest : public QWidget
{
    Q_OBJECT
public:
    explicit FinishTest(QWidget *parent = nullptr);

    ~FinishTest();

    void setTime(quint64 sec);

    /**
     * @brief setValues - функция для установки информации в виджет
     * @param countTasks - количество заданий (всего)
     * @param countRight - количество верно выполненных заданий
     */
    void setValues(int countTasks, int countRight);

signals:
    /**
     * @brief signal_ok - сигнал, сообщающий, что пользователь
     * ознакомился с результатами тестирования
     */
    void signal_ok();

private slots:
    /**
     * @brief slot_ok - слот, вызываемый при клике на конопку
     */
    void slot_ok();

private:
    Ui::FinishTest *ui;
};

