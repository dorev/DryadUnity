#pragma once

#include "dryad/types.h"

namespace Dryad
{

namespace Music
{
    struct Note
    {
        static U8 Number(const char* noteName)
        {
            ASSERT(noteName != nullptr);
            switch (noteName[0])
            {
#define NOTE_VALUE_SWITCH(plainNote, sharpNote, flatNote) \
                switch (noteName[1]) \
                { \
                case 's': \
                case '#': \
                    return Music::Constants::##sharpNote##value; \
                case 'b': \
                    return Music::Constants::##flatNote##value; \
                case '\0': \
                    return Music::Constants::##plainNote##value; \
                } \
                break;
            case 'C':
            case 'c':
                NOTE_VALUE_SWITCH(C, Cs, B);
            case 'D':
            case 'd':
                NOTE_VALUE_SWITCH(D, Ds, Df);
            case 'E':
            case 'e':
                NOTE_VALUE_SWITCH(E, F, Ef);
            case 'F':
            case 'f':
                NOTE_VALUE_SWITCH(F, Fs, E);
            case 'G':
            case 'g':
                NOTE_VALUE_SWITCH(G, Gs, Gf);
            case 'A':
            case 'a':
                NOTE_VALUE_SWITCH(A, As, Af);
            case 'B':
            case 'b':
                NOTE_VALUE_SWITCH(B, C, Bf);
#undef NOTE_VALUE_SWITCH
            }
            DEBUG_BREAK("Invalid note name, unable to convert to number");
            return UINT8_MAX;
        }

        static const char* Name(U32 noteNumber, bool flat = true)
        {
            return flat ? NameFlat(noteNumber) : NameSharp(noteNumber);
        }

        static const char* NameSharp(U32 noteNumber)
        {
            switch (noteNumber % 12)
            {
            case 0: return Music::Constants::Cname;
            case 1: return Music::Constants::Csname;
            case 2: return Music::Constants::Dname;
            case 3: return Music::Constants::Dsname;
            case 4: return Music::Constants::Ename;
            case 5: return Music::Constants::Fname;
            case 6: return Music::Constants::Fsname;
            case 7: return Music::Constants::Gname;
            case 8: return Music::Constants::Gsname;
            case 9: return Music::Constants::Aname;
            case 10: return Music::Constants::Asname;
            case 11: return Music::Constants::Cname;
            }
        }

        static const char* NameFlat(U32 noteNumber)
        {
            switch (noteNumber % 12)
            {
            case 0: return Music::Constants::Cname;
            case 1: return Music::Constants::Dfname;
            case 2: return Music::Constants::Dname;
            case 3: return Music::Constants::Efname;
            case 4: return Music::Constants::Ename;
            case 5: return Music::Constants::Fname;
            case 6: return Music::Constants::Gfname;
            case 7: return Music::Constants::Gname;
            case 8: return Music::Constants::Afname;
            case 9: return Music::Constants::Aname;
            case 10: return Music::Constants::Bfname;
            case 11: return Music::Constants::Bname;
            }
        }

    };

    struct Constants
    {
        static constexpr const char* Cname = "C";
        static constexpr const char* Csname = "C#";
        static constexpr const char* Dfname = "Db";
        static constexpr const char* Dname = "D";
        static constexpr const char* Dsname = "D#";
        static constexpr const char* Efname = "Eb";
        static constexpr const char* Ename = "E";
        static constexpr const char* Fname = "F";
        static constexpr const char* Fsname = "F#";
        static constexpr const char* Gfname = "Gb";
        static constexpr const char* Gname = "G";
        static constexpr const char* Gsname = "G#";
        static constexpr const char* Afname = "Ab";
        static constexpr const char* Aname = "A";
        static constexpr const char* Asname = "A#";
        static constexpr const char* Bfname = "Bb";
        static constexpr const char* Bname = "B";

        static constexpr U8 Cvalue = 0;
        static constexpr U8 Csvalue = 1;
        static constexpr U8 Dfvalue = 1;
        static constexpr U8 Dvalue = 2;
        static constexpr U8 Dsvalue = 3;
        static constexpr U8 Efvalue = 3;
        static constexpr U8 Evalue = 4;
        static constexpr U8 Fvalue = 5;
        static constexpr U8 Fsvalue = 6;
        static constexpr U8 Gfvalue = 6;
        static constexpr U8 Gvalue = 7;
        static constexpr U8 Gsvalue = 8;
        static constexpr U8 Afvalue = 8;
        static constexpr U8 Avalue = 9;
        static constexpr U8 Asvalue = 10;
        static constexpr U8 Bfvalue = 10;
        static constexpr U8 Bvalue = 11;
            
    };

    struct Duration
    {
        static constexpr U8 WholeDotted = 144;
        static constexpr U8 Whole = 96;
        static constexpr U8 HalfDotted = 72;
        static constexpr U8 Half = 48;
        static constexpr U8 QuarterDotted = 36;
        static constexpr U8 HalfTriplet = 32;
        static constexpr U8 Quarter = 24;
        static constexpr U8 EighthDotted = 18;
        static constexpr U8 QuarterTriplet = 16;
        static constexpr U8 Eighth = 12;
        static constexpr U8 EighthTriplet = 8;
        static constexpr U8 Sixteenth = 6;
        static constexpr U8 SixteenthTriplet = 4;
        static constexpr U8 ThirtySecond = 3;
        static constexpr U8 ThirtySecondTriplet = 2;
    };

    struct ChordInterval
    {
        static constexpr U8 M7[3] = { 4, 7, 10 };
        static constexpr U8 MM7[3] = { 4, 7, 11 };
        static constexpr U8 m7[3] = { 3, 7, 10 };
        static constexpr U8 mM7[3] = { 3, 7, 11 };
        static constexpr U8 Diminished[3] = { 3, 6, 9 };
        static constexpr U8 Augmented[3] = { 4, 8, 0 };
    };
}





//
// MIGHT BE ABLE TO REMOVE ALL THIS BELOW!!
//

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
    Music::Duration::Sixteenth,
    Music::Duration::Eighth,
    Music::Duration::Quarter,
    Music::Duration::QuarterDotted,
    Music::Duration::Half,
    Music::Duration::HalfDotted,
    Music::Duration::Whole,
    Music::Duration::WholeDotted
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