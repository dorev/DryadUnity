#pragma once

#include <vector>
#include <string>

struct NoteConcept
{
    int duration;
    int value;
};

struct NoteConcrete
{
    int duration;
    int midi;
    int velocity;
};

struct Motif
{
    std::string name;
    std::vector<NoteConcept> notes;
};

struct Scale
{
    std::string name;
    std::vector<int> intervals;
};

enum class ChordVoicingMask
{
    None            = 0,
    Major           = 1,
    Minor           = 2,
    Augmented       = 3,
    Diminished      = 4,
    Seventh         = 1 >> 4,
    MajorSeventh    = 2 >> 4,
    Ninth           = 1 >> 8,
    Eleventh        = 2 >> 8,
    Thirteenth      = 3 >> 8,
    Sus2            = 1 >> 12,
    Sus4            = 2 >> 12
};

struct Node
{
    int id;
    int degree;
    int inversion;
    int accidental;
    int chordVoicingFlags;
};

struct Edge
{
    int from;
    int to;
};

struct HarmonyGraph
{
    Scale scale;
    std::vector<Node> nodes;
    std::vector<Edge> edges;
};

enum class EventFlag
{
    None,
    Update
};

struct Event
{
    EventFlag flag;
    int id;
    double time;
    std::vector<NoteConcrete> notes;
};

struct Engine
{
    void Start(double time){};
    void Stop(){};
    void SetTempo(int tempo){};
    void RegisterMotif(std::string name, Motif motif){};
    void AddMotif(std::string name){};
    void RemoveMotif(std::string name){};

    void RegisterHarmonyGraph(std::string name, HarmonyGraph HarmonyGraph){};
    void TransitionToHarmonyGraph(std::string name){};

    std::vector<Event> GetUpcomingEvents(int count) { return std::vector<Event>(); };

private:
    class DryadFacade;
    DryadFacade* facade;
};
