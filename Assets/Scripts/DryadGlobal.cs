using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

namespace DryadUnity
{
    static public class Duration
    {
        // DryadCore note duration sizes
        static public readonly uint WholeDotted = 144;
        static public readonly uint Whole = 96;
        static public readonly uint HalfDotted = 72;
        static public readonly uint Half = 48;
        static public readonly uint QuarterDotted = 36;
        static public readonly uint HalfTriplet = 32;
        static public readonly uint Quarter = 24;
        static public readonly uint EighthDotted = 18;
        static public readonly uint QuarterTriplet = 16;
        static public readonly uint Eighth = 12;
        static public readonly uint EighthTriplet = 8;
        static public readonly uint Sixteenth = 6;
        static public readonly uint SixteenthTriplet = 4;
        static public readonly uint ThirtySecond = 3;
        static public readonly uint ThirtySecondTriplet = 2;
    }

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
    public enum ChordExtension
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
        public ChordExtension Extension;
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
            Extension = ChordExtension.None;
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
                case TriadVoicing.Major: break;
                case TriadVoicing.Minor: result = result.ToLower(); break;
                case TriadVoicing.Aug: result += "+"; break;
                case TriadVoicing.Dim: result = result.ToLower() + "°"; break;
                default:
                    throw new System.Exception($"Impossible chord voicing value: {TriadVoicing}");
            }

            switch (Shift)
            {
                case -2: result += "bb"; break;
                case -1: result += "b"; break;
                case 0: break;
                case 1: result += "#"; break;
                case 2: result += "##"; break;
                default:
                    throw new System.Exception($"Impossible chord shift value: {Shift}");
            }

            switch (Extension)
            {
                case ChordExtension.None: break;
                case ChordExtension.Seventh: result += "7"; break;
                case ChordExtension.MajorSeventh: result += "M7"; break;
                case ChordExtension.Ninth: result += "9"; break;
                default:
                    throw new System.Exception($"Impossible chord extension value: {Extension}");
            }

            return result;
        }

    }

}

public class DryadGlobal : MonoBehaviour
{
    public float RefreshPeriod;

    static DryadGlobal Instance;
    WaitForSeconds SlowPeriod;
    uint Tempo = 120;
    uint ScoreTimeTickInterval = DryadUnity.Duration.ThirtySecondTriplet;
    List<DryadListener> Listeners = new List<DryadListener>();

    enum GlobalStatus
    {
        Init,
        Running,
        ShuttingDown
    };

    GlobalStatus Status;

    public static DryadGlobal GetInstance()
    {
        return Instance;
    }

    void Awake()
    {
        // Prefab/Singleton check
        if(Instance != null && Instance != this)
            Destroy(this.gameObject);
        else
            Instance = this;
    }

    void Start()
    {
        Status = GlobalStatus.Init;
        SlowPeriod = new WaitForSeconds(RefreshPeriod);
        StartCoroutine(SlowPeriodicWork());
        StartCoroutine(ScoreTickPeriodicWork());
        SetupDebugUI();
        Status = GlobalStatus.Running;
    }

    void Update()
    {
        UpdateDebugUI();
        SlowPeriod = new WaitForSeconds(RefreshPeriod);
    }

    void Shutdown()
    {
        Status = GlobalStatus.ShuttingDown;
        StopCoroutine(SlowPeriodicWork());
        StopCoroutine(ScoreTickPeriodicWork());
    }

    IEnumerator SlowPeriodicWork()
    {
        while (Status != GlobalStatus.ShuttingDown)
        {
            UpdateListenersMotifs();
            yield return SlowPeriod;
        }
    }

    IEnumerator ScoreTickPeriodicWork()
    {
        while (Status != GlobalStatus.ShuttingDown)
        {
            foreach (DryadListener listener in Listeners)
                listener.Tick(ScoreTimeTickInterval);
            yield return ScoreTickPeriodFromTempo();
        }
    }

    WaitForSeconds ScoreTickPeriodFromTempo()
    {
        float beatDuration = 60.0f / (float)Tempo;
        float tickInterval = beatDuration / (DryadUnity.Duration.Quarter / ScoreTimeTickInterval);
        return new WaitForSeconds(tickInterval);
    }

    void UpdateListenersMotifs()
    {
        // Check what motifs are in range of listeners
        foreach (DryadListener listener in Listeners)
        {
            List<DryadMotif> motifsInRange = new List<DryadMotif>();
            foreach (DryadMotif motif in FindObjectsOfType<DryadMotif>())
            {
                float distance = Vector3.Distance(listener.transform.position, motif.transform.position);
                if (distance < motif.BroadcastRange)
                    motifsInRange.Add(motif);
            }
            listener.CompareUpdateWithSurroundingMotifs(motifsInRange);
        }
    }

    public void Register(DryadListener listener)
     {
        if(!Listeners.Contains(listener))
        {
            Listeners.Add(listener);
            Debug.Log("Added listener");
        }
     }

     public void Unregister(DryadListener listener)
     {
        if (Listeners.Contains(listener))
        {
            Listeners.Remove(listener);
            Debug.Log("Added listener");
        }
     }

    #region DebugUI

    GameObject canvasContainer;
    Canvas canvas;
    GameObject debugContainer;
    Text debugText;
    RectTransform debugTransform;
    void SetupDebugUI()
    {
        canvasContainer = new GameObject();
        canvasContainer.name = "DryadDebugUI";
        canvas = canvasContainer.AddComponent<Canvas>();
        canvas.renderMode = RenderMode.ScreenSpaceOverlay;
        canvasContainer.AddComponent<CanvasScaler>();
        canvasContainer.AddComponent<GraphicRaycaster>();

        debugContainer = new GameObject();
        debugContainer.name = "Debug";
        debugContainer.transform.parent = canvasContainer.transform;
        debugText = debugContainer.AddComponent<Text>();
        debugText.text = "debug debug debug";
        debugText.font = (Font)Resources.GetBuiltinResource(typeof(Font), "Arial.ttf");
        debugText.fontSize = 14;
        debugTransform = debugText.GetComponent<RectTransform>();
        debugTransform.localPosition = new Vector3(-300, 100, 0);
        debugTransform.sizeDelta = new Vector2(400, 200);
    }

    void UpdateDebugUI()
    {
        if (canvasContainer == null)
            return;

        debugText.text = "";

        foreach (DryadListener listener in Listeners)
        {
            debugText.text += $"Listener {listener.Name}\n";

            if (listener.GetCurrentLandscape())
                debugText.text += $"Current landscape: {listener.GetCurrentLandscape()?.Name}\n";

            if (listener.GetPreviousLandscape())
                debugText.text += $"Previous landscape: {listener.GetPreviousLandscape()?.Name}\n";

            debugText.text += $"Motifs:\n";

            foreach (DryadMotif motif in listener.GetMotifs())
                debugText.text += $"  - {motif.Name}\n";
        }
    }

    #endregion
}
