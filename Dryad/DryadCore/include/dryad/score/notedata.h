#pragma once

#include "dryad/types.h"

namespace Dryad
{
    using ScoreTime = U32;

    class Note
    {
        U32 _value;
    };

    class Degree
    {
        U32 _number;
        U32 _scaleOffsets[4];
    };

    class Scale
    {
        Degree _degrees[7];
    };

    class LandscapeNode
    {
        U32 _duration;
        Degree* _degree;
    };

    class Progression
    {
        Vector<LandscapeNode*> _chords;
    };

    class NoteData
    {
        U32 _midi;
        U32 _duration;
        Motif* _motif;
        Voice* _voice;
    };

    class Instant
    {
        Vector<NoteData> _notes;
        LandscapeNode* _landscape;
        Scale* _scale;
        Note _scaleRoot;
        Instant* _next;
        Instant* prev;
        ScoreTime _scoreTime;
    };


    class Score
    {
        Instant* _firstUncommitted;
        List<Instant> _instants;
    };

    class Session
    {
        Map<String, LandscapeNode> _landscapes;
        Map<String, Motif> _motifs;
        Map<String, Voice> _voices;
        Map<String, Scale> _scales;
        Scale* _currentScale;
        Note _currentRoot;
        LandscapeNode* _currentLandscape;
    };
}
