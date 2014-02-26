
#include "timer.h"

#ifdef Q_OS_MAC
#include <GLUT/glut.h>
#endif

#ifdef Q_OS_WIN
#include <GL/freeglut.h>
#endif

#include <assert.h>

//int oak::Timer::_firstUnusedId = 0;
std::vector<oak::Timer*> oak::Timer::_timers;

oak::Timer::Timer() :
    Timer(0, false, nullptr)
{
}

oak::Timer::Timer(int intervalMs, bool repeating /*= true*/, TimeoutFunc timeout /*= nullptr*/) :
    _id(-1),
    _intervalMs(intervalMs),
    _repeating(repeating),
    _active(false),
    _autoDelete(false),
    _timeout(timeout)
{
    _id = _timers.size();
    _timers.push_back(this);
}

oak::Timer::~Timer()
{
    _timers[_id] = nullptr;
    _id = -1;
}

void oak::Timer::setIntervalMs(int intervalMs)
{
    _intervalMs = intervalMs;
}

int oak::Timer::intervalMs() const
{
    return _intervalMs;
}

void oak::Timer::setRepeating(bool repeating)
{
    _repeating = repeating;
}

bool oak::Timer::isRepeating() const
{
    return _repeating;
}

void oak::Timer::onTimeout(TimeoutFunc timeout)
{
    _timeout = timeout;
}

int oak::Timer::timerId() const
{
    return _id;
}

void oak::Timer::start()
{
    _active = true;
    glutTimerFunc(_intervalMs, onTimeoutCb, _id);
}

void oak::Timer::stop()
{
    _active = false;
}

bool oak::Timer::isActive() const
{
    return _active;
}

void oak::Timer::singleShot(int intervalMs, TimeoutFunc timeout)
{
    Timer* timer = new Timer(intervalMs, false, timeout);
    timer->_autoDelete = true;
    timer->start();
}

void oak::Timer::defer(TimeoutFunc timeout)
{
    singleShot(0, timeout);
}

void oak::Timer::onTimeoutCb(int id)
{
    assert(id >= 0 && id < static_cast<int>(_timers.size()));
    Timer* timer = _timers[id];
    if (timer != nullptr && timer->_active)
    {
        timer->_timeout(timer);
        if (timer->_repeating)
        {
            glutTimerFunc(timer->_intervalMs, onTimeoutCb, id);
        }
        else
        {
            timer->_active = false;
            if (timer->_autoDelete)
            {
                delete timer;
                timer = nullptr;
            }
        }
    }
}
