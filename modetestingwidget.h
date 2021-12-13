#pragma once

/*************************************************
 *
 * ModeTestingWidget - класс виджета, реализующий
 * режим тестирования и самотестирования
 *
 *************************************************/

#include <QWidget>
#include "functions.h"
#include "preparerpage.h"

#ifndef COLOR_STR
// Макрос для добавления к строке цвета
#define COLOR_STR(color, text) "<font color=\"" color "\">" text "</font>"
#endif

#ifndef COLOR_STR_BOLD
// Макрос, делающий строку жирной
#define COLOR_STR_BOLD(color, text) COLOR_STR(color, "<b>" text "</b>")
#endif


namespace Ui {
class ModeTestingWidget;
}

/**
 * @brief The ResultTest enum - перечисление результата выполнения задания на перевод
 */
enum ResultTest
{
    Right = 0,     // Правильно
    Skip  = 1,     // Пользователь пропустил задание
    Error = 2      // Ошибка
};

class ModeTestingWidget : public QWidget, public PreparerPage
{
    Q_OBJECT
public:
    explicit ModeTestingWidget(QWidget *parent = nullptr);

    ~ModeTestingWidget();

    void preparePage() override;

    quint64 getTestTimeSec() const;

signals:
    /**
     * @brief signal_resultTest - сигнал, отправляемый, когда пользователь завершает задание
     * @param resultTest - результат выполнения задания
     */
    void signal_resultTest(ResultTest resultTest);

private slots:
    /**
     * @brief slot_maxValueChanged - слот, вызываемый при изменении максимального
     * генерируемого числа
     * @param value - новое максимальное число (десятичное)
     */
    void slot_maxValueChanged(int value);

    /**
     * @brief slot_nextTest - слот, вызываемый, когда пользователь нажимает на кнопку "Далее"
     */
    void slot_nextTest();

private:
    /**
     * @brief setTaskText - метод, устанавливающий описание задания
     * @param value - число, которое нужно перевести
     * @param fromNumberSystem - система счисления, из которой нужно перевести
     * @param toNumberSystem - система счисления, в которую нужно перевести
     */
    void setTaskText(unsigned value, NumberSystem fromNumberSystem, NumberSystem toNumberSystem);

    /**
     * @brief next - метод, генерирующий новое задание
     */
    void next();

private:
    Ui::ModeTestingWidget *ui;
    QString answer;  // Ответ на задание
    TimePoint timeTestStart;
};

