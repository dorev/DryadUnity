using System.Collections;
using System.Collections.Generic;
using UnityEngine;


public class DryadGlobal : MonoBehaviour
{
    static DryadGlobal _instance;
    WaitForSeconds _slowPeriod = new WaitForSeconds(1f);
    List<DryadListener> _listeners = new List<DryadListener>();

    enum Status
    {
        Init,
        Running,
        ShuttingDown
    };
    Status _status;

    public static DryadGlobal GetInstance()
    {
        return _instance;
    }

    void Awake()
    {
        // Prefab/Singleton check
        if(_instance != null && _instance != this)
            Destroy(this.gameObject);
        else
            _instance = this;
    }

    void Shutdown()
    {
        _status = Status.ShuttingDown;
        StopCoroutine(SlowPeriodicWork());
    }

    // Start is called before the first frame update
    void Start()
    {
        _status = Status.Init;
        StartCoroutine(SlowPeriodicWork());
        _status = Status.Running;
    }

     IEnumerator SlowPeriodicWork()
     {
         while (_status != Status.ShuttingDown)
         {
            Debug.Log("Checking listeners/motifs ranges");
            UpdateListenersMotifs();
            yield return _slowPeriod;
         }
     }

     public void Register(DryadListener listener)
     {
        if(!_listeners.Contains(listener))
        {
            _listeners.Add(listener);
            Debug.Log("Added listener");
        }
     }

     public void Unregister(DryadListener listener)
     {
        if (_listeners.Contains(listener))
        {
            _listeners.Remove(listener);
            Debug.Log("Added listener");
        }
     }

     void UpdateListenersMotifs()
     {
        // Check what motifs are in range of listeners
        foreach(DryadListener listener in _listeners)
        {
            listener.ResetMotifCounts();
            foreach(DryadMotif motif in FindObjectsOfType<DryadMotif>())
            {
                float distance = Vector3.Distance(listener.transform.position, motif.transform.position);
                if (distance < motif.Range)
                    listener.IncrementMotifCount(motif);
            }
        }
     }

    void FixedUpdate()
    {
        
    }
}
