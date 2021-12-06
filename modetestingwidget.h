#pragma once

#include <QWidget>
#include "functions.h"
#include "preparerpage.h"


namespace Ui {
class ModeTestingWidget;
}

enum ResultTest
{
    Right = 0,
    Skip  = 1,
    Error = 2
};

class ModeTestingWidget : public QWidget, public PreparerPage
{
    Q_OBJECT
public:
    explicit ModeTestingWidget(QWidget *parent = nullptr);

    ~ModeTestingWidget();

    void preparePage() override;

signals:
    void signal_resultTest(ResultTest resultTest);

private slots:
    void slot_minValueChanged(int value);

    void slot_maxValueChanged(int value);

    void slot_nextTest();

private:
    void setTaskText(unsigned value, NumberSystem fromNumberSystem, NumberSystem toNumberSystem);

    void next();

private:
    Ui::ModeTestingWidget *ui;
    QString answer;
};

