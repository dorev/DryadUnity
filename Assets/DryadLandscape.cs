using System.Collections.Generic;
using UnityEngine;
using UnityEditor;

namespace Dryad
{
    [System.Serializable]
    public class HarmonyNode
    {
        public uint Degree;
        public List<uint> Edges;
    }
}

public class DryadLandscape : MonoBehaviour
{
    public string Name;
    public List<Dryad.HarmonyNode> Nodes;

    public delegate void OpenGraphEditorDelegate();
    public static OpenGraphEditorDelegate OnOpenGraphEditor;
     
    public void OpenGraphEditor()
    {
        if (OnOpenGraphEditor != null)
            OnOpenGraphEditor();
    }
}
