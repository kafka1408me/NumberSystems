#include "numbersysteminputvalidator.h"
#include <QDebug>
#include <array>

//const QRegExp rxp2{"[^0-1]$"};
//const QRegExp rxp8{"[^0-7]$"};
//const QRegExp rxp10{"[^0-9]$"};
//const QRegExp rxp16{"[^0-9a-f]$", Qt::CaseInsensitive};

const std::array<QRegExp, 4> rxps = {
    QRegExp{"[^0-1]"},
    QRegExp{"[^0-7]"},
    QRegExp{"[^0-9]"},
    QRegExp{"[^0-9a-f]", Qt::CaseInsensitive}
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
    qDebug() << "fixup";
    input.replace(getRegExp(), QStringLiteral(""));
}

QValidator::State NumberSystemInputValidator::validate(QString &input, int &pos) const
{
    Q_UNUSED(pos);

    qDebug() << input;
    if(!input.contains(getRegExp()))
    {
        qDebug() << "State::Acceptable";
        return State::Acceptable;
    }
    qDebug() << "State::Invalid";
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
