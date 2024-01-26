#include "diffdisplaydialog.h"
#include "ui_diffdisplaydialog.h"

DiffDisplayDialog::DiffDisplayDialog(QString filename, QString diff, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DiffDisplayDialog)
{
    ui->setupUi(this);
    this->setWindowTitle(filename + " diff");
    ui->diffView->setPlainText(diff);
    connect(ui->doneButton, &QPushButton::clicked, this, &DiffDisplayDialog::onDoneClicked);
}

DiffDisplayDialog::~DiffDisplayDialog()
{
    delete ui;
}

void DiffDisplayDialog::onDoneClicked()
{
    this->done(0);
}
