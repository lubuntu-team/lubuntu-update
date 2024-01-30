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

private slots:
    void onUpgradeClicked();
    void onRemindClicked();
    void onDeclineClicked();

private:
    Ui::ReleaseUpgradeWindow *ui;
};

#endif // RELEASEUPGRADEWINDOW_H
