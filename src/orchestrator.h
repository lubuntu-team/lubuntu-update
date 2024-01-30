#ifndef ORCHESTRATOR_H
#define ORCHESTRATOR_H

#include "mainwindow.h"

#include <QDateTime>
#include <QObject>
#include <QStringList>

class QTimer;
class QSystemTrayIcon;

class Orchestrator : public QObject
{
    Q_OBJECT
public:
    explicit Orchestrator(QObject *parent = nullptr);

public slots:
    void displayUpdater();

private slots:
    void checkForUpdates();
    void handleUpdatesInstalled();
    void handleUpdatesRefreshed();
    void onNewReleaseAvailable(QStringList releaseCodes);

private:
    QTimer *checkTimer;
    QSystemTrayIcon *trayIcon;
    QList<QStringList> updateInfo;
    MainWindow updaterWindow;
    QDateTime nextUpgradeCheck;
};

#endif // ORCHESTRATOR_H
