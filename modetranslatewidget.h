#pragma once

#include <QWidget>
#include "NumberParameters.h"

namespace Ui {
class ModeTranslateWidget;
}

class ModeTranslateWidget : public QWidget, NumberParameters<ModeTranslateWidget>
{
    Q_OBJECT
private:
    friend class NumberParameters<ModeTranslateWidget>;
public:
    explicit ModeTranslateWidget(QWidget *parent = nullptr);

    ~ModeTranslateWidget();

private slots:
    void slot_translate();

    void slot_currentIndexFromChanged(int index);

private:
    Ui::ModeTranslateWidget *ui;
};

