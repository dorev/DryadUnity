using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DryadListener : MonoBehaviour
{
    List<DryadMotif> motifs = new List<DryadMotif>();
    DryadGlobal global;
    DryadLandscape currentLandscape;
    DryadLandscape previousLandscape;

    [SerializeField]
    public string Name { get; set; }

    public DryadLandscape GetCurrentLandscape()
    {
        return currentLandscape;
    }

    public DryadLandscape GetPreviousLandscape()
    {
        return previousLandscape;
    }

    void Start()
    {
        if (Name == null)
            Name = "Paul";

        global = DryadGlobal.GetInstance();
        if (global == null)
            Debug.LogError("No DryadGlobal accessible!");
        else
            global.Register(this);
    }

    void OnDestroy()
    {
        if(global != null)
            global.Unregister(this);
    }

    void Update()
    {
        foreach(DryadMotif motif in motifs)
            Debug.DrawLine(transform.position, motif.transform.position);
    }

    public void ClearMotifs()
    {
        motifs.Clear();
    }

    public void AddMotif(DryadMotif motif)
    {
        motifs.Add(motif);
    }

    public void RemoveMotif(DryadMotif motif)
    {
        if (motifs.Contains(motif))
            motifs.Remove(motif);
        else
            Debug.LogError($"Motif {motif.Name} note present in listener");
    }

    private void OnTriggerEnter(Collider other)
    {
        DryadLandscape landscape = other.gameObject.GetComponent<DryadLandscape>();
        if (landscape != null && landscape != currentLandscape)
        {
            previousLandscape = currentLandscape;
            currentLandscape = landscape;
        }
    }

    public List<DryadMotif> GetMotifs()
    {
        return motifs;
    }

}
