#pragma once

#include "includes.h"

namespace Dryad
{

// Forward declarations
struct Degree;
struct HarmonyGraph;
struct HarmonyNode;
struct Measure;
struct Motif;
struct MotifConfig;
struct MotifVariation;
struct Note;
struct Phrase;
struct Position;
struct Scale;
struct ScaleConfig;
struct Score;
struct Session;
struct Voice;

// Progression fitting modes
enum class FittingMode
{
    PowerOf2Left,
    PowerOf2Right,
    CompactLeft,
    CompactRight
};

enum class Accidental
{
    None,
    Sharp,
    Flat
};

template <class T>
using SharedPtr = std::shared_ptr<T>;

template <class T>
using WeakPtr = std::weak_ptr<T>;

template <class T>
using Vector = std::vector<T>;

template <class T, class U>
using Pair = std::pair<T, U>;

template <class T>
using InitializerList = std::initializer_list<T>;

using String = std::string;

using uint = uint64_t;
using sint = int64_t;
using Energy = sint;


} // namespace Dryad