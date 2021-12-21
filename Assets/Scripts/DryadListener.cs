using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DryadListener : MonoBehaviour
{
    List<DryadMotif> _motifs = new List<DryadMotif>();
    DryadGlobal _global;
    DryadLandscape _currentLandscape;
    DryadLandscape _previousLandscape;

    [SerializeField]
    public string Name { get; set; }

    public DryadLandscape GetCurrentLandscape()
    {
        return _currentLandscape;
    }

    public DryadLandscape GetPreviousLandscape()
    {
        return _previousLandscape;
    }


    void Start()
    {
        if (Name == null)
            Name = "Paul";

        _global = DryadGlobal.GetInstance();
        if (_global == null)
            Debug.LogError("No DryadGlobal accessible!");
        else
            _global.Register(this);
    }

    void OnDestroy()
    {
        if(_global != null)
            _global.Unregister(this);
    }

    void Update()
    {
        foreach(DryadMotif motif in _motifs)
            Debug.DrawLine(transform.position, motif.transform.position);
    }

    public void ClearMotifs()
    {
        _motifs.Clear();
    }

    public void AddMotif(DryadMotif motif)
    {
        _motifs.Add(motif);
    }

    public void RemoveMotif(DryadMotif motif)
    {
        if (_motifs.Contains(motif))
            _motifs.Remove(motif);
        else
            Debug.LogError($"Motif {motif.Name} note present in listener");
    }

    private void OnTriggerEnter(Collider other)
    {
        DryadLandscape landscape = other.gameObject.GetComponent<DryadLandscape>();
        if (landscape != null && landscape != _currentLandscape)
        {
            _previousLandscape = _currentLandscape;
            _currentLandscape = landscape;
        }
    }

    /*
    private void OnTriggerExit(Collider other)
    {
        DryadLandscape landscape = other.gameObject.GetComponent<DryadLandscape>();
        if (landscape != null)
            _previousLandscape = landscape;
    }
    */

    public List<DryadMotif> GetMotifs()
    {
        return _motifs;
    }

}
