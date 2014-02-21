
#ifndef OAK_CLOCK_H
#define OAK_CLOCK_H

#include <string>
#include <chrono>
#include <iostream>

namespace oak
{
class Clock;
class Duration;
}
std::ostream& operator<< (std::ostream& stream, const oak::Clock& clock);
std::ostream& operator<< (std::ostream& stream, const oak::Duration& clock);

namespace oak
{

class Clock
{
public:
    explicit Clock(const std::string& name = std::string());

public:
    void reset();

    Duration elapsed() const;

public:
    friend std::ostream& ::operator<< (std::ostream& stream, const Clock& clock);

private:
    std::string _name;
    typedef std::chrono::high_resolution_clock hrc;
    hrc::time_point _start;
    mutable hrc::time_point _last;
};

class Duration
{
public:
    Duration() = delete;

private:
    typedef std::chrono::high_resolution_clock hrc;
    Duration(const hrc::duration& sinceStart, const hrc::duration& sinceLast);

public:
    friend class Clock;
    friend std::ostream& ::operator<< (std::ostream& stream, const Duration& duration);

private:
    hrc::duration _sinceStart;
    hrc::duration _sinceLast;
};

}

#endif // OAK_CLOCK_H
