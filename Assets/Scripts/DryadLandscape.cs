using System.Collections.Generic;
using UnityEngine;

// All this is expected to come from the SWIG classes


[System.Serializable]
public class LandscapeNodeData
{
    public uint Id;
    public DryadUnity.Chord Chord;
    public List<uint> Edges;
    public Rect Rect;

    public LandscapeNodeData(uint id, DryadUnity.Chord chord, List<uint> edges, Rect position)
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
    public DryadUnity.Scale Scale;
    [SerializeField]
    public List<LandscapeNodeData> NodesData;

    // Delegate required because of compilation order
    public delegate void OpenGraphEditorDelegate(DryadLandscape landscape);
    public static OpenGraphEditorDelegate OnOpenLandscapeEditor;
     
    public void OpenLandscapeEditor()
    {
        if (NodesData == null)
            NodesData = new List<LandscapeNodeData>();

        OnOpenLandscapeEditor?.Invoke(this);
    }
}
