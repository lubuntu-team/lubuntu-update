#include "aptmanager.h"

#include <QFile>
#include <QProcess>
#include <QProgressBar>
#include <QRegularExpression>
#include <QPlainTextEdit>
#include <QSystemTrayIcon>
#include <QTextStream>
#include <QThread>

AptManager::AptManager(QObject *parent)
    : QObject(parent)
{
}

void AptManager::applyFullUpgrade()
{
    internalUpdateProgress = 0;
    internalUpdateInfo = getUpdateInfo();

    // these six vars are used to track how far along things are for updating the progress bar
    instUnpackList = instConfigList = internalUpdateInfo[0];
    upgradeUnpackList = upgradeConfigList = internalUpdateInfo[1];
    removeList = internalUpdateInfo[2];
    numPackagesToPrep = instUnpackList.count() + upgradeUnpackList.count();

    aptProcess = new QProcess();
    aptProcess->setProgram("/usr/bin/lxqt-sudo");
    // Note that the lubuntu-update-backend script sets LC_ALL=C in it already, so we don't need to add that here.
    aptProcess->setArguments(QStringList() << "/usr/libexec/lubuntu-update-backend" << "doupdate");
    aptProcess->setProcessChannelMode(QProcess::MergedChannels);
    QObject::connect(aptProcess, &QProcess::readyRead, this, &AptManager::handleProcessBuffer);
    QObject::connect(aptProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &AptManager::handleProcessBuffer);
    aptProcess->start();
}

void AptManager::keepConffile(QString conffile)
{
    aptProcess->write("keep\n");
    aptProcess->write(conffile.toUtf8() + "\n");
    aptProcess->waitForBytesWritten();
}

void AptManager::replaceConffile(QString conffile)
{
    aptProcess->write("replace\n");
    aptProcess->write(conffile.toUtf8() + "\n");
    aptProcess->waitForBytesWritten();
}

void AptManager::doneWithConffiles()
{
    aptProcess->write("done\n");
    aptProcess->waitForBytesWritten();
    aptProcess->closeWriteChannel();
}

