#pragma once

#include <vector>
#include <string>
#include <memory>

struct NoteToPlay
{
    unsigned scoreTime;
    unsigned midiValue;
    unsigned duration;
    unsigned parentMotif;
    bool played;
};

struct MotifNote
{
    unsigned scoreTime;
    unsigned duration;
    int offset;
};

struct Motif
{
    unsigned id;
    unsigned duration;
    std::string name;
    std::vector<MotifNote> notes;
};

enum class FlatOrSharp
{
    Sharp,
    Flat,
    Unspecified
};

struct Scale
{
    unsigned rootNote;
    FlatOrSharp flatOrSharp;
    std::string name;
    std::vector<unsigned> intervals;
};

enum class TriadVoicing
{
    Major,
    Minor,
    Augmented,
    Diminished
};

enum class ChordExtension
{
    None,
    Seventh,
    MajorSeventh,
    Ninth
};

struct Node
{
    unsigned id;
    unsigned degree;
    int inversion;
    int accidental;
    int shift;
    bool entry;
    TriadVoicing triadVoicing;
    ChordExtension chordExtension;
};

struct Edge
{
    unsigned from;
    unsigned to;
};

struct LandscapeGraph
{
    unsigned id;
    Scale scale;
    std::string name;
    std::vector<Node> nodes;
    std::vector<Edge> edges;
};

class Facade
{
public:
    Facade();
    ~Facade();
    void RegisterMotif(const Motif& motif);
    void RegisterLandscape(const LandscapeGraph& landscape);
    void AddMotif(const unsigned motifId);
    void RemoveMotif(const unsigned motifId);
    void SetLandscape(const unsigned landscapeId);
    std::vector<NoteToPlay> Generate(const unsigned durationToGenerate);
    std::vector<NoteToPlay> Commit(unsigned durationToPlay);

private:
    class Internal;
    std::unique_ptr<Internal> _internal;
};
