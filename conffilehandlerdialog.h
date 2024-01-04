#ifndef CONFFILEHANDLERDIALOG_H
#define CONFFILEHANDLERDIALOG_H

#include <QDialog>
#include <QStringList>

class ConffileWidget;

namespace Ui {
class ConffileHandlerDialog;
}

class ConffileHandlerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConffileHandlerDialog(const QStringList &conffiles, QWidget *parent = nullptr);
    ~ConffileHandlerDialog();
    QStringList getReplaceConffileList();

private slots:
    void onDoneClicked();

private:
    Ui::ConffileHandlerDialog *ui;
    QList<ConffileWidget *> conffileList;
    QStringList replaceConffileList;
};

#endif // CONFFILEHANDLERDIALOG_H
