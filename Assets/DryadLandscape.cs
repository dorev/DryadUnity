using System.Collections;
using System.Collections.Generic;
using UnityEngine;


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

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
