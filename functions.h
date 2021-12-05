#pragma once

#include <QStringList>

enum NumberSystem
{
    Two     = 0,
    Eight   = 1,
    Ten     = 2,
    Sixteen = 3
};

extern const QStringList numberSystems;

unsigned valueOfNumberSystem(NumberSystem ns);

QString translateNumber(const QString& fromStr, NumberSystem numberSystemFrom, NumberSystem numberSystemTo);

unsigned generateRandomNumber(unsigned min, unsigned max);

NumberSystem generateRandomNumberSystem(int without = -1);

int getCountDigits(int val);
