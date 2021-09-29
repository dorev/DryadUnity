#pragma once

#include "dryad/types.h"
#include "dryad/error.h"
#include "dryad/result.h"
#include "dryad/idprovider.h"

#include "dryad/harmony/harmonygraph.h"
#include "dryad/harmony/motif.h"
#include "dryad/harmony/composer.h"

#include "dryad/score/score.h"

namespace Dryad
{

class ScoreReader
{

};

class Session
{

public:

    Session()
        : _tempo(0)
        , _startTimestamp(0)
        , _currentTimestamp(0)
        , _frameDuration(0)
        , _phraseSize(0)
        , _score(*this)
    {
    }

    Session& reachSession()
    {
        return *this;
    }

    //void stop();
    //void setPhraseLength(uint phraseLength);
    //void registerMotif(const String& name, Motif&& motif);
    //SessionError generateMotif(const String& name, ScoreTime duration, uint melodicEnergy, uint rhythmicEnergy);
    //Error changeMotifInstanceCount(const String& name, S32 amount);
    //Error setMotifInstanceCount(const String& name, U32 count);
    //Error transitionToGraph(const String& name);

    Score& getScore()
    {
        return _score;
    }

    void setTempo(U32 tempo) 
    {
        _tempo = tempo;
    }

    U32 getTempo() const
    {
        return _tempo;
    }

    void setFrameDuration(TimeMs frameDuration)
    {
        _frameDuration = frameDuration;
    }
    
    TimeMs getFrameDuration() const
    {
        return _frameDuration;
    }

    void start(TimestampMs startTime)
    {
        _startTimestamp = startTime;
    }

    Error registerMotif(const String& name, const Motif& motif)
    {
        if(_motifs.find(name) != _motifs.end())
            return {ErrorCode::MotifNameAlreadyExists};

        _motifs[name] = motif;
    }

    Error registerHarmonyGraph(const String& name, const HarmonyGraph& graph)
    {
        if(_graphs.find(name) != _graphs.end())
            return {ErrorCode::GraphNameAlreadyExists};

        _graphs[name] = graph;
    }

    using Frame = Vector<Pair<Note, TimestampMs>>;

    Result<Frame> getNextFrame(TimestampMs currentTimestamp)
    {
        if((_currentTimestamp + _frameDuration) > currentTimestamp)
            return {ErrorCode::TooEarlyForNextFrame};

        Frame frame;

        // figure when generating, always generate for the full phrase and keep it on stand-by
        // only modify the rest of the phrase if necessary
        // know then the end of the phrase is close and prepare the next one
        // if a motif is added, add it coherently (next strong time)
        // if it is removed, let it finish its current occurence

        // identify the measures & measureTime boundaries of next frame

        // check if next motifs need to be added/removed
        // check if a we need to transition to a new graph
            // generate the rest of the phrase accordingly (only if something changed)

        // always have the current phrase and the next one generated
        return frame;
    }

private:

    U32 _tempo;
    U32 _phraseSize;
    TimeMs _frameDuration;
    TimestampMs _startTimestamp;
    TimestampMs _currentTimestamp;
    IdProvider _idProvider;
    Composer _composer;

    Map<String, HarmonyGraph> _graphs;
    Map<String, Motif> _motifs;

    Map<String, U32> _activeMotifs;
    String _activeGraph;

    Score _score;
};


} // namespace Dryad