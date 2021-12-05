#pragma once

#include <QStackedWidget>


class Window: public QStackedWidget
{
    Q_OBJECT
public:
    Window(QWidget* parent = nullptr);
};


