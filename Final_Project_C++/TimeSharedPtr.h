#ifndef TIMED_SHARED_PTR_H
#define TIMED_SHARED_PTR_H

#include <iostream>
#include <cstddef>
#include <chrono>
#include <utility>
#include <thread>
#include <atomic>

using Clock = std::chrono::high_resolution_clock;
typedef std::chrono::time_point<Clock> clockTime;
typedef std::chrono::milliseconds milliSeconds;

struct ControlTimedSharedPtr {
    void *          ptr;
    std::atomic<long> myUseCount;
    clockTime       TimedSharedPtrStartTime;
    milliSeconds    deletePtrInMs;

    ControlTimedSharedPtr(void * p, long deleteMeInMilliseconds) : ptr(p), 
                                                        myUseCount(1),
                                                        TimedSharedPtrStartTime(Clock::now()),
                                                        deletePtrInMs{milliSeconds(deleteMeInMilliseconds)} 
    { 
        std::cout << "ControlTimedSharedPtr " << ptr
                  << " start: 0 ms " << std::endl;
    }

    ~ControlTimedSharedPtr() {
        std::cout << "ControlTimedSharedPtr " << ptr
                  << " end: " 
                  << std::chrono::duration_cast<milliSeconds>(
                        Clock::now().time_since_epoch() - TimedSharedPtrStartTime.time_since_epoch()
                     ).count() 
                  << " ms " << std::endl;
        delete static_cast<char*>(ptr); // assuming dynamic memory allocated with new
    }
};

template<typename T>
class TimedSharedPtr {
private:
    ControlTimedSharedPtr* _ptrToControl;

public:
    TimedSharedPtr() : _ptrToControl(nullptr) {}

    TimedSharedPtr(T* raw, long deleteMeInMilliseconds) 
      : _ptrToControl(new ControlTimedSharedPtr(static_cast<void*>(raw), deleteMeInMilliseconds)) {}

    TimedSharedPtr(T* raw) 
      : _ptrToControl(new ControlTimedSharedPtr(static_cast<void*>(raw), 1000)) {}

    TimedSharedPtr(const TimedSharedPtr& other) 
      : _ptrToControl(other._ptrToControl) {
        if (_ptrToControl) {
            ++_ptrToControl->myUseCount;
        }
    }

    TimedSharedPtr& operator=(const TimedSharedPtr& other) {
        if (this != &other) {
            release();
            _ptrToControl = other._ptrToControl;
            if (_ptrToControl) {
                ++_ptrToControl->myUseCount;
            }
        }
        return *this;
    }

    ~TimedSharedPtr() {
        release();
    }

    void release() {
        if (_ptrToControl) {
            if (--_ptrToControl->myUseCount == 0) {
                delete _ptrToControl;
            }
            _ptrToControl = nullptr;
        }
    }

    long use_count() const noexcept {
        return _ptrToControl ? _ptrToControl->myUseCount.load() : 0;
    }

    T* get() const {
        if (!_ptrToControl) return nullptr;

        auto timeElapsed = Clock::now().time_since_epoch() - _ptrToControl->TimedSharedPtrStartTime.time_since_epoch();
        if (timeElapsed > _ptrToControl->deletePtrInMs) {
            std::cout << "Yeo! Expired ";
            return nullptr;
        }
        return static_cast<T*>(_ptrToControl->ptr);
    }
};

#endif
