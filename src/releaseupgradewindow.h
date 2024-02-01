#ifndef RELEASEUPGRADEWINDOW_H
#define RELEASEUPGRADEWINDOW_H

#include <QDialog>

namespace Ui {
class ReleaseUpgradeWindow;
}

class ReleaseUpgradeWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ReleaseUpgradeWindow(QString releaseCode, QWidget *parent = nullptr);
    ~ReleaseUpgradeWindow();
    bool getUpgradeAccepted();
    qint64 getUpgradeDelayStamp();

private slots:
    void onUpgradeClicked();
    void onRemindClicked();
    void onDeclineClicked();

private:
    Ui::ReleaseUpgradeWindow *ui;
    QString code;
    bool upgradeAccepted = false;
    qint64 upgradeDelayStamp = 0;
};

#endif // RELEASEUPGRADEWINDOW_H
