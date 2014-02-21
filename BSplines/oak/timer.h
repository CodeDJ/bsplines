
#ifndef OAK_TIMER_H
#define OAK_TIMER_H

#include <vector>
#include <functional>

namespace oak
{

class Timer
{
public:
    typedef std::function<void(Timer*)> TimeoutFunc;

public:
    Timer();
    Timer(int intervalMs, bool repeating = true, TimeoutFunc timeout = nullptr);
    ~Timer();

    Timer(const Timer&) = delete;
     Timer& operator=(const Timer&) = delete;

    void setIntervalMs(int intervalMs);
    int intervalMs() const;

    void setRepeating(bool repeating);
    bool isRepeating() const;

    void onTimeout(TimeoutFunc timeout);

    int timerId() const;

    void start();
    void stop();
    bool isActive() const;

public:
    static void singleShot(int intervalMs, TimeoutFunc timeout);
    static void defer(TimeoutFunc timeout);

private:
    int _id;
    int _intervalMs;
    bool _repeating;
    bool _active;
    bool _autoDelete;
    TimeoutFunc _timeout;

private:
    static void onTimeoutCb(int id);

private:
    //static int _firstUnusedId;
    static std::vector<Timer*> _timers;
};

} // namespace oak

#endif // OAK_TIMER_H
