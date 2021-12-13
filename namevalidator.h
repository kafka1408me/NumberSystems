#pragma once

#include <QValidator>

constexpr int minLengthName = 2;

class NameValidator: public QValidator
{
public:
    NameValidator(QObject * parent = nullptr);

    /**
     * @brief validate - метод для валидации текста
     * @param input - проверяемая строка
     * @param pos - позиция в строке
     * @return значение перечисления QValidator::State
     */
    QValidator::State validate(QString &input, int &pos) const override;
};


