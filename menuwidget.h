#pragma once

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
    void signal_toModeLearning();

    void signal_toModeTranslate();

    void signal_toModeTest();

    void signal_toModeSelfTest();

private:
    Ui::MenuWidget *ui;
};

