#include "aptmanager.h"
#include "conffilehandlerdialog.h"
#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QSystemTrayIcon>
#include <QTextCursor>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    aptManager = new AptManager();

    // The progress bar and log view are shown after the user chooses to begin installing updates
    ui->progressBar->setVisible(false);
    ui->logView->setVisible(false);

    connect(ui->installButton, &QPushButton::clicked, this, &MainWindow::onInstallButtonClicked);
    connect(ui->closeButton, &QPushButton::clicked, this, &MainWindow::onCloseButtonClicked);
    connect(aptManager, &AptManager::updateComplete, this, &MainWindow::onUpdateCompleted);
    connect(aptManager, &AptManager::progressUpdated, this, &MainWindow::onProgressUpdate);
    connect(aptManager, &AptManager::logLineReady, this, &MainWindow::onLogLineReady);
    connect(aptManager, &AptManager::conffileListReady, this, &MainWindow::onConffileListReady);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setUpdateInfo(QList<QStringList> updateInfo)
{
    ui->packageView->clear();
    // The progress bar and log view are shown after the user chooses to begin installing updates
    ui->progressBar->setVisible(false);
    ui->logView->setVisible(false);
    ui->installButton->setEnabled(true);
    ui->detailsButton->setEnabled(true);
    ui->closeButton->setEnabled(true);

    for (int i = 0;i < 4;i++) {
        QTreeWidgetItem *installItem;
        switch (i) {
        case 0:
            installItem = new QTreeWidgetItem(QStringList() << "To be installed");
            break;
        case 1:
            installItem = new QTreeWidgetItem(QStringList() << "To be upgraded");
            break;
        case 2:
            installItem = new QTreeWidgetItem(QStringList() << "To be removed");
            break;
        case 3:
            installItem = new QTreeWidgetItem(QStringList() << "Held back (usually temporarily)");
            break;
        }

        for (int j = 0;j < updateInfo[i].count();j++) {
            // TODO: Add security update detection here - security updates should be marked in some way
            installItem->addChild(new QTreeWidgetItem(QStringList() << updateInfo[i][j]));
        }

        ui->packageView->addTopLevelItem(installItem);
    }
    ui->statLabel->setText(QString("%1 package(s) will be updated. %2 of these updates are security-related.")
      .arg(QString::number(updateInfo[0].count() + updateInfo[1].count() + updateInfo[2].count()),
      QString::number(updateInfo[4].count())));
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    /*
     * Don't allow the user to close the window with the close button if we've
     * disabled the normal "Close" button.
     */

    if (!ui->closeButton->isEnabled()) {
        event->ignore();
    }
}

void MainWindow::onInstallButtonClicked()
{
    ui->progressBar->setVisible(true);
    ui->logView->setVisible(true);
    ui->installButton->setEnabled(false);
    ui->detailsButton->setEnabled(false);
    ui->closeButton->setEnabled(false);
    aptManager->applyFullUpgrade();
}

void MainWindow::onCloseButtonClicked()
{
    hide();
}

void MainWindow::onUpdateCompleted()
{
    ui->closeButton->setEnabled(true);
    ui->progressBar->setVisible(false);
    ui->statLabel->setText("Update installation complete.");
    emit updatesInstalled(); // this tells the orchestrator to hide the tray icon
}

void MainWindow::onProgressUpdate(int progress)
{
    ui->progressBar->setValue(progress);
}

void MainWindow::onLogLineReady(QString logLine)
{
    ui->logView->textCursor().insertText(logLine);
    ui->logView->moveCursor(QTextCursor::End);
}

void MainWindow::onConffileListReady(QStringList conffileList)
{
    ConffileHandlerDialog chd(conffileList);
    chd.exec();
    for (QString single : chd.getReplaceConffileList()) {
        conffileList.removeAll(single);
        aptManager->replaceConffile(single);
    }
    for (QString single : conffileList) {
        aptManager->keepConffile(single);
    }
    aptManager->doneWithConffiles();
}
