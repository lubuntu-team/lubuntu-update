#include "windowshowwatcher.h"

void WindowShowWatcher::showWindow()
{
    emit showWindowTriggered();
}
