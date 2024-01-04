#ifndef DIFFDISPLAYDIALOG_H
#define DIFFDISPLAYDIALOG_H

#include <QDialog>

namespace Ui {
class DiffDisplayDialog;
}

class DiffDisplayDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DiffDisplayDialog(QString filename, QString diff, QWidget *parent = nullptr);
    ~DiffDisplayDialog();

private slots:
    void onDoneClicked();

private:
    Ui::DiffDisplayDialog *ui;
};

#endif // DIFFDISPLAYDIALOG_H
