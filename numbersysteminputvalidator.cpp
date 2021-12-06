#include "numbersysteminputvalidator.h"
//#include <QDebug>
#include <array>

// Массив регулярных выражений для валидации текста для каждой из доступных систем счисления
const std::array<QRegExp, 4> rxps = {
    QRegExp{"[^0-1]"},                          // Для 2-й системы счисления
    QRegExp{"[^0-7]"},                          // Для 8-й системы счисления
    QRegExp{"[^0-9]"},                          // Для 10-й системы счисления
    QRegExp{"[^0-9a-f]", Qt::CaseInsensitive}   // Для 16-й системы счисления (не зависит от регистра)
};


NumberSystemInputValidator::NumberSystemInputValidator(NumberSystem numberSystem, QObject *parent) :
    QValidator(parent),
    currentNumberSystem(numberSystem)
{
 /*Empty*/
}

NumberSystemInputValidator* NumberSystemInputValidator::getValidator()
{
    static NumberSystemInputValidator validator(NumberSystem::Ten);
    return &validator;
}

void NumberSystemInputValidator::fixup(QString &input) const
{
 //   qDebug() << "fixup";
    input.replace(getRegExp(), "");
}

QValidator::State NumberSystemInputValidator::validate(QString &input, int &pos) const
{
    // Для того, чтобы не было предупреждения о неиспользуемой переменной
    Q_UNUSED(pos);

    // Если не содержит символов, не вошедших
    // в регулярное выражение
    if(!input.contains(getRegExp()))
    {
        return State::Acceptable;
    }
    return State::Invalid;
}

void NumberSystemInputValidator::setNumberSystem(NumberSystem numberSystem)
{
    currentNumberSystem = numberSystem;
}

inline const QRegExp &NumberSystemInputValidator::getRegExp() const
{
    return rxps[int(currentNumberSystem)];
}
