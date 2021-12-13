#include "statisticswidget.h"
#include "ui_statisticswidget.h"
#include "filehandler.h"


StatisticsWidget::StatisticsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StatisticsWidget)
{
    ui->setupUi(this);

    ui->tableView->setModel(&model);
}

StatisticsWidget::~StatisticsWidget()
{
    delete ui;
}

void StatisticsWidget::preparePage()
{
    auto users = FileHandler::getUsers();
    model.setUsers(users);
}
