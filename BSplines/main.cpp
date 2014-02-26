
#include "applicationcontroller.h"

#include "oak/application.h"
#include "oak/log.h"

int main(int argc, char* argv[])
{
    oak::setLogLevel(oak::LogInfo);
    //oak::logTo("bsplines.log");

    try
    {
        oak::Application app(&argc, argv);
        ApplicationController appController(&app);

        return app.exec();
    }
    catch(int exitCode)
    {
        return exitCode;
    }
    return -1;
}
