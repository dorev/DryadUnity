#pragma once

#include "dryad/types.h"
#include "dryad/descriptors.h"
#include "dryad/score/instant.h"

namespace Dryad
{

class Score
{
public:

    Instant* GetFirstUncommittedInstant()
    {
        if (_instants.empty())
            _firstUncommittedInstant = &(*_instants.emplace(_instants.end()));

        return _firstUncommittedInstant;
    }

    Result<> Commit(ScoreTime deltaScoreTime)
    {
        if (deltaScoreTime > 0)
        {
            Instant* instant = GetFirstUncommittedInstant();
            ScoreTime commitUntil = instant->GetScoreTime() + deltaScoreTime;

            do
            {
                instant->Commit();
                Instant* next = instant->GetNext();

                if (next == nullptr)
                {
                    SetFirstUncommittedInstant(nullptr);
                    return { ErrorCode::EndOfScore };
                }
            }

            while (instant->GetScoreTime() < commitUntil);

            SetFirstUncommittedInstant(instant);
        }

        return Success;
    }

    ScoreTime GetMeasureBeginningTime(const Instant* instant)
    {
        ScoreTime instantTime = instant->GetScoreTime();
        return instantTime - (instantTime % Music::Duration::Whole);
    }

    U32 GetMeasureNumber(const Instant* instant)
    {
        return static_cast<U32>(GetMeasureBeginningTime(instant) / Music::Duration::Whole);
    }

    Instant* GetFirstCommittedInstantFrom(ScoreTime time)
    {
        Instant* instant = GetFirstUncommittedInstant();

        if (time >= instant->GetScoreTime())
            return nullptr;

        for(;;)
        {
            Instant* previous = instant->GetPrev();

            if (previous == nullptr || previous->GetScoreTime() < time)
                break;

            if (previous->GetScoreTime() == time)
                return previous;

            instant = previous;
        }

        return instant;
    }

    Instant* GetFirstInstant()
    {
        if (_instants.empty())
            return nullptr;

        return &_instants.front();
    }

    Instant* GetLastInstant()
    {
        if (_instants.empty())
            return nullptr;

        return &_instants.back();
    }

private:

    void SetFirstUncommittedInstant(Instant* instant)
    {
        _firstUncommittedInstant = instant;
    }

    List<Instant> _instants;
    Instant* _firstUncommittedInstant;
};

} // namespace Dryad
