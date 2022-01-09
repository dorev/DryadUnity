using System.Collections.Generic;
using System.Linq;
using UnityEngine;

public class DryadListener : MonoBehaviour
{
    List<DryadMotif> motifs = new List<DryadMotif>();
    DryadGlobal global;
    DryadLandscape currentLandscape;
    DryadLandscape previousLandscape;

    [SerializeField]
    public string Name { get; set; }

    [HideInInspector]
    public bool HasChanged = true;

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
            HasChanged = true;
        }
    }

    public List<DryadMotif> GetMotifs()
    {
        return motifs;
    }

    public void CompareUpdateWithSurroundingMotifs(List<DryadMotif> surroundingMotifs)
    {
        IEnumerable<DryadMotif> distinctMotifs = motifs.Distinct();
        List<DryadMotif> motifsToRefresh = new List<DryadMotif>();

        // Compare count of motifs with similar names.
        // If the name count of a motif is different, add or remove the appropriate count of the motif
        foreach(DryadMotif motif in distinctMotifs)
        {
            int surroundingMotifCount = surroundingMotifs.Where(item => item.Name == motif.Name).Count();
            int currentMotifCount = surroundingMotifs.Where(item => item.Name == motif.Name).Count();

            if (surroundingMotifCount != currentMotifCount)
            {
                int delta = surroundingMotifCount - currentMotifCount;
                if(delta > 0)
                {
                    for (int i = 0; i < delta; ++i)
                        motifs.Add(motif);
                }
                else
                {
                    for (int i = 0; i > delta; --i)
                        motifs.Remove(motif);
                }
                HasChanged = true;
            }
        }
    }

}
