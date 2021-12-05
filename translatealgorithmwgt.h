#pragma once

#include <QWidget>
#include "functions.h"

class TranslateAlgorithmWgt : public QWidget
{
    Q_OBJECT
public:
    explicit TranslateAlgorithmWgt(QWidget *parent = nullptr);

    ~TranslateAlgorithmWgt();

    void drawTranslateAlgorithm(const QString& valStr, NumberSystem numberSystemFrom, NumberSystem numberSystemTo);

protected:
    void paintEvent(QPaintEvent*);

private:
    QString valStr;
    NumberSystem from;
    NumberSystem to;
};

