#include "namevalidator.h"

// Регулярное выражение:
// все символы кроме букв русского и латинского алфавитов
const QRegExp re{"[^а-яa-z]", Qt::CaseInsensitive};

NameValidator::NameValidator(QObject *parent):
    QValidator(parent)
{
}

QValidator::State NameValidator::validate(QString &input, int &pos) const
{
    // pos не используется
    // Q_UNUSED чтобы не было предупреждения
    Q_UNUSED(pos);

    if(input.contains(re))
    {
        return State::Invalid;
    }
    // Если ввод не содержит недопустимых символов,
    // но количество символов недостаточное
    else if(input.size() < minLengthName)
    {
        return State::Intermediate;
    }
    return State::Acceptable;
}
