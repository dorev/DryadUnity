#pragma once

#include "dryad/types.h"
#include "dryad/crtphelper.h"

namespace Dryad
{

// Order is important in this enum, from lowest to highest in the score hierarchy
enum class ScoreElementTypeId
{
    Note,
    Position,
    Measure,
    Phrase,
    Score
};

class Session;
class Score;
class Phrase;
class Measure;
class Position;
class Note;

// Compile-time function to extract the score element type id
template <class T> struct GetTypeIdImpl;
template <> struct GetTypeIdImpl<Score> { static constexpr ScoreElementTypeId value = ScoreElementTypeId::Score; };
template <> struct GetTypeIdImpl<Phrase> { static constexpr ScoreElementTypeId value = ScoreElementTypeId::Phrase; };
template <> struct GetTypeIdImpl<Measure> { static constexpr ScoreElementTypeId value = ScoreElementTypeId::Measure; };
template <> struct GetTypeIdImpl<Position> { static constexpr ScoreElementTypeId value = ScoreElementTypeId::Position; };
template <> struct GetTypeIdImpl<Note> { static constexpr ScoreElementTypeId value = ScoreElementTypeId::Note; };
template <class T> constexpr ScoreElementTypeId GetTypeId = GetTypeIdImpl<T>::value;

// Compile-time function to obtain the parent type of a score element
template <class T> struct ParentTypeImpl;
template <> struct ParentTypeImpl<Score> { using type = Session; };
template <> struct ParentTypeImpl<Phrase> { using type = Score; };
template <> struct ParentTypeImpl<Measure> { using type = Phrase; };
template <> struct ParentTypeImpl<Position> { using type = Measure; };
template <> struct ParentTypeImpl<Note> { using type = Position; };
template <class T> using ParentType = typename ParentTypeImpl<T>::type;

// Compile-time function to obtain the child type of a score element
template <class T> struct ChildTypeImpl;
template <> struct ChildTypeImpl<Session> { using type = Score; };
template <> struct ChildTypeImpl<Score> { using type = Phrase; };
template <> struct ChildTypeImpl<Phrase> { using type = Measure; };
template <> struct ChildTypeImpl<Measure> { using type = Position; };
template <> struct ChildTypeImpl<Position> { using type = Note; };
template <class T> using ChildType = typename ChildTypeImpl<T>::type;

// Compile-time function to identify the hierarchic difference between two type ids
template <ScoreElementTypeId SourceTypeId, ScoreElementTypeId DestinationTypeId>
constexpr S32 GetHierarchyDistance = static_cast<S32>(SourceTypeId) - static_cast<S32>(DestinationTypeId);

}
