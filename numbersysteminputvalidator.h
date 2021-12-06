#pragma once

/*************************************************
 *
 * NumberSystemInputValidator - класс валидатора
 * введенного пользователем текста (числа в какой-то
 * системе счисления)
 *
 *************************************************/

#include <QValidator>
#include "functions.h"

// Класс реализован при помощи паттерна Singleton (одиночка)
class NumberSystemInputValidator : public QValidator
{
public:
    /**
     * @brief getValidator - статическая функция для получения указателя на валидатор
     * @return указатель на валидатор
     */
    static NumberSystemInputValidator* getValidator();

    /**
     * @brief fixup - метод для исправления текста
     * @param input - ссылка на строку текста, который нужно исправить
     */
    void fixup(QString &input) const override;

    /**
     * @brief validate - метод для валидации текста
     * @param input - проверяемая строка
     * @param pos - позиция в строке
     * @return значение перечисления QValidator::State
     */
    QValidator::State validate(QString &input, int &pos) const override;

    /**
     * @brief setNumberSystem - метод для установки системы счисления
     * @param numberSystem - система счисления
     */
    void setNumberSystem(NumberSystem numberSystem);

private:
    /**
     * @brief getRegExp - метод, возвращающий регулярное выражение
     * для валидации текста в соответствии с становленной системой счисления
     * @return регулярное выражение для валидации текста
     */
    const QRegExp& getRegExp() const;

    // Конструктор в закрытой секции, потому что не должен вызываться напрямую
    // в соответсвии с паттерном Singleton
    NumberSystemInputValidator(NumberSystem numberSystem, QObject *parent = nullptr);

private:
    NumberSystem currentNumberSystem;  // СС, используемая для валидации текста
};

// Для удобного доступа к валидатору
#define MyValidator NumberSystemInputValidator::getValidator()


