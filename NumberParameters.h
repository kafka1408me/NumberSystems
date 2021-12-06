#pragma once

#include "functions.h"

template <typename T>
class NumberParameters
{
public:
    void setNumberSystems(NumberSystem from = NumberSystem::Ten, NumberSystem to = NumberSystem::Two)
    {
        static_cast<T*>(this)->ui->numberSystemFromCmbx->setCurrentIndex(int(from));
        static_cast<T*>(this)->ui->numberSystemToCmbx->setCurrentIndex(int(to));
    }

    void addItemsForComboBoxes()
    {
        static_cast<T*>(this)->ui->numberSystemFromCmbx->addItems(numberSystems);
        static_cast<T*>(this)->ui->numberSystemToCmbx->addItems(numberSystems);
    }

    void init(NumberSystem from = NumberSystem::Ten, NumberSystem to = NumberSystem::Two)
    {
        setNumberSystems(from, to);
        addItemsForComboBoxes();
    }
};

