#pragma once

#include "dryad/types.h"

namespace Dryad
{

    class Degree
    {
        U8 _number;
        U8 _scaleOffsets[4];
    };

    class Scale
    {
        Vector<Degree> _degrees;
        U32 _tonic;
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
        Instant* _next;
        Instant* prev;
        ScoreTime _scoreTime;
    };

    class Session
    {
        List<Instant> _score;
        Map<String, LandscapeNode> _landscapes;
        Map<String, Motif> _motifs;
        Scale* _currentScale;
        LandscapeNode* _currentLandscape;
        LandscapeNode* _previousLandscape;
        bool _transitioningLandscape;
        Instant* _firstUncommitted;
    };
}
