#pragma once

#include "dryad/definitions.h"
#include "dryad/types.h"

namespace Dryad
{
namespace Music
{
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

        PRIVATE_CONSTRUCTOR(Constants);
        DELETE_COPY_CONSTRUCTOR(Constants);
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

        PRIVATE_CONSTRUCTOR(Duration);
        DELETE_COPY_CONSTRUCTOR(Duration);
    };

    struct ChordInterval
    {
        static constexpr U8 M7[3] = { 4, 7, 10 };
        static constexpr U8 MM7[3] = { 4, 7, 11 };
        static constexpr U8 m7[3] = { 3, 7, 10 };
        static constexpr U8 mM7[3] = { 3, 7, 11 };
        static constexpr U8 Diminished[3] = { 3, 6, 9 };
        static constexpr U8 Augmented[3] = { 4, 8, 0 };

        PRIVATE_CONSTRUCTOR(ChordInterval);
        DELETE_COPY_CONSTRUCTOR(ChordInterval);
    };

    struct Note
    {
        static U8 GetNumber(const char* noteName)
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
                default: \
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

        static const char* GetName(U32 noteNumber, Accidental accidental = Accidental::Flat)
        {
            switch (accidental)
            {
            case Accidental::Flat:
                return NameFlat(noteNumber);
            default:
                return NameSharp(noteNumber);
            }
        }

    private:
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

        PRIVATE_CONSTRUCTOR(Note);
        DELETE_COPY_CONSTRUCTOR(Note);
    };

} // namespace Music
} // namespace Dryad
