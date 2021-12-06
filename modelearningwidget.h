#pragma once

/*************************************************
 *
 * ModeLearningWidget - класс виджета, реализующий
 * режим обучения
 *
 *************************************************/

#include <QWidget>
#include "NumberParameters.h"
#include "preparerpage.h"

namespace Ui {
class ModeLearningWidget;
}


class ModeLearningWidget : public QWidget, public NumberParameters<ModeLearningWidget>, public PreparerPage
{
    Q_OBJECT
private:
    // для доступа из класса NumberParameters<ModeLearningWidget> к
    // объкту ui класса ModeLearningWidget
    friend class NumberParameters<ModeLearningWidget>;
public:
    explicit ModeLearningWidget(QWidget *parent = nullptr);

    ~ModeLearningWidget();

    /**
     * @brief drawTranslateAlgorithm - функция рисования алгоритма перевода в систему счисления
     * @param valStr
     * @param numberSystemFrom
     * @param numberSystemTo
     */
    void drawTranslateAlgorithm(QString valStr, NumberSystem numberSystemFrom, NumberSystem numberSystemTo);

    void preparePage() override;

private slots:
    /**
     * @brief slot_translate - слот, вызывающий рисование объяснения перевода
     * в систему счисления
     */
    void slot_translate();

    /**
     * @brief slot_fromNumberSystemChanged - слот, вызываемый при
     * изменении системы счисления, из которой выполняется перевод
     * @param index - новый индекс
     */
    void slot_fromNumberSystemChanged(int index);

private:
    Ui::ModeLearningWidget *ui;
};


