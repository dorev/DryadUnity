#pragma once

#include "types.h"
#include "error.h"

#include "note.h"
#include "score.h"
#include "harmonygraph.h"
#include "motif.h"
#include "result.h"
#include "composer.h"
#include "templateutils.h"

namespace Dryad
{

class Session;
class Score;
class Phrase;
class Measure;
class Position;

enum class ScoreElementTypeId
{
    Score,
    Phrase,
    Measure,
    Position,
    Note
};

template <class T> struct GetTypeIdImpl;
template <> struct GetTypeIdImpl<Score> { static constexpr ScoreElementTypeId value = ScoreElementTypeId::Score; };
template <> struct GetTypeIdImpl<Phrase> { static constexpr ScoreElementTypeId value = ScoreElementTypeId::Phrase; };
template <> struct GetTypeIdImpl<Measure> { static constexpr ScoreElementTypeId value = ScoreElementTypeId::Measure; };
template <> struct GetTypeIdImpl<Position> { static constexpr ScoreElementTypeId value = ScoreElementTypeId::Position; };
template <> struct GetTypeIdImpl<Note> { static constexpr ScoreElementTypeId value = ScoreElementTypeId::Note; };
template <class T> constexpr ScoreElementTypeId GetTypeId = GetTypeIdImpl<T>::value;

template <class T> struct ParentTypeImpl;
template <> struct ParentTypeImpl<Score> { using type = Session; };
template <> struct ParentTypeImpl<Phrase> { using type = Score; };
template <> struct ParentTypeImpl<Measure> { using type = Phrase; };
template <> struct ParentTypeImpl<Position> { using type = Measure; };
template <> struct ParentTypeImpl<Note> { using type = Position; };
template <class T> using ParentType = typename ParentTypeImpl<T>::type;

template <class T> struct ChildTypeImpl;
template <> struct ChildTypeImpl<Session> { using type = Score; };
template <> struct ChildTypeImpl<Score> { using type = Phrase; };
template <> struct ChildTypeImpl<Phrase> { using type = Measure; };
template <> struct ChildTypeImpl<Measure> { using type = Position; };
template <> struct ChildTypeImpl<Position> { using type = Note; };
template <class T> using ChildType = typename ChildTypeImpl<T>::type;

template <class ScoreElementType>
class Cursor : public CrtpHelper<ScoreElementType, Cursor>
{

    using CrtpBase = typename CrtpHelper<ScoreElementType, Cursor>;

public:
    Cursor() = delete;
    Cursor(ParentType<ScoreElementType>& parent)
        : _parent(parent)
        , _session(parent.reachSession())
    {
    }

    Score& reachScore()
    {
        if constexpr(_typeId == ScoreElementTypeId::Score)
            return *this;
        else
            return reachSession().getScore();
    }

    Session& reachSession()
    {
        return _session;
    }

    ParentType<ScoreElementType>& parent(U16 ancestry = 1)
    {
        if(ancestry == 1)
            return _parent;

        return parent(--ancestry);
    }

    List<ChildType<ScoreElementType>>& getChildren()
    {
        return CrtpBase::getCrtpChild()._children;
    }

    List<ScoreElementType>& getSiblings()
    {
        return parent().getChildren();
    }

    ScoreElementType* prev()
    {
        List<ScoreElementType>& siblings = getSiblings();

        for(auto sibling = siblings.begin(); sibling != siblings.end(); sibling++)
        {
            if (&*sibling == this)
            {
                if(sibling == siblings.begin())
                    break;
                else
                    return &*(--sibling);
            }
        }

        return nullptr;
    }

    ScoreElementType* next()
    {
        List<ScoreElementType>& siblings = parent().getChildren();

        for(auto sibling = siblings.begin(); sibling != siblings.end(); sibling++)
        {
            if (&*sibling == this)
            {
                if(++sibling == siblings.end())
                    break;
                else
                    return &*(sibling);
            }
        }

        return nullptr;
    }

    template <class AnyScoreElement>
    Phrase* prevPhrase(AnyScoreElement& element)
    {
        switch (GetTypeId<AnyScoreElement>)
        {
        case Phrase: return element.prev();
        case Measure: return element.parent().prev();
        case Position: return element.parent(2).prev();
        case Note: return element.parent(3).prev();
        }
    }

    Phrase* reachLastCommittedPhrase();
    Measure* reachLastComittedMeasure();
    Position* reachLastComittedPosition();

    Phrase* nextPhrase();

    Measure* nextMeasure();
    Measure* prevMeasure();
    Position* nextPosition();
    Position* prevPosition();
    Note* nextNote();
    Note* prevNote();

protected:

    using Base = typename Cursor<ScoreElementType>;
    ParentType<ScoreElementType>& _parent;
    Session& _session;
    static constexpr ScoreElementTypeId _typeId = GetTypeId<ScoreElementType>;
};

class ScoreWriter
{
    //insertMotifAfterPosition(Position&
};

class ScoreReader
{

};

class Position : Cursor<Position>
{
public:

    Position(Measure& parentMeasure)
        : Base(parentMeasure)
        , _children(_notes)
    {
    }

private:

    friend Base;
    List<Note>& _children;
    List<Note> _notes;
};

class Measure : public Cursor<Measure>
{
public:

    Measure(Phrase& parentPhrase)
        : Base(parentPhrase)
        , _children(_positions)
    {
    }

private:

    friend Base;
    List<Position>& _children;
    List<Position> _positions;
};

class Phrase : public Cursor<Phrase>
{
public:

    Phrase(Score& parentScore)
        : Base(parentScore)
        , _children(_measures)
    {
    }

private:

    friend Base;
    List<Measure>& _children;
    List<Measure> _measures;
};

class Score : public Cursor<Score>
{
public:

    Score(Session& parentSession)
        : Base(parentSession)
        , _children(_phrases)
    {
    }

    Phrase& appendPhrase()
    {
        _phrases.emplace_back(*this);
        return _phrases.back();
    }

    Phrase& appendPhrase(Phrase&& phrase)
    {
        _phrases.emplace_back(phrase);
        return _phrases.back();
    }

private:

    friend Base;
    List<Phrase>& _children;
    List<Phrase> _phrases;
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