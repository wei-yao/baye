#include "global.h"

MainWindow* getGlobalWidget()
{
    static MainWindow globalWidget;
    return &globalWidget;
}
