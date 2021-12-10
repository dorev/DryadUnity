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
    public float Range { get; set; }
    public string Name { get; set; }
    public string Voice { get; set; }

    public DryadMotif()
    {
        Range = 3.0f;
    }
}
