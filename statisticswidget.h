#pragma once

#include <QWidget>
#include "preparerpage.h"
#include "usersmodel.h"


namespace Ui {
class StatisticsWidget;
}

class StatisticsWidget : public QWidget, public PreparerPage
{
    Q_OBJECT
public:
    explicit StatisticsWidget(QWidget *parent = nullptr);
    ~StatisticsWidget();

    void preparePage() override;

private:
    Ui::StatisticsWidget *ui;
    UsersModel model;
};

