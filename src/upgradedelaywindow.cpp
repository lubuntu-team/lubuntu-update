#include "upgradedelaywindow.h"
#include "ui_upgradedelaywindow.h"

UpgradeDelayWindow::UpgradeDelayWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UpgradeDelayWindow)
{
    ui->setupUi(this);
    connect(ui->okButton, &QPushButton::clicked, this, &UpgradeDelayWindow::onOkClicked);
    connect(ui->cancelButton, &QPushButton::clicked, this, &UpgradeDelayWindow::onCancelClicked);
}

UpgradeDelayWindow::~UpgradeDelayWindow()
{
    delete ui;
}

qint64 UpgradeDelayWindow::getDelayDays()
{
    return delayDays;
}

void UpgradeDelayWindow::onOkClicked()
{
    switch (ui->timeTypeComboBox->currentIndex()) {
    case 0: // days
        delayDays = ui->timeSpinBox->value();
        break;
    case 1: // weeks
        delayDays = ui->timeSpinBox->value() * 7;
        break;
    case 2: // months
        delayDays = ui->timeSpinBox->value() * 28;
        break;
    }
    this->done(0);
}

void UpgradeDelayWindow::onCancelClicked()
{
    this->done(0);
}
