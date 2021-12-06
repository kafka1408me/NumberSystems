#pragma once

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
    void signal_startTest(int countTasks);

private slots:
    void slot_startTest();

private:
    Ui::StartTest *ui;
};


