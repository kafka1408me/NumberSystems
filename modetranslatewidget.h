#pragma once

#include <QWidget>
#include "NumberParameters.h"
#include "preparerpage.h"

namespace Ui {
class ModeTranslateWidget;
}

class ModeTranslateWidget : public QWidget, public NumberParameters<ModeTranslateWidget>, public PreparerPage
{
    Q_OBJECT
private:
    friend class NumberParameters<ModeTranslateWidget>;
public:
    explicit ModeTranslateWidget(QWidget *parent = nullptr);

    ~ModeTranslateWidget();

    void preparePage() override;

private slots:
    void slot_translate();

    void slot_currentIndexFromChanged(int index);

private:
    Ui::ModeTranslateWidget *ui;
};