void AptManager::handleProcessBuffer()
{
    int maxWaitTime = 20;
    while (!aptProcess->canReadLine() && maxWaitTime > 0) {
        // this is horrible why doesn't QProcess have canReadLine signal
        QThread::msleep(20);
        maxWaitTime--;
    }

    QRegularExpression dlRegex("^Get:\\d+");
    char lineBuf[2048];

    while(aptProcess->canReadLine()) {
        aptProcess->readLine(lineBuf, 2048);
        QString line = QString(lineBuf);
        emit logLineReady(line); // this tells the main window to print the line to the log view
        QRegularExpressionMatch dlLineMatch = dlRegex.match(line);

        // yes, this gave me a headache also
        if (dlLineMatch.hasMatch()) { // Increments the progress counter for each package downloaded
            internalUpdateProgress++;
        } else if (line.count() >= 25 && line.left(24) == "Preparing to unpack .../" && numPackagesToPrep != 0) {
            internalUpdateProgress++; // Increments the progress counter for each package that is "prepared to unpack"
            numPackagesToPrep--;
        } else if (line.count() >= 10 && line.left(9) == "Unpacking") {
            /*
             * Increments the progress counter for each package that is unpacked
             * The package name may be suffixed with ":amd64" or some other
             * :architecture string, so we have to split that off
             * This and the subsequent progress updaters are complex since
             * we don't want any line that starts with "Unpacking" or some
             * other simple string to cause a progress increment. What if a
             * maintainer script says it's unpacking something?
             */
            QStringList parts = line.split(' ');
            QString packageName;
            if (parts.count() >= 2) {
                packageName = parts[1].split(':')[0]; // strip off a trailing :amd64 if it's there
                if (instUnpackList.removeAll(packageName) || upgradeUnpackList.removeAll(packageName)) {
                    internalUpdateProgress++;
                }
            }
        } else if (line.count() >= 11 && line.left(10) == "Setting up") {
            QStringList parts = line.split(' ');
            QString packageName;
            if (parts.count() >= 3) {
                packageName = parts[2].split(':')[0];
                if (instConfigList.removeAll(packageName) || upgradeConfigList.removeAll(packageName)) {
                    internalUpdateProgress++;
                }
            }
        } else if (line.count() >= 9 && line.left(8) == "Removing") {
            QStringList parts = line.split(' ');
            QString packageName;
            if (parts.count() >= 2) {
                packageName = parts[1].split(':')[0];
                if (removeList.removeAll(packageName)) {
                    internalUpdateProgress++;
                }
            }
        } else if (line == "Lubuntu Update !!! CONFIGURATION FILE LIST START\r\n") {
            // oh boy, conffile processing. Stop everything and get the full list of conffiles, busy-waiting as needed.
            QStringList conffileList;
            while (true) {
                while (!aptProcess->canReadLine()) {
                    QThread::msleep(20);
                }
                aptProcess->readLine(lineBuf, 2048);
                QString confLine = QString(lineBuf);
                confLine = confLine.left(confLine.count() - 2);
                if (confLine == "Lubuntu Update !!! CONFIGURATION FILE LIST END") {
                    emit conffileListReady(conffileList); // this triggers the main window to show the conffile handler window
                    break;
                } else {
                    conffileList.append(confLine);
                }
            }
        }

        double percentageDone = (static_cast<double>(internalUpdateProgress) / (((internalUpdateInfo[0].count() + internalUpdateInfo[1].count()) * 4) + internalUpdateInfo[2].count())) * 100;
        if (percentageDone > 100.0) {
            percentageDone = 100.0;
        }
        emit progressUpdated(static_cast<int>(percentageDone));
    }

    if (aptProcess->state() == QProcess::NotRunning) {
        emit progressUpdated(100); // just in case the progress bar didn't fill all the way due to a previous partial download
        emit updateComplete();
        aptProcess->deleteLater();
    }
}

QList<QStringList> AptManager::getUpdateInfo()
{
    /*
     * We use `apt-get -s full-upgrade` here rather than `apt list
     * --upgradable` because it tells us not only what packages need upgraded,
     * but also which packages need installed and removed, as well as which
     * packages have been held back. The only thing this doesn't tell us is
     * which packages are security updates, and for that we do use `apt list
     * --upgradable`.
     */

    QList<QStringList> output;

    QProcess *checker = new QProcess();
    QProcessEnvironment checkerEnv;
    checkerEnv.insert("LC_ALL", "C");
    checker->setProcessEnvironment(checkerEnv);
    checker->setProgram("/usr/bin/apt-get");
    checker->setArguments(QStringList() << "-s" << "full-upgrade");
    checker->start();

    if (checker->waitForFinished(60000)) {
        QString stdoutString = QString(checker->readAllStandardOutput());
        QTextStream stdoutStream(&stdoutString);
        QString stdoutLine;

        /*
         * output[0] = packages to install
         * output[1] = packages to upgrade
         * output[2] = packages to remove
         * output[3] = held packages
         * output[5] = security-related updates, this is populated at the end
         */

        for (int i = 0; i < 4;i++) {
            output.append(QStringList());
        }

        bool gettingInstallPackages = false;
        bool gettingUpgradePackages = false;
        bool gettingUninstallPackages = false;
        bool gettingHeldPackages = false;
        bool gettingPackageList = false;

        while (stdoutStream.readLineInto(&stdoutLine)) {
            if (!gettingPackageList) {
                parseAptLine(stdoutLine, &gettingInstallPackages, &gettingUpgradePackages, &gettingUninstallPackages, &gettingHeldPackages, &gettingPackageList);
            } else {
                /*
                 * Each line of output apt-get gives when displaying package
                 * lists is intended by two spaces. A package name will always
                 * consist of at least one character, so if there are less
                 * than three characters or the line isn't indented with two
                 * spaces, we know we're no longer reading a package list.
                 */

                if (stdoutLine.count() < 3 || stdoutLine.left(2) != "  ") {
                    gettingInstallPackages = false;
                    gettingUpgradePackages = false;
                    gettingUninstallPackages = false;
                    gettingHeldPackages = false;
                    gettingPackageList = false;

                    parseAptLine(stdoutLine, &gettingInstallPackages, &gettingUpgradePackages, &gettingUninstallPackages, &gettingHeldPackages, &gettingPackageList);
                } else {
                    QString packageLine = stdoutLine.trimmed();
                    QStringList packageNames = packageLine.split(' ');

                    if (gettingInstallPackages) {
                        for (int i = 0;i < packageNames.count();i++) {
                            output[0].append(packageNames[i]);
                        }
                    } else if (gettingUpgradePackages) {
                        for (int i = 0;i < packageNames.count();i++) {
                            output[1].append(packageNames[i]);
                        }
                    } else if (gettingUninstallPackages) {
                        for (int i = 0;i < packageNames.count();i++) {
                            output[2].append(packageNames[i]);
                        }
                    } else if (gettingHeldPackages) {
                        for (int i = 0;i < packageNames.count();i++) {
                            output[3].append(packageNames[i]);
                        }
                    }
                }
            }
        }

        output.append(getSecurityUpdateList());
    }
    checker->terminate();
    checker->deleteLater();
    return output;
}

