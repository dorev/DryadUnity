using System.Collections.Generic;
using UnityEngine;
using UnityEditor;


// All this is expected to come from the SWIG classes
namespace Dryad
{
    public enum FlatOrSharp
    {
        Flat,
        Sharp,
        Unspecified
    }

    public enum Note
    {
        C   = 0,
        Db  = 1,
        D   = 2,
        Eb  = 3,
        E   = 4,
        F   = 5,
        Gb  = 6,
        G   = 7,
        Ab  = 8,
        A   = 9,
        Bb  = 10,
        B   = 11
    }

    public class Scale
    {
        public static readonly Scale Major = new Scale(new uint[7] { 0, 2, 4, 5, 7, 9, 11 });
        public static readonly Scale MinorNatural = new Scale(new uint[7] { 0, 2, 3, 5, 7, 8, 10 });
        public static readonly Scale MinorMelodic = new Scale(new uint[7] { 0, 2, 3, 5, 7, 9, 11 });
        public uint[] Intervals { get; set; }
        public uint RootNote { get; set; }
        public FlatOrSharp FlatOrSharp { get; set; }

        public Scale(uint[] intervals, uint rootNote = 48, FlatOrSharp flatOrSharp = FlatOrSharp.Unspecified)
        {
            // check that intervals is 6 long
            Intervals = intervals;
            RootNote = rootNote;
            FlatOrSharp = (flatOrSharp == FlatOrSharp.Unspecified)
                ? FlatOrSharp.Sharp
                : flatOrSharp;

            Tonic = new Chord(this, 1);
            Second = new Chord(this, 2);
            Third = new Chord(this, 3);
            Fourth = new Chord(this, 4);
            Fifth = new Chord(this, 5);
            Sixth = new Chord(this, 6);
            Seventh = new Chord(this, 7);
        }

        public readonly Chord Tonic;
        public readonly Chord Second;
        public readonly Chord Third;
        public readonly Chord Fourth;
        public readonly Chord Fifth;
        public readonly Chord Sixth;
        public readonly Chord Seventh;
    }

    public enum TriadVoicing
    {
        Major,
        Minor,
        Dim,
        Aug,
        Sus2,
        Sus4
    }

    public enum Extension
    {
        None,
        Seventh,
        MajorSeventh,
        Ninth,
        Eleventh
    }

    public class Chord
    {
        public Scale Scale { get; set; }
        public uint Degree { get; set; }
        public TriadVoicing TriadVoicing { get; set; }
        public Extension Extension { get; set; }
        public int Inversion { get; set; }
        public int Accidental { get; set; }
        public string Name
        {
            get
            {
                return EvaluateName();
            }
            private set
            {
                Name = value;
            }
        }

        public Chord(Scale scale, uint degree)
        {
            Scale = scale;
            Degree = degree;
            Extension = Extension.None;
            Inversion = 0;
            Accidental = 0;
            EvaluateTriadVoicing();
        }

        void EvaluateTriadVoicing()
        {
            uint root = Scale.Intervals[Degree - 1];
            uint third = Scale.Intervals[(Degree + 1) % 7];
            uint fifth = Scale.Intervals[(Degree + 3) % 7];
            if (third < root)
                third += 12;
            if (fifth < third)
                fifth += 12;

            if (third - root == 4 && fifth - third == 3)
                TriadVoicing = TriadVoicing.Major;
            else if (third - root == 3 && fifth - third == 4)
                TriadVoicing = TriadVoicing.Minor;
            else if (third - root == 3 && fifth - third == 3)
                TriadVoicing = TriadVoicing.Dim;
            else if (third - root == 4 && fifth - third == 4)
                TriadVoicing = TriadVoicing.Aug;
            else
                throw new System.Exception("Unexpected chord triad deduced");
        }

        string EvaluateName()
        {
            return "N0D3 N4M3";
        }

    }

    public class LandscapeNode
    {
        uint ChordId;
        public Chord Chord;
        public List<uint> NextNodeIds;
        public Rect PositionOnGraph;
    }
}

public class DryadLandscape : MonoBehaviour
{
    [HideInInspector]
    public string Name;
    [HideInInspector]
    public List<Dryad.LandscapeNode> Nodes;
    [HideInInspector]
    public Dryad.Scale Scale;

    // Delegate required because of compilation order
    public delegate void OpenGraphEditorDelegate(DryadLandscape landscape);
    public static OpenGraphEditorDelegate OnOpenGraphEditor;
     
    public void OpenGraphEditor()
    {
        if (OnOpenGraphEditor != null)
            OnOpenGraphEditor(this);
    }
}
