#pragma once

#include "types.h"

namespace Dryad
{

namespace Constants
{

namespace Duration
{
    static constexpr U64 WholeDotted           = 144;
    static constexpr U64 Whole                 = 96;
    static constexpr U64 HalfDotted            = 72;
    static constexpr U64 Half                  = 48;
    static constexpr U64 QuarterDotted         = 36;
    static constexpr U64 HalfTriplet           = 32;
    static constexpr U64 Quarter               = 24;
    static constexpr U64 EighthDotted          = 18;
    static constexpr U64 QuarterTriplet        = 16;
    static constexpr U64 Eighth                = 12;
    static constexpr U64 EighthTriplet         = 8;
    static constexpr U64 Sixteenth             = 6;
    static constexpr U64 SixteenthTriplet      = 4;
    static constexpr U64 ThirtySecond          = 3;
    static constexpr U64 ThirtySecondTriplet   = 2;
};

static const Vector<U64> allowedRythmicFractions =
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
    static constexpr const char* C     = "C";
    static constexpr const char* Cs    = "C#";
    static constexpr const char* Df    = "Db";
    static constexpr const char* D     = "D";
    static constexpr const char* Ds    = "D#";
    static constexpr const char* Ef    = "Eb";
    static constexpr const char* E     = "E";
    static constexpr const char* F     = "F";
    static constexpr const char* Fs    = "F#";
    static constexpr const char* Gf    = "Gb";
    static constexpr const char* G     = "G";
    static constexpr const char* Gs    = "G#";
    static constexpr const char* Af    = "Ab";
    static constexpr const char* A     = "A";
    static constexpr const char* As    = "A#";
    static constexpr const char* Bf    = "Bb";
    static constexpr const char* B     = "B";
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
    static const Vector<U64> M7  = { 4, 7, 10 };
    static const Vector<U64> MM7 = { 4, 7, 11 };
    static const Vector<U64> m7  = { 3, 7, 10 };
    static const Vector<U64> mM7 = { 3, 7, 11 };
    static const Vector<U64> Diminished = { 3, 6 };
    static const Vector<U64> Augmented = { 4, 8 };
}

} // namespace Constants

} // namespace Dryad