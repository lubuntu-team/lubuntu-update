#include "orchestrator.h"
#include "mainwindow.h"
#include "aptmanager.h"
#include "releaseupgradewindow.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QIcon>
#include <QProcess>
#include <QSystemTrayIcon>
#include <QTimer>

Orchestrator::Orchestrator(QObject *parent)
    : QObject{parent}
{
    checkTimer = new QTimer(); // every time this triggers, the apt database is checked for new updates
    trayIcon = new QSystemTrayIcon(); // this is shown to the user to offer updates

    /*
     * Parse the Lubuntu Update config file. It contains two critical pieces
     * of info - when the system last offered the user a release upgrade,
     * and whether the user has disabled release upgrade notifications.
     */
    QFile configFile(QDir::homePath() + "/.config/lubuntu-update.conf");
    bool success = configFile.open(QFile::ReadOnly);
    if (success) {
        char lineBuf[2048];
        while (!configFile.atEnd()) {
            configFile.readLine(lineBuf, 2048);
            QString line(lineBuf);
            line = line.trimmed();
            QStringList lineParts = line.split("=");
            if (lineParts.count() == 2) {
                if (lineParts[0] == "nextDoReleaseUpgradeNotify") {
                    nextUpgradeCheck = QDateTime::fromSecsSinceEpoch(lineParts[1].toLongLong());
                } else {
                    qWarning() << "Unrecognized config line: " << line;
                }
            } else {
                qWarning() << "Wrong number of fields in line: " << line;
            }
        }
    }
    configFile.close();

    connect(checkTimer, &QTimer::timeout, this, &Orchestrator::checkForUpdates);
    connect(trayIcon, &QSystemTrayIcon::activated, this, &Orchestrator::displayUpdater);
    connect(&updaterWindow, &MainWindow::updatesInstalled, this, &Orchestrator::handleUpdatesInstalled);
    connect(&updaterWindow, &MainWindow::updatesRefreshed, this, &Orchestrator::handleUpdatesRefreshed);
    connect(&updaterWindow, &MainWindow::newReleaseAvailable, this, &Orchestrator::onNewReleaseAvailable);

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

void Orchestrator::onNewReleaseAvailable(QStringList releaseCodes)
{
    // First, determine what kinds of releases the user wants to see.
    QFile druTypeFile("/etc/update-manager/release-upgrades");
    bool success = druTypeFile.open(QFile::ReadOnly);
    QString druType;
    if (success) {
        char lineBuf[2048];
        while (!druTypeFile.atEnd()) {
            druTypeFile.readLine(lineBuf, 2048);
            QString line(lineBuf);
            line = line.trimmed();
            if (line == "Prompt=lts") {
                druType="lts";
                druTypeFile.close();
                break;
            } else if (line == "Prompt=none") {
                // The user has disabled all upgrade prompts.
                druTypeFile.close();
                return;
            } else if (line == "Prompt=normal") {
                druType="normal";
                druTypeFile.close();
                break;
            }
        }
    } else {
        druType="normal";
        druTypeFile.close();
    }

    for (int i = 0;i < releaseCodes.count();i++) {
        QStringList releaseCodeParts = releaseCodes[i].split('.');
        if (releaseCodeParts.count() >= 2) {
            int releaseYear = releaseCodeParts[0].toInt();
            int releaseMonth = releaseCodeParts[1].toInt();
            if (((releaseYear % 2 == 0) && (releaseMonth == 4)) || druType == "normal") {
                QDateTime now = QDateTime::currentDateTime();
                if (nextUpgradeCheck < now) {
                    ReleaseUpgradeWindow upgradeWindow(releaseCodes[i]);
                    upgradeWindow.exec();
                    if (upgradeWindow.getUpgradeAccepted()) {
                        doReleaseUpgrade();
                    } else if (upgradeWindow.getUpgradeDelayStamp() > 0) {
                        delayReleaseUpgrade(upgradeWindow.getUpgradeDelayStamp());
                    } else {
                        declineReleaseUpgrade();
                    }
                    break;
                }
            }
        }
    }
}

void Orchestrator::doReleaseUpgrade()
{
    QProcess druProcess;
    druProcess.setProgram("/usr/bin/lxqt-sudo");
    druProcess.setArguments(QStringList() << "/usr/libexec/lubuntu-update-backend" << "doReleaseUpgrade");
    druProcess.start();
    druProcess.waitForFinished(-1);
}

void Orchestrator::delayReleaseUpgrade(qint64 timestamp)
{
    QFile configFile(QDir::homePath() + "/.config/lubuntu-update.conf");
    bool success = configFile.open(QFile::WriteOnly);
    if (success) {
        configFile.write("nextDoReleaseUpgradeNotify=");
        configFile.write(QString::number(timestamp).toUtf8());
        configFile.write("\n");
    } else {
        qWarning() << "Could not write to " + QDir::homePath() + "/.config/lubuntu-update.conf, check permissions";
    }
    configFile.close();
    nextUpgradeCheck = QDateTime::fromSecsSinceEpoch(timestamp);
}

void Orchestrator::declineReleaseUpgrade()
{
    QProcess druProcess;
    druProcess.setProgram("/usr/bin/lxqt-sudo");
    druProcess.setArguments(QStringList() << "/usr/libexec/lubuntu-update-backend" << "declineReleaseUpgrade");
    druProcess.start();
    druProcess.waitForFinished(-1);
}
