#ifndef SHAREDVALUE_H
#define SHAREDVALUE_H

#include <functional>
#include <memory>

template<typename T>
class SharedValue
{
public:
    SharedValue(const T& value, const std::function<void(const T&)>& deleter)
        : _id(std::make_shared<T>(value)),
          _deleter(deleter)
    {}

    SharedValue operator=(const T& value)
    {
        *_id = value;
        return *this;
    }

    operator const T&() const
    {
        return *_id;
    }

    ~SharedValue()
    {
        if (_id.unique())
        {
            _deleter(*_id);
            *_id = 0;
        }
    }

    void reset(const T& value)
    {
        _deleter(*_id);
        *_id = value;
    }

private:
    const std::shared_ptr<T> _id;
    const std::function<void(const T&)>& _deleter;
};

#endif // SHAREDVALUE_H
