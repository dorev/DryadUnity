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

    Result<> update(TimestampMs currentTimestamp)
    {
        // Commit past notes
        if (currentTimestamp <= _latestTimestamp)
            return {ErrorCode::CannotCommitPastElements};

        _latestTimestamp = currentTimestamp;

        Result<> commitResult = _score.commitPositionsUntil(_latestTimestamp);
        if(commitResult.hasError())
            return commitResult.getError();

        // Do we have enough phrases progressions generated?
        U32 uncommittedPhraseCount = _score.uncommittedPhraseCount();
        U32 phraseDifference = _minPregeneratedPhrases - uncommittedPhraseCount;
        if (phraseDifference > 1)
        {
            Result<> generateProgResult = _composer.generatePhrases(phraseDifference);
            if(generateProgResult.hasError())
                return generateProgResult.getError();
        }

        // Do we have enough notes generated?
        Position* position = _score.lastUncommittedPosition();
        if(position == nullptr)
            return {ErrorCode::PositionDoesNotExist};

        ScoreTime scoreTime = position->getScoreTime();

        return _composer.writeNotesUntil(scoreTime + _minPregeneratedNotesDuration);
    }

    Result<Vector<MidiNote>> fetchUpcomingNotes(TimestampMs currentTimestamp)
    {
        Result<> updateResult = update(currentTimestamp);
        if(updateResult.hasError())
            return updateResult.getError();

        Position* position = _score.lastUncommittedPosition();
        if(position == nullptr)
            return {ErrorCode::PositionDoesNotExist};

        Result<> harmonizeResult = _composer.harmonizeFrom(*position);
        if(harmonizeResult.hasError())
            return harmonizeResult.getError();

        // Return all notes currently generated and harmonized
        Vector<MidiNote> result;

        while (position != nullptr)
        {
            List<Note>& notes = position->getChildren();

            if (!notes.empty())
            {
                TimestampMs timestamp = scoreTimeToTimestamp(
                    position->getScoreTime(),
                    _tempo,
                    _startTimestamp);

                for(const Note& note : notes)
                    result.emplace_back(
                        note.midi,
                        note.duration,
                        timestamp);
            }

            position = position->next();
        }

        if(result.empty())
            return {ErrorCode::NoUpcomingNotesAvailable};

        return result;
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
    TimestampMs _latestTimestamp;
    IdProvider _idProvider;

    ScoreTime _minPregeneratedNotesDuration;
    U32 _minPregeneratedPhrases;


    Score _score;
    Composer _composer;
    Map<String, Motif> _motifs;
    Map<String, HarmonyGraph> _graphs;

    //void stop();
    //void setPhraseLength(uint phraseLength);
    //Error transitionToGraph(const String& name);
};

} // namespace Dryad