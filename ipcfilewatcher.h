#ifndef IPCFILEWATCHER_H
#define IPCFILEWATCHER_H

#include <QObject>

class IPCFileWatcher : public QObject
{
    Q_OBJECT
public:
    explicit IPCFileWatcher(QObject *parent = nullptr);
    bool didInitFail();

signals:
    void showWindowFlagDetected();

private:
    bool initFailed;

    void checkForShowWindowFile();
};

#endif // IPCFILEWATCHER_H
