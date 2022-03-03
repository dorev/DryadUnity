#pragma once

#include "dryad/types.h"

namespace Dryad
{

///////////////////////////////////////////////////////////////////////////////
// Error codes
///////////////////////////////////////////////////////////////////////////////

class ErrorCode
{
public:
    using Type = U32;
    static constexpr U32 CategoryShift = sizeof(Type) / 2;

    static constexpr Type GenericCategory = 0 << CategoryShift;
    static constexpr Type NoError = GenericCategory | 0;
    static constexpr Type Undefined = GenericCategory | 1;
    static constexpr Type UselessCall = GenericCategory | 2;
    static constexpr Type NullPointer = GenericCategory | 3;

    static constexpr Type ResultCategory = 1 << CategoryShift;
    static constexpr Type UninitializedError = ResultCategory | 0;
    static constexpr Type AsyncResultLocked = ResultCategory | 1;

    static constexpr Type SessionCategory = 2 << CategoryShift;
    static constexpr Type LandscapeDoesNotExist = SessionCategory | 0;
    static constexpr Type LandscapeAlreadyExists = SessionCategory | 1;
    static constexpr Type MotifDoesNotExist = SessionCategory | 2;
    static constexpr Type MotifAlreadyExists = SessionCategory | 3;
    static constexpr Type CannotCommitPastElements = SessionCategory | 4;
    static constexpr Type NoMotifAvailable = SessionCategory | 5;
    static constexpr Type NoHarmonyNodeAvailable = SessionCategory | 6;
    static constexpr Type MotifAlreadyFullyDeactivated = SessionCategory | 7;
    static constexpr Type NoUpcomingNotesAvailable = SessionCategory | 8;
    static constexpr Type NothingToCommit = SessionCategory | 9;

    static constexpr Type ScoreCategory = 3 << CategoryShift;
    static constexpr Type PhraseDoesNotExist = ScoreCategory | 0;
    static constexpr Type MeasureDoesNotExist = ScoreCategory | 1;
    static constexpr Type PositionDoesNotExist = ScoreCategory | 2;
    static constexpr Type NoteDoesNotExist = ScoreCategory | 3;
    static constexpr Type CannotWritePastElements = ScoreCategory | 4;
    

    static constexpr Type TBD = 42 << 32;
    static constexpr Type NoAvailableEquivalence = TBD | 0;
};


///////////////////////////////////////////////////////////////////////////////
// Error
///////////////////////////////////////////////////////////////////////////////

class Error
{

public:

    constexpr Error()
        : _code(ErrorCode::UninitializedError)
        , _description("Uninitialized Error")
    {
    }

    constexpr Error(U64 code, const char* description)
        : _code(code)
        , _description(description)
    {
    }

    constexpr Error(U64 code)
        : _code(code)
        , _description(nullptr)
    {
    }

    constexpr bool operator==(const Error& other) const
    {
        if(_code != other._code)
            return false;

        if(_description == nullptr)
            return other._description == nullptr;
        
        if(other._description == nullptr)
            return false;

        return strcmp(_description, other._description) == 0;
    }

    constexpr U64 GetCode() const
    {
        return _code;
    }

    constexpr const char* GetDescription() const
    {
        return _description;
    }

    constexpr explicit operator bool() const
    {
        return _code != ErrorCode::NoError;
    }

private:

    const char* _description;
    U64 _code;
};


} // namespace Utils
