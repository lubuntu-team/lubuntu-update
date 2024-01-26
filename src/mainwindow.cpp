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
    connect(ui->checkUpdatesButton, &QPushButton::clicked, this, &MainWindow::onCheckUpdatesButtonClicked);
    connect(ui->closeButton, &QPushButton::clicked, this, &MainWindow::onCloseButtonClicked);
    connect(aptManager, &AptManager::updateComplete, this, &MainWindow::onUpdateCompleted);
    connect(aptManager, &AptManager::checkUpdatesComplete, this, &MainWindow::onCheckUpdatesCompleted);
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
    ui->closeButton->setEnabled(true);
    ui->checkUpdatesButton->setEnabled(true);

    bool installEnabled = false

    for (int i = 0;i < 4;i++) {
        int count = updateInfo[i].count();

        if (count > 0) {
            installEnabled = true;
        } else { continue; }

        QTreeWidgetItem *installItem;
        switch (i) {
        case 0:
            installItem = new QTreeWidgetItem(QStringList() << tr("To be installed"));
            break;
        case 1:
            installItem = new QTreeWidgetItem(QStringList() << tr("To be upgraded"));
            break;
        case 2:
            installItem = new QTreeWidgetItem(QStringList() << tr("To be removed"));
            break;
        case 3:
            installItem = new QTreeWidgetItem(QStringList() << tr("Held back (usually temporarily)"));
            break;
        }

        for (int j = 0;j < count;j++) {
            // TODO: Add security update detection here - security updates should be marked in some way
            installItem->addChild(new QTreeWidgetItem(QStringList() << updateInfo[i][j]));
        }

        ui->packageView->addTopLevelItem(installItem);
    }
    ui->statLabel->setText(tr("%1 package(s) will be updated. %2 of these updates are security-related.")
      .arg(QString::number(updateInfo[0].count() + updateInfo[1].count() + updateInfo[2].count()),
      QString::number(updateInfo[4].count())));
    
    ui->installButton->setEnabled(installEnabled);
}

bool MainWindow::isLockedOpen()
{
    /*
     * If the Close button is disabled, we do NOT want the window to close
     * under virtually any circumstances. This allows the Orchestrator to
     * determine whether or not it is save to close and re-open the window
     * from the outside.
     */

    return !ui->closeButton->isEnabled();
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
    ui->logView->clear();
    ui->progressBar->setVisible(true);
    ui->logView->setVisible(true);
    ui->installButton->setEnabled(false);
    ui->checkUpdatesButton->setEnabled(false);
    ui->closeButton->setEnabled(false);
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(100);
    aptManager->applyFullUpgrade();
}

void MainWindow::onCheckUpdatesButtonClicked()
{
    ui->logView->clear();
    ui->progressBar->setVisible(true);
    ui->logView->setVisible(true);
    ui->installButton->setEnabled(false);
    ui->checkUpdatesButton->setEnabled(false);
    ui->closeButton->setEnabled(false);
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(0);
    aptManager->checkForUpdates();
}

void MainWindow::onCloseButtonClicked()
{
    hide();
}

void MainWindow::onUpdateCompleted()
{
    ui->closeButton->setEnabled(true);
    ui->checkUpdatesButton->setEnabled(true);
    ui->installButton->setEnabled(false);
    ui->progressBar->setVisible(false);
    ui->statLabel->setText(tr("Update installation complete."));
    emit updatesInstalled(); // this tells the orchestrator to hide the tray icon
}

void MainWindow::onCheckUpdatesCompleted()
{
    ui->closeButton->setEnabled(true); // unlocks the updater so that the orchestrator actually can update things
    emit updatesRefreshed(); // this tells the orchestrator to re-scan for updates
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
