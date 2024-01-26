#ifndef CONFFILEWIDGET_H
#define CONFFILEWIDGET_H

#include <QWidget>

namespace Ui {
class ConffileWidget;
}

class ConffileWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ConffileWidget(QString filename, QWidget *parent = nullptr);
    ~ConffileWidget();
    QString filename();
    bool doReplace();

private slots:
    void onDiffClicked();

private:
    Ui::ConffileWidget *ui;
    QString internalFilename;
};

#endif // CONFFILEWIDGET_H
