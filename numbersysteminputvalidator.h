#pragma once

#include <QValidator>
#include "functions.h"

class NumberSystemInputValidator : public QValidator
{
public:
    static NumberSystemInputValidator* getValidator();

    void fixup(QString &input) const override;

    QValidator::State validate(QString &input, int &pos) const override;

    void setNumberSystem(NumberSystem numberSystem);

private:
    const QRegExp& getRegExp() const;

    explicit NumberSystemInputValidator(NumberSystem numberSystem, QObject *parent = nullptr);

private:
    NumberSystem currentNumberSystem;
};

#define MyValidator NumberSystemInputValidator::getValidator()


