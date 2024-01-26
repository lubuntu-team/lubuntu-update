#include "orchestrator.h"
#include "mainwindow.h"
#include "aptmanager.h"

#include <QIcon>
#include <QSystemTrayIcon>
#include <QTimer>

Orchestrator::Orchestrator(QObject *parent)
    : QObject{parent}
{
    checkTimer = new QTimer(); // every time this triggers, the apt database is checked for new updates
    trayIcon = new QSystemTrayIcon(); // this is shown to the user to offer updates

    connect(checkTimer, &QTimer::timeout, this, &Orchestrator::checkForUpdates);
    connect(trayIcon, &QSystemTrayIcon::activated, this, &Orchestrator::displayUpdater);
    connect(&updaterWindow, &MainWindow::updatesInstalled, this, &Orchestrator::handleUpdatesInstalled);
    connect(&updaterWindow, &MainWindow::updatesRefreshed, this, &Orchestrator::handleUpdatesRefreshed);

    checkTimer->start(21600000); // check four times a day, at least one of those times unattended-upgrades should have refreshed the apt database

    checkForUpdates(); // check immediately after launch, which usually will be immediately after boot or login
}

/*
 * Checks the apt database to see if updated software is available, and places
 * the results in the updateInfo variable. If updated software is available,
 * show the system tray icon and trigger a notification.
 */
void Orchestrator::checkForUpdates()
{
    updateInfo = AptManager::getUpdateInfo();
    if (!updateInfo[0].isEmpty() || !updateInfo[1].isEmpty() || !updateInfo[2].isEmpty() || !updateInfo[3].isEmpty()) { // no need to check updateInfo[4], it will only ever contain security updates already listed in updateInfo[1]
        trayIcon->setIcon(QPixmap(":/res/images/update.svg"));
        trayIcon->show();
        // Yes, we do intentionally use updateInfo[1], then updateInfo[0], then updateInfo[2]. The updateInfo array is populated in a different order than the one we display in.
        trayIcon->showMessage("",
          tr("Updates available!\n\n%1 to upgrade, %2 to install, and %3 to remove.\n\nClick the tray icon to install the updates.")
          .arg(QString::number(updateInfo[1].count()), QString::number(updateInfo[0].count()), QString::number(updateInfo[2].count())));
    }
}

void Orchestrator::displayUpdater()
{
    if (!updaterWindow.isLockedOpen()) {
        if (!updaterWindow.isVisible()) {
            updaterWindow.hide();
        }
        updaterWindow.setUpdateInfo(updateInfo);
        updaterWindow.show();
    }
}

void Orchestrator::handleUpdatesInstalled()
{
    // We can't clear the updateInfo list directly as MainWindow::setUpdateInfo requires that it contains five inner lists (even if those lists are all empty).
    for (int i = 0;i < 5;i++) {
        updateInfo[i].clear();
    }
    trayIcon->hide();
}

void Orchestrator::handleUpdatesRefreshed()
{
    checkForUpdates();
    displayUpdater();
}
