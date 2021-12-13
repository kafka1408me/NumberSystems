#pragma once

/*************************************************
 *
 * MenuWidget - класс меню, с помощью которого
 * активируются режимы работы программы
 *
 *************************************************/

#include <QWidget>

namespace Ui {
class MenuWidget;
}

class MenuWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MenuWidget(QWidget *parent = nullptr);

    ~MenuWidget();

signals:
    /**
     * @brief signal_toModeLearning - сигнал о том, что нужно перейти
     * в режим обучения
     */
    void signal_toModeLearning();

    /**
     * @brief signal_toModeTranslate - сигнал о том, что нужно перейти
     * в режим перевода
     */
    void signal_toModeTranslate();

    /**
     * @brief signal_toModeTest - сигнал о том, что нужно перейти
     * в режим тестирования
     */
    void signal_toModeTest();

    /**
     * @brief signal_toModeSelfTest - сигнал о том, что нужно перейти
     * в режим самотестирования
     */
    void signal_toModeSelfTest();

    /**
     * @brief signal_toStatistics - сигнал о том, что нужно перейти
     * к просмотру статистики пользователей
     */
    void signal_toStatistics();

private:
    Ui::MenuWidget *ui;
};

