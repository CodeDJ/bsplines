
#ifndef OAK_APPLICATION_H
#define OAK_APPLICATION_H

#include <string>
#include <vector>

namespace oak
{

class Application
{
public:
    Application(int* argc, char** argv);
    ~Application();

    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

public:
    const std::vector<std::string>& args() const;
    std::string glRenderer() const;
    std::string glVersion() const;

public:
    int exec();
    void exit(int code = 0);

private:
    void initOpenGL(int* argc, char** argv);

private:
    std::vector<std::string> _args;
    int _exitCode;
};

}

#endif // OAK_APPLICATION_H
