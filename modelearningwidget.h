#pragma once

#include <QWidget>
#include "NumberParameters.h"

namespace Ui {
class ModeLearningWidget;
}


class ModeLearningWidget : public QWidget, public NumberParameters<ModeLearningWidget>
{
    Q_OBJECT
private:
    friend class NumberParameters<ModeLearningWidget>;
public:
    explicit ModeLearningWidget(QWidget *parent = nullptr);
    ~ModeLearningWidget();

    void drawTranslateAlgorithm(QString valStr, NumberSystem numberSystemFrom, NumberSystem numberSystemTo);

private slots:
    void slot_translate();

    void slot_fromNumberSystemChanged(int index);

private:
    Ui::ModeLearningWidget *ui;
};


