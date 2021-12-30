using System.Collections.Generic;
using UnityEngine;

// All this is expected to come from the SWIG classes
namespace Dryad
{
    [System.Serializable]
    public enum FlatOrSharp
    {
        Flat,
        Sharp,
        Unspecified
    }

    [System.Serializable]
    public enum Note
    {
        C = 0,
        Db = 1,
        D = 2,
        Eb = 3,
        E = 4,
        F = 5,
        Gb = 6,
        G = 7,
        Ab = 8,
        A = 9,
        Bb = 10,
        B = 11
    }

    [System.Serializable]
    public enum TriadVoicing
    {
        Major,
        Minor,
        Dim,
        Aug
    }

    [System.Serializable]
    public enum Extension
    {
        None,
        Seventh,
        MajorSeventh,
        Ninth
    }

    [System.Serializable]
    public class Scale
    {
        public static readonly Scale Major = new Scale(new uint[7] { 0, 2, 4, 5, 7, 9, 11 });
        public static readonly Scale MinorNatural = new Scale(new uint[7] { 0, 2, 3, 5, 7, 8, 10 });
        public static readonly Scale MinorMelodic = new Scale(new uint[7] { 0, 2, 3, 5, 7, 9, 11 });
        public uint[] Intervals;
        public uint RootNote;
        public FlatOrSharp FlatOrSharp;

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

    [System.Serializable]
    public class Chord
    {
        public Scale Scale;
        public uint Degree;
        public TriadVoicing TriadVoicing;
        public Extension Extension;
        public int Inversion;
        public int Shift;
        public bool Entry;
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
            Shift = 0;
            Entry = false;
            EvaluateTriadVoicing();
        }

        public Chord(Chord other)
        {
            Scale = other.Scale;
            Degree = other.Degree;
            Extension = other.Extension;
            Inversion = other.Inversion;
            Shift = other.Shift;
            Entry = other.Entry;
            TriadVoicing = other.TriadVoicing;
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
            string result;
            switch (Degree)
            {
                case 1: result = "I"; break;
                case 2: result = "II"; break;
                case 3: result = "III"; break;
                case 4: result = "IV"; break;
                case 5: result = "V"; break;
                case 6: result = "VI"; break;
                case 7: result = "VII"; break;
                default:
                    throw new System.Exception($"Impossible chord degree value: {Degree}");
            }

            switch (TriadVoicing)
            {
                case TriadVoicing.Major:    break;
                case TriadVoicing.Minor:    result = result.ToLower(); break;
                case TriadVoicing.Aug:      result += "+"; break;
                case TriadVoicing.Dim:      result = result.ToLower() + "°"; break;
                default:
                    throw new System.Exception($"Impossible chord voicing value: {TriadVoicing}");
            }

            switch (Shift)
            {
                case -2:    result += "bb"; break;
                case -1:    result += "b"; break;
                case 0:     break;
                case 1:     result += "#"; break;
                case 2:     result += "##"; break;
                default:
                    throw new System.Exception($"Impossible chord shift value: {Shift}");
            }

            switch(Extension)
            {
                case Extension.None:            break;
                case Extension.Seventh:         result += "7"; break;
                case Extension.MajorSeventh:    result += "M7"; break;
                case Extension.Ninth:           result += "9"; break;
                default:
                    throw new System.Exception($"Impossible chord extension value: {Extension}");
            }

            return result;
        }
                
    }

}

[System.Serializable]
public class LandscapeNodeData
{
    public uint Id;
    public Dryad.Chord Chord;
    public List<uint> Edges;
    public Rect Rect;

    public LandscapeNodeData(uint id, Dryad.Chord chord, List<uint> edges, Rect position)
    {   
        Id = id;
        Chord = chord;
        Edges = edges;
        Rect = position;
    }
}

public class DryadLandscape : MonoBehaviour
{
    [HideInInspector]
    public string Name;
    [HideInInspector]
    public Dryad.Scale Scale;
    [SerializeField]
    public List<LandscapeNodeData> NodesData;

    // Delegate required because of compilation order
    public delegate void OpenGraphEditorDelegate(DryadLandscape landscape);
    public static OpenGraphEditorDelegate OnOpenGraphEditor;
     
    public void OpenLandscapeEditor()
    {
        if (NodesData == null)
            NodesData = new List<LandscapeNodeData>();

        if (OnOpenGraphEditor != null)
            OnOpenGraphEditor(this);
    }
}
