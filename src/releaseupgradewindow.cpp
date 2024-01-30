#include "releaseupgradewindow.h"
#include "ui_releaseupgradewindow.h"

ReleaseUpgradeWindow::ReleaseUpgradeWindow(QString releaseCode, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReleaseUpgradeWindow)
{
    ui->setupUi(this);
    ui->upgradeLabel->setText(tr("An upgrade to Lubuntu %1 is available! Would you like to install this upgrade now?").arg(releaseCode));
    connect(ui->upgradeButton, &QPushButton::clicked, this, &ReleaseUpgradeWindow::onUpgradeClicked);
    connect(ui->remindButton, &QPushButton::clicked, this, &ReleaseUpgradeWindow::onRemindClicked);
    connect(ui->declineButton, &QPushButton::clicked, this, &ReleaseUpgradeWindow::onDeclineClicked);
}

ReleaseUpgradeWindow::~ReleaseUpgradeWindow()
{
    delete ui;
}

void ReleaseUpgradeWindow::onUpgradeClicked()
{

}

void ReleaseUpgradeWindow::onRemindClicked()
{

}

void ReleaseUpgradeWindow::onDeclineClicked()
{

}
