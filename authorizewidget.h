#pragma once

/*************************************************
 *
 * AuthorizeWidget - класс виджета авторизации
 *
 *************************************************/

#include <QWidget>
#include "userdata.h"

namespace Ui {
class AuthorizeWidget;
}

class AuthorizeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AuthorizeWidget(QWidget *parent = nullptr);
    ~AuthorizeWidget();

signals:
    /**
     * @brief signal_authorize - сигнал об успешной авторизации
     * @param userData - данные пользователя
     */
    void signal_authorize(const UserData& userData);

public slots:
    /**
     * @brief slot_show - слот, открывающий виджет
     */
    void slot_show();

private slots:
    /**
     * @brief slot_tryAuthorize - слот, вызываемый
     * при нажатии на кнопку "Далее"
     */
    void slot_tryAuthorize();

private:
    Ui::AuthorizeWidget *ui;
};

