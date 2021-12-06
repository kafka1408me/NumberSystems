#pragma once

#include <QWidget>
#include "NumberParameters.h"
#include "preparerpage.h"

namespace Ui {
class ModeLearningWidget;
}


class ModeLearningWidget : public QWidget, public NumberParameters<ModeLearningWidget>, public PreparerPage
{
    Q_OBJECT
private:
    friend class NumberParameters<ModeLearningWidget>;
public:
    explicit ModeLearningWidget(QWidget *parent = nullptr);

    ~ModeLearningWidget();

    void drawTranslateAlgorithm(QString valStr, NumberSystem numberSystemFrom, NumberSystem numberSystemTo);

    void preparePage() override;

private slots:
    void slot_translate();

    void slot_fromNumberSystemChanged(int index);

private:
    Ui::ModeLearningWidget *ui;
};


