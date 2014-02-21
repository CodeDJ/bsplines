
#include "clock.h"

oak::Clock::Clock(const std::string& name /*= std::string()*/) :
    _name(name)
{
    reset();
}

void oak::Clock::reset()
{
    _start = hrc::now();
    _last = _start;
}

oak::Duration oak::Clock::elapsed() const
{
    auto now = hrc::now();
    Duration d(now - _start, now - _last);
    _last = now;
    return d;
}

std::ostream& operator<< (std::ostream& stream, const oak::Clock& clock)
{
    stream << clock._name << clock.elapsed();
    return stream;
}

oak::Duration::Duration(const hrc::duration& sinceStart, const hrc::duration& sinceLast) :
    _sinceStart(sinceStart),
    _sinceLast(sinceLast)
{
}

std::ostream& operator<< (std::ostream& stream, const oak::Duration& duration)
{
    stream << " T: " << std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(duration._sinceStart).count() << "ms \t+" << std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(duration._sinceLast).count() << "ms";
    return stream;
}
