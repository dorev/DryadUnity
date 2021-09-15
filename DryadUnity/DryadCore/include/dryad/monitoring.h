#pragma once

#include "utils.h"

namespace Dryad
{

template <class T>
struct MonitorCount : CrtpHelper<T, MonitorCount>
{
    MonitorCount()
    {
        _aliveCount++;
    }

    ~MonitorCount()
    {
        _aliveCount--;
    }

    static uint GetCount() { return _aliveCount; }

private:

    static uint _aliveCount;
};

template <typename T>
uint64_t MonitorCount<T>::_aliveCount = 0;

} // namespace Dryad