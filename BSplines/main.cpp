
#include "applicationcontroller.h"

#include "oak/application.h"
#include "oak/log.h"

int main(int argc, char* argv[])
{
    //oak::setLogLevel(oak::LogInfo);
    //oak::logTo("bsplines.log");

    oak::Application app(&argc, argv);
    ApplicationController appController(&app);

    return app.exec();
}
