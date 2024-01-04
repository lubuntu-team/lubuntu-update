#include "orchestrator.h"
#include "mainwindow.h"
#include "conffilehandlerdialog.h"

#include <QApplication>
#include <QDialog>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "lubuntu-update_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    // Don't want the updater to stop just because the user closed it :P
    a.setQuitOnLastWindowClosed(false);

    /*
     * As this is a background process, we don't pop up any window upon
     * startup. An Orchestrator object periodically checks to see if new
     * updates have been detected, and offers them to the user (by displaying
     * a tray icon) if so. The user can click on this tray icon to see the
     * updater window.
     *
     * Orchestrator's constructor automatically starts the update checker, so
     * there's no need to do anything with this except create it and then
     * start the event loop.
     */

    Orchestrator *o = new Orchestrator();

    /*
     * This is an artifact from testing the conffile handler window. You can
     * uncomment this and rebuild lubuntu-update in order to test the conffile
     * handler UI and develop it further.
     *
     * ConffileHandlerDialog cfhd(QStringList() << "/home/user/testfile");
     * cfhd.show();
     */

    return a.exec();
}
