#pragma once

#include "dryad/error.h"
#include "dryad/idprovider.h"
#include "dryad/midinote.h"
#include "dryad/result.h"
#include "dryad/types.h"
#include "dryad/utils.h"

#include "dryad/composer/composer.h"

#include "dryad/score/score.h"

namespace Dryad
{

class Session
{

public:

    Session(TimestampMs startTime = 0)
        : _startTimestamp(startTime)
        , _tempo(0)
        , _latestTimestamp(0)
        , _minPregeneratedNotesDuration(Constants::Duration::Whole * 2)
        , _minPregeneratedPhrases(2)
        , _score(*this)
        , _composer(_score)
    {
    }

    Result<> Update(TimestampMs currentTimestamp)
    {
        // Commit past notes
        if (currentTimestamp <= _latestTimestamp)
            return {ErrorCode::CannotCommitPastElements};

        _latestTimestamp = currentTimestamp;

        Result<> commitResult = _score.CommitPositionsUntil(_latestTimestamp);
        if(commitResult.HasError())
            return commitResult.GetError();

        // Do we have enough phrases progressions generated?
        U32 uncommittedPhraseCount = _score.UncommittedPhraseCount();
        U32 phraseDifference = _minPregeneratedPhrases - uncommittedPhraseCount;
        if (phraseDifference > 1)
        {
            Result<> generateProgResult = _composer.GeneratePhrases(phraseDifference);
            if(generateProgResult.HasError())
                return generateProgResult.GetError();
        }

        // Do we have enough notes generated?
        Position* position = _score.GetFirstUncommittedPosition();
        if(position == nullptr)
            return {ErrorCode::PositionDoesNotExist};

        ScoreTime scoreTime = position->GetScoreTime();

        return _composer.GenerateNotesUntil(scoreTime + _minPregeneratedNotesDuration);
    }

    Result<Vector<MidiNote>> FetchUpcomingNotes(TimestampMs currentTimestamp)
    {
        Result<> updateResult = Update(currentTimestamp);
        if(updateResult.HasError())
            return updateResult.GetError();

        Position* position = _score.GetFirstUncommittedPosition();
        if(position == nullptr)
            return {ErrorCode::PositionDoesNotExist};

        Result<> harmonizeResult = _composer.HarmonizeFrom(*position);
        if(harmonizeResult.HasError())
            return harmonizeResult.GetError();

        // Return all notes currently generated and harmonized
        Vector<MidiNote> result;

        while (position != nullptr)
        {
            List<Note>& notes = position->GetChildren();

            if (!notes.empty())
            {
                TimestampMs timestamp = ScoreTimeToTimestamp(
                    position->GetScoreTime(),
                    _tempo,
                    _startTimestamp);

                for(const Note& note : notes)
                    result.emplace_back(
                        note.midi,
                        note.duration,
                        timestamp);
            }

            position = position->Next();
        }

        if(result.empty())
            return {ErrorCode::NoUpcomingNotesAvailable};

        return result;
    }

    // Forwarding to composer

    Result<> RegisterMotif(const String& motifName, const Motif& motif)
    {
        return _composer.RegisterMotif(motifName, motif);
    }

    Result<> RegisterLandscapeGraph(const String& graphName, const LandscapeGraph& graph)
    {
        return _composer.RegisterLandscapeGraph(graphName, graph);
    }

    Result<> RemoveMotif(const String& motifName)
    {
        return _composer.RemoveMotif(motifName);
    }

    Result<> AddMotif(const String& motifName)
    {
         return _composer.AddMotif(motifName);
    }

    Score& GetScore() { return _score; }
    Session& GetSession() { return *this; }
    void SetTempo(U32 tempo) { _tempo = tempo; }
    U32 GetTempo() const { return _tempo; }

private:

    U32 _tempo;
    TimestampMs _startTimestamp;
    TimestampMs _latestTimestamp;
    IdProvider _idProvider;

    ScoreTime _minPregeneratedNotesDuration;
    U32 _minPregeneratedPhrases;


    Score _score;
    Composer _composer;
    Map<String, Motif> _motifs;
    Map<String, LandscapeGraph> _graphs;

    //void stop();
    //void setPhraseLength(uint phraseLength);
    //Error transitionToGraph(const String& name);
};

} // namespace Dryad