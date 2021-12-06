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

private:
    Ui::ModeLearning *ui;
};


