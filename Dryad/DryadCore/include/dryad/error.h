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
    static constexpr U32 CategoryShift = sizeof(U32) / 2;

    static constexpr U32 GenericCategory = 0 << CategoryShift;
    static constexpr U32 NoError = GenericCategory | 0;
    static constexpr U32 Undefined = GenericCategory | 1;
    static constexpr U32 UselessCall = GenericCategory | 2;
    static constexpr U32 NullPointer = GenericCategory | 3;
    static constexpr U32 UnableToStoreData = GenericCategory | 4;
    static constexpr U32 UnexpectedNegativeNumber = GenericCategory | 5;
    
    static constexpr U32 ResultCategory = 1 << CategoryShift;
    static constexpr U32 UninitializedError = ResultCategory | 0;
    static constexpr U32 AsyncResultLocked = ResultCategory | 1;

    static constexpr U32 HarmonyCategory = 2 << CategoryShift;
    static constexpr U32 LandscapeDoesNotExist = HarmonyCategory | 0;
    static constexpr U32 LandscapeAlreadyExists = HarmonyCategory | 1;
    static constexpr U32 MotifDoesNotExist = HarmonyCategory | 2;
    static constexpr U32 MotifAlreadyExists = HarmonyCategory | 3;
    static constexpr U32 NoMotifAvailable = HarmonyCategory | 4;
    static constexpr U32 NoHarmonyNodeAvailable = HarmonyCategory | 5;
    static constexpr U32 MotifAlreadyFullyDeactivated = HarmonyCategory | 6;
    // static constexpr U32  = SessionCategory | ;

    static constexpr U32 ScoreCategory = 3 << CategoryShift;
    static constexpr U32 EndOfScore = ScoreCategory | 0;
    static constexpr U32 NothingToCommit = ScoreCategory | 1;
    static constexpr U32 CannotCommitPastElements = ScoreCategory | 2;
    static constexpr U32 NoUpcomingNotesAvailable = ScoreCategory | 3;
    //static constexpr U32   = ScoreCategory | ;


    static constexpr U32 TBD = 42 << CategoryShift;
    static constexpr U32 NoAvailableEquivalence = TBD | 0;
};


///////////////////////////////////////////////////////////////////////////////
// Error
///////////////////////////////////////////////////////////////////////////////

class Error
{

public:

    constexpr Error()
        : _code(ErrorCode::UninitializedError)
        , _description("Uninitialized error")
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
