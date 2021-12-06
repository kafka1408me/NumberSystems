#pragma once

/*************************************************
 *
 * ModeTranslateWidget - класс виджета, реализующий
 * режим перевода
 *
 *************************************************/

#include <QWidget>
#include "NumberParameters.h"
#include "preparerpage.h"

namespace Ui {
class ModeTranslateWidget;
}

class ModeTranslateWidget : public QWidget, public NumberParameters<ModeTranslateWidget>, public PreparerPage
{
    Q_OBJECT
private:
    // для доступа из класса NumberParameters<ModeTranslateWidget> к
    // объкту ui класса ModeTranslateWidget
    friend class NumberParameters<ModeTranslateWidget>;
public:
    explicit ModeTranslateWidget(QWidget *parent = nullptr);

    ~ModeTranslateWidget();

    void preparePage() override;

private slots:
    /**
     * @brief slot_translate - метод, осуществляющий перевод из одной СС в другую
     */
    void slot_translate();

    /**
     * @brief slot_currentIndexFromChanged - слот, вызываемый при изменении системы счисления, из которой
     * нужно перевести
     * @param index - индекс новой системы счисления
     */
    void slot_currentIndexFromChanged(int index);

private:
    Ui::ModeTranslateWidget *ui;
};

