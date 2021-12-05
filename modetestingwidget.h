#pragma once

#include <QWidget>
#include "functions.h"

namespace Ui {
class ModeTestingWidget;
}

enum ResultTest
{
    Right = 0,
    Skip  = 1,
    Error = 2
};

class ModeTestingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ModeTestingWidget(QWidget *parent = nullptr);
    ~ModeTestingWidget();

signals:
    void signal_resultTest(ResultTest resultTest);

private slots:
    void slot_minValueChanged(int value);

    void slot_maxValueChanged(int value);

    void slot_nextTest();

    void next();

private:
    void setTaskText(unsigned value, NumberSystem fromNumberSystem, NumberSystem toNumberSystem);

private:
    Ui::ModeTestingWidget *ui;
    QString answer;
};

