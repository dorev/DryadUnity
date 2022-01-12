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

    static constexpr U64 GenericCategory = 0ULL << 32;
    static constexpr U64 NoError = GenericCategory | 0;
    static constexpr U64 Undefined = GenericCategory | 1;
    static constexpr U64 UselessCall = GenericCategory | 2;
    static constexpr U64 NullPointer = GenericCategory | 3;

    static constexpr U64 ResultCategory = 1ULL << 32;
    static constexpr U64 UninitializedError = ResultCategory | 0;
    static constexpr U64 AsyncResultLocked = ResultCategory | 1;

    static constexpr U64 SessionCategory = 2ULL << 32;
    static constexpr U64 GraphDoesNotExist = SessionCategory | 0;
    static constexpr U64 GraphAlreadyExists = SessionCategory | 1;
    static constexpr U64 MotifDoesNotExist = SessionCategory | 2;
    static constexpr U64 MotifAlreadyExists = SessionCategory | 3;
    static constexpr U64 CannotCommitPastElements = SessionCategory | 4;
    static constexpr U64 NoMotifAvailable = SessionCategory | 5;
    static constexpr U64 NoHarmonyNodeAvailable = SessionCategory | 6;
    static constexpr U64 MotifAlreadyFullyDeactivated = SessionCategory | 7;
    static constexpr U64 NoUpcomingNotesAvailable = SessionCategory | 8;

    static constexpr U64 ScoreCategory = 3ULL << 32;
    static constexpr U64 PhraseDoesNotExist = ScoreCategory | 0;
    static constexpr U64 MeasureDoesNotExist = ScoreCategory | 1;
    static constexpr U64 PositionDoesNotExist = ScoreCategory | 2;
    static constexpr U64 NoteDoesNotExist = ScoreCategory | 3;
    static constexpr U64 CannotWritePastElements = ScoreCategory | 4;

    static constexpr U64 TBD = 42ULL << 32;
    static constexpr U64 NoAvailableEquivalence = TBD | 0;
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

    constexpr U64 getCode() const
    {
        return _code;
    }

    constexpr const char* getDescription() const
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
