#pragma once

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

    void setValues(int countTasks, int countRight);

signals:
    void signal_ok();

private slots:
    void slot_ok();

private:
    Ui::FinishTest *ui;
};

