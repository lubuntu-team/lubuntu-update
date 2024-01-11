#include "ipcfilewatcher.h"

#include <QApplication>
#include <QDir>
#include <QFile>
#include <QFileSystemWatcher>

#include <QDebug>

IPCFileWatcher::IPCFileWatcher(QObject *parent)
    : QObject{parent}
{
    QDir targetDir("/dev/shm/lubuntu-update");
    bool couldRemove = targetDir.removeRecursively();
    if (!couldRemove) {
        qCritical() << "Could not clear IPC directory. Ensure that /dev/shm and /dev/shm/lubuntu-update are world-readable and world-writable.";
        initFailed = true;
        return;
    }
    targetDir.mkdir("/dev/shm/lubuntu-update");
    QFileSystemWatcher *watcher = new QFileSystemWatcher(QStringList() << "/dev/shm/lubuntu-update");
    connect(watcher, &QFileSystemWatcher::directoryChanged, this, &IPCFileWatcher::checkForShowWindowFile);
    initFailed = false;
}

bool IPCFileWatcher::didInitFail()
{
    return initFailed;
}

void IPCFileWatcher::checkForShowWindowFile()
{
    QFile flagFile("/dev/shm/lubuntu-update/lubuntu-update-show-win");
    if (flagFile.exists()) {
        flagFile.remove();
        emit showWindowFlagDetected();
    }
}