void AptManager::parseAptLine(QString line, bool *gettingInstallPackages, bool *gettingUpgradePackages, bool *gettingUninstallPackages, bool *gettingHeldPackages, bool *gettingPackageList)
{
    *gettingPackageList = true;

    if (line == "The following NEW packages will be installed:") {
        *gettingInstallPackages = true;
    } else if (line == "The following packages will be upgraded:") {
        *gettingUpgradePackages = true;
    } else if (line == "The following packages will be REMOVED:") {
        *gettingUninstallPackages = true;
    } else if (line == "The following packages have been kept back:") {
        *gettingHeldPackages = true;
    } else {
        *gettingPackageList = false;
    }
}

QStringList AptManager::getSecurityUpdateList()
{
    QStringList updateList;
    QString distroName;

    // Find the distro name
    QFile lsbReleaseFile("/etc/lsb-release");
    lsbReleaseFile.open(QIODevice::ReadOnly);
    QString contents = QString(lsbReleaseFile.readAll());
    lsbReleaseFile.close();
    QTextStream distroFinder(&contents);
    QString distroLine;
    while (distroFinder.readLineInto(&distroLine)) {
        // The line has to be at least 18 characters long - 16 for the string "DISTRIB_CODENAME", one for the = sign, and one for a codename with a length of at least one.
        if (distroLine.count() >= 18 && distroLine.left(16) == "DISTRIB_CODENAME") {
            QStringList distroParts = distroLine.split('=');
            if (distroParts.count() >= 2) {
                distroName = distroParts[1];
            }
        }
    }

    // Now check for security updates
    QProcess *checker = new QProcess();
    QProcessEnvironment checkerEnv;
    checkerEnv.insert("LC_ALL", "C");
    checker->setProcessEnvironment(checkerEnv);
    checker->setProgram("/usr/bin/apt");
    checker->setArguments(QStringList() << "list" << "--upgradable");
    checker->start();

    if (checker->waitForFinished(60000)) {
        QRegularExpression regex(QString("%1-security").arg(distroName));
        QString stdoutString = checker->readAllStandardOutput();
        QTextStream stdoutStream(&stdoutString);
        QString stdoutLine;

        while (stdoutStream.readLineInto(&stdoutLine)) {
            QRegularExpressionMatch match = regex.match(stdoutLine);
            if (match.hasMatch()) {
                // The package name we want is followed immediately by a single forward slash, so we can use that as our delimiter.
                QStringList updateParts = stdoutLine.split('/');
                updateList.append(updateParts[0]);
            }
        }
    }

    checker->terminate();
    checker->deleteLater();
    return updateList;
}
