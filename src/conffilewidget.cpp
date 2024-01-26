#include "conffilewidget.h"
#include "diffdisplaydialog.h"
#include "ui_conffilewidget.h"

#include <QProcess>

ConffileWidget::ConffileWidget(QString filename, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConffileWidget)
{
    ui->setupUi(this);
    internalFilename = filename;
    ui->filenameLabel->setText(filename);
    ui->keepRadioButton->setChecked(true);
    connect(ui->showDiffButton, &QPushButton::clicked, this, &ConffileWidget::onDiffClicked);
}

ConffileWidget::~ConffileWidget()
{
    delete ui;
}

QString ConffileWidget::filename()
{
    return internalFilename;
}

bool ConffileWidget::doReplace()
{
    return ui->replaceRadioButton->isChecked();
}

void ConffileWidget::onDiffClicked()
{
    QProcess diffProcess;
    diffProcess.setProgram("/usr/bin/diff");
    diffProcess.setArguments(QStringList() << "-u" << internalFilename << internalFilename + ".dpkg-dist");
    diffProcess.start();
    diffProcess.waitForFinished();
    QString result = diffProcess.readAllStandardOutput();
    DiffDisplayDialog ddw(internalFilename, result);
    ddw.exec();
}
