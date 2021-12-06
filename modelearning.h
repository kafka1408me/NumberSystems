#pragma once

#include <QWidget>
#include "NumberParameters.h"

namespace Ui {
class ModeLearning;
}


class ModeLearning : public QWidget, public NumberParameters<ModeLearning>
{
    Q_OBJECT
private:
    friend class NumberParameters<ModeLearning>;
public:
    explicit ModeLearning(QWidget *parent = nullptr);
    ~ModeLearning();

    void drawTranslateAlgorithm(QString valStr, NumberSystem numberSystemFrom, NumberSystem numberSystemTo);

private slots:
    void slot_translate();

    void slot_fromNumberSystemChanged(int index);

private:
    Ui::ModeLearning *ui;
};


