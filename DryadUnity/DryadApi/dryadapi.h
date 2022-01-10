#pragma once

#include <vector>
#include <string>

struct NoteToPlay
{
    unsigned scoreTime;
    unsigned midiValue;
    unsigned duration;
    unsigned parentMotif;
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
    std::string name;
    unsigned duration;
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
    std::string name;
    unsigned rootNote;
    FlatOrSharp flatOrSharp;
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

struct Score
{
    Score() {};
    void RegisterMotif(Motif motif) {};
    void RegisterLandscape(LandscapeGraph landscape) {};
    void AddMotif(unsigned motifId) {};
    void RemoveMotif(unsigned motifId) {};
    void SetLandscape(unsigned landscapeId) {};
    void Generate(unsigned durationToGenerate) {};
    std::vector<NoteToPlay> Play(unsigned durationToPlay) { return {}; };

private:
    class DryadFacade;
    DryadFacade* facade;
};
