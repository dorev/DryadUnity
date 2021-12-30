using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[System.Serializable]
public class MotifNote
{
    uint OnScoreTime;
    uint OffScoreTime;
    int ScaleOffset;
}

public class DryadMotif : MonoBehaviour
{
    // https://unity3d.college/2017/05/22/unity-attributes/

    public string Name = "NoMotifName";
    public string Voice = "NoVoiceName";
    public float BroadcastRange = 5f;

    [HideInInspector]
    public List<MotifNote> Notes;

    // Delegate required because of compilation order
    public delegate void OpenMotifEditorDelegate(DryadMotif landscape);
    public static OpenMotifEditorDelegate OnOpenMotifEditor;

    public void OpenMotifEditor()
    {
        if (Notes == null)
            Notes = new List<MotifNote>();

        if (OnOpenMotifEditor != null)
            OnOpenMotifEditor(this);
    }

    public DryadMotif()
    {
    }

    public override int GetHashCode()
    {
        return 42
            ^ BroadcastRange.GetHashCode()
            ^ Name.GetHashCode()
            ^ Voice.GetHashCode();
    }

    public override bool Equals(object other)
    {
        if(other.GetType() != typeof(DryadMotif))
            return false;
        if (object.ReferenceEquals(other, this))
            return true;

        return Equals(other as DryadMotif);
    }

    public bool Equals(DryadMotif other)
    {
        return BroadcastRange == other.BroadcastRange
            && Name == other.Name
            && Voice == other.Voice;
    }
}
