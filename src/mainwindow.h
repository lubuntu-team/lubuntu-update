#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "aptmanager.h"

#include <QCloseEvent>
#include <QMainWindow>
#include <QStringList>

class QSystemTrayIcon;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setUpdateInfo(QList<QStringList> updateInfo);
    bool isLockedOpen();

signals:
    void updatesInstalled();
    void updatesRefreshed();
    void newReleaseAvailable(QStringList releaseCodes);

protected slots:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void onInstallButtonClicked();
    void onCheckUpdatesButtonClicked();
    void onCloseButtonClicked();
    void onUpdateCompleted();
    void onCheckUpdatesCompleted();
    void onProgressUpdate(int progress);
    void onLogLineReady(QString logLine);
    void onConffileListReady(QStringList conffileList);
    void onNewRelease(QString code);

private:
    Ui::MainWindow *ui;
    AptManager *aptManager;
    QStringList releaseCodes;

    void handleNewReleases();
};
#endif // MAINWINDOW_H
