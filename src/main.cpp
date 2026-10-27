#include "App.h"

App& application()
{
    static App instance;
    return instance;
}

void setup()
{
    application().init();
}

void loop()
{
    application().update();
}