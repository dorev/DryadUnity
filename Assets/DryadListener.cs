using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DryadListener : MonoBehaviour
{
    Dictionary<DryadMotif, uint> _motifsCache = new Dictionary<DryadMotif, uint>();
    DryadGlobal _global;

    // Start is called before the first frame update
    void Start()
    {
        _global = DryadGlobal.GetInstance();
        if (_global == null)
            Debug.LogError("No DryadGlobal accessible!");
        _global.Register(this);
    }

    void OnDestroy()
    {
        _global.Unregister(this);
    }

    void Update()
    {
        foreach(KeyValuePair<DryadMotif, uint> item in _motifsCache)
        {
            if (item.Value > 0)
                Debug.DrawLine(transform.position, item.Key.transform.position);
        }
    }

    public void ResetMotifCounts()
    {
        List<DryadMotif> keys = new List<DryadMotif>(_motifsCache.Keys);
        foreach(var key in keys)
            _motifsCache[key] = 0;
    }

    public void IncrementMotifCount(DryadMotif motif, uint amount = 1)
    {
        if (!_motifsCache.ContainsKey(motif))
            _motifsCache.Add(motif, 0);
        _motifsCache[motif] += amount;
        Debug.Log($"Set motif {motif.Name} count to {_motifsCache[motif]}");
    }

    public void DecrementMotifCount(DryadMotif motif, uint amount = 1)
    {
        uint currentCount = _motifsCache[motif];
        if (amount > currentCount)
        {
            _motifsCache[motif] = 0;
            Debug.LogError($"Motif {motif.Name} count underflow");
        }
        else
            _motifsCache[motif] -= amount;
    }
}
