#include "releaseupgradewindow.h"
#include "ui_releaseupgradewindow.h"

#include "upgradedelaywindow.h"

#include <QDateTime>
#include <QMessageBox>

ReleaseUpgradeWindow::ReleaseUpgradeWindow(QString releaseCode, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReleaseUpgradeWindow)
{
    ui->setupUi(this);
    ui->upgradeLabel->setText(tr("An upgrade to Lubuntu %1 is available! Would you like to install this upgrade now?").arg(releaseCode));
    code=releaseCode;
    connect(ui->upgradeButton, &QPushButton::clicked, this, &ReleaseUpgradeWindow::onUpgradeClicked);
    connect(ui->remindButton, &QPushButton::clicked, this, &ReleaseUpgradeWindow::onRemindClicked);
    connect(ui->declineButton, &QPushButton::clicked, this, &ReleaseUpgradeWindow::onDeclineClicked);
}

ReleaseUpgradeWindow::~ReleaseUpgradeWindow()
{
    delete ui;
}

bool ReleaseUpgradeWindow::getUpgradeAccepted()
{
    return upgradeAccepted;
}

qint64 ReleaseUpgradeWindow::getUpgradeDelayStamp()
{
    return upgradeDelayStamp;
}

void ReleaseUpgradeWindow::onUpgradeClicked()
{
    upgradeAccepted = true;
    this->done(0);
}

void ReleaseUpgradeWindow::onRemindClicked()
{
    UpgradeDelayWindow delayWindow;
    delayWindow.exec();
    if (delayWindow.getDelayDays() > 0) {
        QDateTime delayDateTime = QDateTime::currentDateTime();
        delayDateTime = delayDateTime.addDays(delayWindow.getDelayDays());
        upgradeAccepted = false;
        upgradeDelayStamp = delayDateTime.toSecsSinceEpoch();
        this->done(0);
    }
}

void ReleaseUpgradeWindow::onDeclineClicked()
{
    QMessageBox confirmDialog;
    confirmDialog.setWindowTitle(tr("Lubuntu Update"));
    confirmDialog.setText(tr("You have declined the upgrade to Lubuntu %1.\nYou can upgrade manually by following the directions at https://manual.lubuntu.me/stable/D/upgrading.html.").arg(code));
    confirmDialog.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    int result = confirmDialog.exec();
    if (result == QMessageBox::Ok) {
        upgradeDelayStamp = 0;
        upgradeAccepted = false;
        this->done(0);
    }
}
