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

signals:
    void updatesInstalled();

protected slots:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void onInstallButtonClicked();
    void onCloseButtonClicked();
    void onUpdateCompleted();
    void onProgressUpdate(int progress);
    void onLogLineReady(QString logLine);
    void onConffileListReady(QStringList conffileList);

private:
    Ui::MainWindow *ui;
    AptManager *aptManager;
};
#endif // MAINWINDOW_H
