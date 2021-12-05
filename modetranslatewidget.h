#pragma once

#include <QWidget>
#include "functions.h"

namespace Ui {
class ModeTranslateWidget;
}

class ModeTranslateWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ModeTranslateWidget(QWidget *parent = nullptr);

    ~ModeTranslateWidget();

private slots:
    void slot_translate();

    void slot_currentIndexFromChanged(int index);

private:
    void setNumberSystems(NumberSystem from = NumberSystem::Ten, NumberSystem to = NumberSystem::Two);

private:
    Ui::ModeTranslateWidget *ui;
};

