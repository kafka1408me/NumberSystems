#include "functions.h"
#include <QRandomGenerator>

const QStringList numberSystems{"2", "8", "10", "16"};

inline unsigned valueOfNumberSystem(NumberSystem ns)
{
    switch (ns)
    {
    case NumberSystem::Two:
        return 2;
    case NumberSystem::Eight:
        return 8;
    case NumberSystem::Ten:
        return 10;
    case NumberSystem::Sixteen:
        return 16;
    }
}


QString translateNumber(const QString& fromStr, NumberSystem numberSystemFrom, NumberSystem numberSystemTo)
{
    unsigned int number = fromStr.toUInt(nullptr, valueOfNumberSystem(numberSystemFrom));
    return QString::number(number, valueOfNumberSystem(numberSystemTo));
}

unsigned generateRandomNumber(unsigned min, unsigned max)
{
    return QRandomGenerator::global()->bounded(min, max + 1);
}

NumberSystem generateRandomNumberSystem(int without)
{
    int indx = 0;
    do{
        indx = generateRandomNumber(0, 3);
    }while(indx == without);

    return NumberSystem(indx);
}

int getCountDigits(int val)
{
    return val == 0 ? 1 : int (log10 (val) + 1);
}
