#include "conffilehandlerdialog.h"
#include "conffilewidget.h"
#include "ui_conffilehandlerdialog.h"

ConffileHandlerDialog::ConffileHandlerDialog(const QStringList &conffiles, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConffileHandlerDialog)
{
    ui->setupUi(this);
    for (QString conffile : conffiles) {
        ConffileWidget *conffileWidget = new ConffileWidget(conffile);
        conffileList.append(conffileWidget);
        ui->conffileStack->insertWidget(ui->conffileStack->count() - 1, conffileWidget);
    }

    connect(ui->doneButton, &QPushButton::clicked, this, &ConffileHandlerDialog::onDoneClicked);
}

ConffileHandlerDialog::~ConffileHandlerDialog()
{
    delete ui;
}

QStringList ConffileHandlerDialog::getReplaceConffileList()
{
    return replaceConffileList;
}

void ConffileHandlerDialog::onDoneClicked()
{
    for (ConffileWidget *w : conffileList) {
        if (w->doReplace()) {
            replaceConffileList.append(w->filename());
        }
    }
    this->done(0);
}
