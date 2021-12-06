#pragma once

#include "functions.h"
#include "numbersysteminputvalidator.h"

template <typename T>
class NumberParameters
{
public:
    void setNumberSystems(NumberSystem from = NumberSystem::Ten, NumberSystem to = NumberSystem::Two)
    {
        T* super = static_cast<T*>(this);
        super->ui->numberSystemFromCmbx->setCurrentIndex(int(from));
        super->ui->numberSystemToCmbx->setCurrentIndex(int(to));
    }

    void addItemsForComboBoxes()
    {
        T* super = static_cast<T*>(this);
        super->ui->numberSystemFromCmbx->addItems(numberSystems);
        super->ui->numberSystemToCmbx->addItems(numberSystems);
    }

    void init(NumberSystem from = NumberSystem::Ten, NumberSystem to = NumberSystem::Two)
    {
        addItemsForComboBoxes();
        setNumberSystems(from, to);
    }

    void fixInput(NumberSystem numberSystem)
    {
        T* super = static_cast<T*>(this);
        MyValidator->setNumberSystem(numberSystem);
        QString str = super->ui->numberLineEdit->text();
        int pos = 0;
        QValidator::State state = MyValidator->validate(str, pos);

        if(state == QValidator::State::Invalid)
        {
            MyValidator->fixup(str);
            super->ui->numberLineEdit->setText(str);
        }
    }
};

