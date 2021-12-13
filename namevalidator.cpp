#include "namevalidator.h"


const QRegExp re{"[^а-яa-z]", Qt::CaseInsensitive};

NameValidator::NameValidator(QObject *parent):
    QValidator(parent)
{
}

QValidator::State NameValidator::validate(QString &input, int &pos) const
{
    Q_UNUSED(pos);

    if(input.contains(re))
    {
        return State::Invalid;
    }
    else if(input.size() < minLengthName)
    {
        return State::Intermediate;
    }
    return State::Acceptable;
}
