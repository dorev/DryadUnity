#pragma once

#include "dryad/types.h"
#include "dryad/constants.h"

namespace Dryad
{

class Note
{
public:

    Note()
        : _value(0)
    {
    }

    Note(U32 value)
        : _value(value)
    {
    }

    const char* GetName()
    {
        return Music::Note::GetName(_value);
    }

    U32 GetNumber()
    {
        return _value;
    }

    U32 GetOctave()
    {
        return _value / 12;
    }

    U32 GetBaseNote()
    {
        return _value % 12;
    }

    bool operator==(const Note& other) const
    {
        return _value == other._value;
    }

private:
    U32 _value;
};

} // namespace Dryad
