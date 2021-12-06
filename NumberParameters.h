#pragma once

/*************************************************
 *
 * NumberParameters - шаблонный класс, используемый
 * в качестве базового для реализации в
 * классах-наследниках некоторой части функциональности
 * работы с системами счисления. Для этого в
 * классах-наследниках должны быть соответствующие
 * данные-члены.
 *
 *************************************************/

#include "functions.h"
#include "numbersysteminputvalidator.h"

// T должен быть производным классом от NumberParameters<T>
template <typename T>
class NumberParameters
{
public:
    /**
     * @brief setNumberSystems - метод для установки систем счисления в комбобоксы
     * @param from
     * @param to
     */
    void setNumberSystems(NumberSystem from = NumberSystem::Ten, NumberSystem to = NumberSystem::Two)
    {
        // Преобразуем указатель к указателю на производный класс
        T* super = static_cast<T*>(this);
        super->ui->numberSystemFromCmbx->setCurrentIndex(int(from));
        super->ui->numberSystemToCmbx->setCurrentIndex(int(to));
    }

    /**
     * @brief addItemsForComboBoxes - метод для установки в комбобоксы
     * строк, представляющих собой доступные системы счисления
     */
    void addItemsForComboBoxes()
    {
        // Преобразуем указатель к указателю на производный класс
        T* super = static_cast<T*>(this);
        super->ui->numberSystemFromCmbx->addItems(numberSystems);
        super->ui->numberSystemToCmbx->addItems(numberSystems);
    }

    /**
     * @brief init - метод для инициализации комбобоксов
     * @param from - СС, из которой переводим
     * @param to - СС, в которую переводим
     */
    void init(NumberSystem from = NumberSystem::Ten, NumberSystem to = NumberSystem::Two)
    {
        addItemsForComboBoxes();
        setNumberSystems(from, to);
    }

    /**
     * @brief fixInput - метод для исправления (в случае необходимости)
     * пользовательского ввода
     * @param numberSystem - новая система счисления
     */
    void fixInput(NumberSystem numberSystem)
    {
        // Преобразуем указатель к указателю на производный класс
        T* super = static_cast<T*>(this);
        MyValidator->setNumberSystem(numberSystem);
        QString str = super->ui->numberLineEdit->text();
        int pos = 0;
        QValidator::State state = MyValidator->validate(str, pos);

        // Если строка не подходит (согласно валидации)
        if(state == QValidator::State::Invalid)
        {
            MyValidator->fixup(str);
            super->ui->numberLineEdit->setText(str);
        }
    }

    /**
     * @brief setCurrentValidator - метод установки текущей системы счисления
     * в валадатор. Этот метод вызывается перед показом виджета (класса-наследника)
     * в окне приложения
     */
    void setCurrentValidator()
    {
        // Преобразуем указатель к указателю на производный класс
        T* super = static_cast<T*>(this);
        NumberSystem numberSystem = NumberSystem(super->ui->numberSystemFromCmbx->currentIndex());
        MyValidator->setNumberSystem(numberSystem);
    }
};

