#pragma once

#include "dryad/types.h"
#include "dryad/error.h"
#include "dryad/result.h"
#include "dryad/idprovider.h"


#include "dryad/score/score.h"
#include "dryad/composer/composer.h"

namespace Dryad
{

class Session
{

public:

    Session()
        : _tempo(0)
        , _startTimestamp(0)
        , _currentTimestamp(0)
        , _pregeneratedNotesBufferDuration(Constants::Duration::Whole * 2)
        , _pregeneratedPhrasesBufferCount(2)
        , _score(*this)
        , _composer(_score)
    {
    }

    Result<> update(TimestampMs currentTimestamp)
    {
        // commits past notes and generating more music if necessary

        if (currentTimestamp <= _currentTimestamp)
            return {ErrorCode::CannotCommitPastElements};

        Result<> commitResult = _score.commitPositionsUntil(currentTimestamp);
        if(commitResult.hasError())
            return commitResult.getError();

        // Do we have enough phrases progressions generated?
        if (_score.uncommittedPhraseCount() <= _pregeneratedPhrasesBufferCount)
        {
            _composer.generateNextPhraseProgression();
        }

        Position* lastUncommittedPosition = _score.lastUncommittedPosition();
        if(lastUncommittedPosition == nullptr)
            return {ErrorCode::PositionDoesNotExist};

        ScoreTime lastUncommittedScoreTime = lastUncommittedPosition->getScoreTime();

        // Do we have enough notes generated?
        Result<> notesAreWrittenResult = _composer.writeNotesUntil(lastUncommittedScoreTime + _pregeneratedNotesBufferDuration);
        if(notesAreWrittenResult.hasError())
            return notesAreWrittenResult.getError();

        return Success;
    }

    Result<Frame> fetchUpcomingNotes(TimestampMs currentTimestamp)
    {
        Result<> updateResult = update(currentTimestamp);
        if(updateResult.hasError())
            return updateResult.getError();

        Position* lastUncommittedPosition = _score.lastUncommittedPosition();
        if(lastUncommittedPosition == nullptr)
            return {ErrorCode::PositionDoesNotExist};

        Result<> harmonizeResult = _composer.harmonizeFrom(*lastUncommittedPosition);
        if(harmonizeResult.hasError())
            return harmonizeResult.getError();

        // get all notes currently generated and harmonized

        return Frame{};
    }

    void start(TimestampMs startTime)
    {
        _startTimestamp = startTime;
    }

    // Forwarding to composer

    Result<> registerMotif(const String& motifName, const Motif& motif)
    {
        return _composer.registerMotif(motifName, motif);
    }

    Result<> registerHarmonyGraph(const String& graphName, const HarmonyGraph& graph)
    {
        return _composer.registerHarmonyGraph(graphName, graph);
    }

    Result<> removeMotif(const String& motifName)
    {
        return _composer.removeMotif(motifName);
    }

    Result<> addMotif(const String& motifName)
    {
         return _composer.addMotif(motifName);
    }

    Score& getScore() { return _score; }
    Session& getSession() { return *this; }
    void setTempo(U32 tempo) { _tempo = tempo; }
    U32 getTempo() const { return _tempo; }

private:

    U32 _tempo;
    TimestampMs _startTimestamp;
    TimestampMs _currentTimestamp;
    IdProvider _idProvider;

    ScoreTime _pregeneratedNotesBufferDuration;
    U32 _pregeneratedPhrasesBufferCount;


    Score _score;
    Composer _composer;
    Map<String, Motif> _motifs;
    Map<String, HarmonyGraph> _graphs;

    //void stop();
    //void setPhraseLength(uint phraseLength);
    //Error transitionToGraph(const String& name);
};

} // namespace Dryad