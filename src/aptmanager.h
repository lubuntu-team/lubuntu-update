#ifndef APTMANAGER_H
#define APTMANAGER_H

#include <QObject>
#include <QStringList>

class QProcess;
class QProgressBar;
class QPlainTextEdit;
class QSystemTrayIcon;

class AptManager : public QObject
{
    Q_OBJECT

public:
    AptManager(QObject *parent = nullptr);
    static QList<QStringList> getUpdateInfo();
    void applyFullUpgrade();
    void checkForUpdates();
    void keepConffile(QString conffile);
    void replaceConffile(QString conffile);
    void doneWithConffiles();

signals:
    void updateComplete();
    void checkUpdatesComplete();
    void progressUpdated(int progress);
    void logLineReady(QString logLine);
    void conffileListReady(QStringList conffileList);
    void newRelease(QString code);

private slots:
    void handleUpdateProcessBuffer();
    void handleCheckUpdateProcessBuffer();

private:
    static void parseAptLine(QString line, bool *gettingInstallPackages, bool *gettingUpgradePackages, bool *gettingUninstallPackages, bool *gettingHeldPackages, bool *gettingPackageList);
    static QStringList getSecurityUpdateList();
    QProcess *aptProcess;
    QList<QStringList> internalUpdateInfo;
    QStringList instUnpackList;
    QStringList instConfigList;
    QStringList upgradeUnpackList;
    QStringList upgradeConfigList;
    QStringList removeList;
    quint32 numPackagesToPrep;
    quint32 internalUpdateProgress;
};

#endif // APTMANAGER_H
