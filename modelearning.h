#pragma once

#include <QWidget>
#include "functions.h"

namespace Ui {
class ModeLearning;
}

class ModeLearning : public QWidget
{
    Q_OBJECT

public:
    explicit ModeLearning(QWidget *parent = nullptr);
    ~ModeLearning();

    void drawTranslateAlgorithm(QString valStr, NumberSystem numberSystemFrom, NumberSystem numberSystemTo);


private:
    Ui::ModeLearning *ui;
};


