
#include "oak/application.h"
#include "applicationcontroller.h"

int main(int argc, char* argv[])
{
    oak::Application app(&argc, argv);
    ApplicationController appController(&app);

    return app.exec();
}
