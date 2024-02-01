#ifndef UPGRADEDELAYWINDOW_H
#define UPGRADEDELAYWINDOW_H

#include <QDialog>

namespace Ui {
class UpgradeDelayWindow;
}

class UpgradeDelayWindow : public QDialog
{
    Q_OBJECT

public:
    explicit UpgradeDelayWindow(QWidget *parent = nullptr);
    ~UpgradeDelayWindow();
    qint64 getDelayDays();

private slots:
    void onOkClicked();
    void onCancelClicked();

private:
    Ui::UpgradeDelayWindow *ui;
    qint64 delayDays;
};

#endif // UPGRADEDELAYWINDOW_H
