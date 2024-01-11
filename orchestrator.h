#ifndef ORCHESTRATOR_H
#define ORCHESTRATOR_H

#include "mainwindow.h"

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

private:
    QTimer *checkTimer;
    QSystemTrayIcon *trayIcon;
    QList<QStringList> updateInfo;
    MainWindow updaterWindow;
};

#endif // ORCHESTRATOR_H
