#pragma once

#include "dryad/types.h"

namespace Dryad
{

namespace Constants
{

namespace Duration
{
    constexpr U64 WholeDotted           = 144;
    constexpr U64 Whole                 = 96;
    constexpr U64 HalfDotted            = 72;
    constexpr U64 Half                  = 48;
    constexpr U64 QuarterDotted         = 36;
    constexpr U64 HalfTriplet           = 32;
    constexpr U64 Quarter               = 24;
    constexpr U64 EighthDotted          = 18;
    constexpr U64 QuarterTriplet        = 16;
    constexpr U64 Eighth                = 12;
    constexpr U64 EighthTriplet         = 8;
    constexpr U64 Sixteenth             = 6;
    constexpr U64 SixteenthTriplet      = 4;
    constexpr U64 ThirtySecond          = 3;
    constexpr U64 ThirtySecondTriplet   = 2;
};

static const Vector<U64> AllowedRythmicFractions =
{
    Duration::Sixteenth,
    Duration::Eighth,
    Duration::Quarter,
    Duration::QuarterDotted,
    Duration::Half,
    Duration::HalfDotted,
    Duration::Whole,
    Duration::WholeDotted
};

namespace NoteName
{
    constexpr const char* C     = "C";
    constexpr const char* Cs    = "C#";
    constexpr const char* Df    = "Db";
    constexpr const char* D     = "D";
    constexpr const char* Ds    = "D#";
    constexpr const char* Ef    = "Eb";
    constexpr const char* E     = "E";
    constexpr const char* F     = "F";
    constexpr const char* Fs    = "F#";
    constexpr const char* Gf    = "Gb";
    constexpr const char* G     = "G";
    constexpr const char* Gs    = "G#";
    constexpr const char* Af    = "Ab";
    constexpr const char* A     = "A";
    constexpr const char* As    = "A#";
    constexpr const char* Bf    = "Bb";
    constexpr const char* B     = "B";
}

static const Vector<const char*> NoteNamesSharp =
{
    NoteName::C,
    NoteName::Cs,
    NoteName::D,
    NoteName::Ds,
    NoteName::E,
    NoteName::F,
    NoteName::Fs,
    NoteName::G,
    NoteName::Gs,
    NoteName::A,
    NoteName::As,
    NoteName::B
};

static const Vector<const char*> NoteNamesFlat =
{
    NoteName::C,
    NoteName::Df,
    NoteName::D,
    NoteName::Ef,
    NoteName::E,
    NoteName::F,
    NoteName::Gf,
    NoteName::G,
    NoteName::Af,
    NoteName::A,
    NoteName::Bf,
    NoteName::B
};

static const Vector<U64> BaseNotes = {0, 2, 4, 5, 7, 9, 11};

namespace ChordIntervals
{
    const Vector<U64> M7  = { 4, 7, 10 };
    const Vector<U64> MM7 = { 4, 7, 11 };
    const Vector<U64> m7  = { 3, 7, 10 };
    const Vector<U64> mM7 = { 3, 7, 11 };
    const Vector<U64> Diminished = { 3, 6 };
    const Vector<U64> Augmented = { 4, 8 };
}

} // namespace Constants

} // namespace Dryad