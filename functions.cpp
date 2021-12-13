#include "functions.h"
#include <QRandomGenerator>  // для использование генератора случайных чисел

// Список доступных систем счисления
const QStringList numberSystems{"2", "8", "10", "16"};

constexpr int secsInhours = 60*60;

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
    // Сначала переводим строку в целочисленный тип
    unsigned int number = fromStr.toUInt(nullptr,  // указатель на булеву переменную, в которую будет записан результат
                                         valueOfNumberSystem(numberSystemFrom)  // в какой системе счисления представлено число
                                         );
    return QString::number(number,
                           valueOfNumberSystem(numberSystemTo) // В какой систему счисления записать число в строку
                           );
}

unsigned generateRandomNumber(unsigned min, unsigned max)
{
    // В bounded указывается мин. и макс. значения. Макс значение при этом не включительно
    return QRandomGenerator::global()->bounded(min, max + 1);
}

NumberSystem generateRandomNumberSystem(int without)
{
    int indx = 0;
    // В цикле генерируем случайные СС до тех пор,
    // пока не будет получена СС, отличная от without
    do{
        indx = generateRandomNumber(0, 3);
    }while(indx == without);

    return NumberSystem(indx);
}

int getCountDigits(int val)
{
    // Если val == 0, то количество цифр 1, иначе узнаем с помощью десятичного логарифма
    return val == 0 ? 1 : int (log10 (val) + 1);
}

QString addColorToString(const QString& color,const QString &text)
{
     return "<font color=\"" + color + "\">" + text  +"</font>";
}

QString getTimeStrFromSec(quint64 sec)
{
    quint32 hours = sec / secsInhours;
    sec = sec % secsInhours;
    quint32 mins = sec / 60;
    sec = sec % 60;

    return QString("%1 ч %2 мин %3 сек").
            arg(QString::number(hours), QString::number(mins), QString::number(sec));
}

TimePoint getCurrentTime()
{
    return ClockType::now();
}

quint64 getDiffTime(const TimePoint &oldTime)
{
    using namespace std;
    using namespace chrono;

    ClockType::duration diff(ClockType::now() - oldTime);
    return duration_cast<seconds>(diff).count();
}
