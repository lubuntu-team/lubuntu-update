#ifndef WINDOWSHOWWATCHER_H
#define WINDOWSHOWWATCHER_H

#include <QObject>
#include <QDBusAbstractAdaptor>
#include <QDBusVariant>

class WindowShowWatcher : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "me.lubuntu.LubuntuUpdate.window")

public:
    explicit WindowShowWatcher(QObject *obj) : QDBusAbstractAdaptor(obj) {}

public slots:
    void showWindow();

signals:
    void showWindowTriggered();
};

#endif // WINDOWSHOWWATCHER_H
