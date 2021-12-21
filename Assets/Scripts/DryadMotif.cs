using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MotifNote
{
    float Duration { get; set; }
    int Offset { get; set; }
}

public class DryadMotif : MonoBehaviour
{
    // https://unity3d.college/2017/05/22/unity-attributes/

    public float Range = 5f;
    public string Name = "NoMotifName";
    public string Voice = "NoVoiceName";

    public DryadMotif()
    {
    }

    public override int GetHashCode()
    {
        return 42
            ^ Range.GetHashCode()
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
        return Range == other.Range 
            && Name == other.Name
            && Voice == other.Voice;
    }
}
