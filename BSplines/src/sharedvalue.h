#ifndef SHAREDVALUE_H
#define SHAREDVALUE_H

#include <functional>
#include <memory>

template<typename T>
class SharedValue
{
public:
    SharedValue(const T& value, const std::function<void(T&)>& finalizer)
        : _value(std::make_shared<T>(value)),
          _finalizer(finalizer)
    { }

    SharedValue& operator=(const SharedValue&) = default;

    SharedValue& operator=(const T& value)
    {
        *_value = value;
        return *this;
    }

    operator const T&() const
    {
        return *_value;
    }

    ~SharedValue()
    {
        if (_value.unique())
        {
            _finalizer(*_value);
            *_value = 0;
        }
    }

    void reset(const T& value)
    {
        _finalizer(*_value);
        *_value = value;
    }

private:
    const std::shared_ptr<T> _value;
    const std::function<void(T&)> _finalizer;
};

#endif // SHAREDVALUE_H
