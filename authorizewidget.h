#pragma once

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
    void signal_authorize(const UserData& userData);

public slots:
    void slot_show();

private slots:
    void slot_tryAuthorize();

private:
    Ui::AuthorizeWidget *ui;
};

