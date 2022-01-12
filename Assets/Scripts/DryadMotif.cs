using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[System.Serializable]
public class MotifNoteData
{
    public uint Id;
    public uint ScoreTime;
    public int TonicOffset;
    public uint Duration;

    public MotifNoteData(uint id, uint scoreTime, int tonicOffset, uint duration)
    {
        Id = id;
        ScoreTime = scoreTime;
        TonicOffset = tonicOffset;
        Duration = duration;
    }
}

public class DryadMotif : MonoBehaviour
{
    [SerializeField]
    public string Name = "NoMotifName";
    [SerializeField]
    public string Voice = "NoVoiceName";
    [SerializeField]
    public float BroadcastRange = 5f;
    [SerializeField]
    public uint Duration = DryadUnity.Duration.Quarter;

    [HideInInspector]
    [SerializeField]
    public List<MotifNoteData> NotesData;

    // Delegate required because of compilation order
    public delegate void OpenMotifEditorDelegate(DryadMotif landscape);
    public static OpenMotifEditorDelegate OnOpenMotifEditor;

    public void OpenMotifEditor()
    {
        if (NotesData == null)
            NotesData = new List<MotifNoteData>();

        OnOpenMotifEditor?.Invoke(this);
    }

    public DryadMotif()
    {
    }
}
