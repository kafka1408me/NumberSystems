#include "menuwidget.h"
#include "ui_menuwidget.h"

MenuWidget::MenuWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MenuWidget)
{
    ui->setupUi(this);

    // Сигналы от нажатия (кликов) кнопок вызывают испускание сигналов перехода в различные
    // режимы работы приложения
    connect(ui->modeLearningBtn, SIGNAL(clicked(bool)), this, SIGNAL(signal_toModeLearning()));
    connect(ui->modeTranslateBtn, SIGNAL(clicked(bool)), this, SIGNAL(signal_toModeTranslate()));
    connect(ui->modeTestBtn, SIGNAL(clicked(bool)), this, SIGNAL(signal_toModeTest()));
    connect(ui->modeSelfTestBtn, SIGNAL(clicked(bool)), this, SIGNAL(signal_toModeSelfTest()));
}

MenuWidget::~MenuWidget()
{
    delete ui;
}
