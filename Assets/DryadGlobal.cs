using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;


public class DryadGlobal : MonoBehaviour
{
    [SerializeField]
    public float RefreshPeriod
    {
        get
        {
            return RefreshPeriod;
        }
        set
        {
            RefreshPeriod = value;
            _slowPeriod = new WaitForSeconds(value);
        }
    }

    static DryadGlobal _instance;
    WaitForSeconds _slowPeriod;
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

    void Start()
    {
        _status = Status.Init;
        _slowPeriod = new WaitForSeconds(RefreshPeriod);
        StartCoroutine(SlowPeriodicWork());
        _status = Status.Running;

        SetupDebugUI();
    }

    void Update()
    {
        UpdateDebugUI();
    }

    void Shutdown()
    {
        _status = Status.ShuttingDown;
        StopCoroutine(SlowPeriodicWork());
    }

     IEnumerator SlowPeriodicWork()
     {
         while (_status != Status.ShuttingDown)
         {
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
            listener.ClearMotifs();
            foreach(DryadMotif motif in FindObjectsOfType<DryadMotif>())
            {
                float distance = Vector3.Distance(listener.transform.position, motif.transform.position);
                if (distance < motif.Range)
                    listener.AddMotif(motif);
            }
        }
     }

    // Debug ///////////////////////////////////////////////////////////////////////////////////////////////////////////

    GameObject _canvasContainer;
    Canvas _canvas;
    GameObject _debugContainer;
    Text _debugText;
    RectTransform _debugTransform;
    void SetupDebugUI()
    {
        _canvasContainer = new GameObject();
        _canvasContainer.name = "DryadDebugUI";
        _canvas = _canvasContainer.AddComponent<Canvas>();
        _canvas.renderMode = RenderMode.ScreenSpaceOverlay;
        _canvasContainer.AddComponent<CanvasScaler>();
        _canvasContainer.AddComponent<GraphicRaycaster>();

        _debugContainer = new GameObject();
        _debugContainer.name = "Debug";
        _debugContainer.transform.parent = _canvasContainer.transform;
        _debugText = _debugContainer.AddComponent<Text>();
        _debugText.text = "debug debug debug";
        _debugText.font = (Font)Resources.GetBuiltinResource(typeof(Font), "Arial.ttf");
        _debugText.fontSize = 14;
        _debugTransform = _debugText.GetComponent<RectTransform>();
        _debugTransform.localPosition = new Vector3(-300, 100, 0);
        _debugTransform.sizeDelta = new Vector2(400, 200);
    }

    void UpdateDebugUI()
    {
        if (_canvasContainer == null)
            return;
        _debugText.text = "";

        foreach (DryadListener listener in _listeners)
        {
            _debugText.text += $"Listener {listener.Name}\n";

            if (listener.GetCurrentLandscape())
                _debugText.text += $"Current landscape: {listener.GetCurrentLandscape()?.Name}\n";

            if (listener.GetPreviousLandscape())
                _debugText.text += $"Previous landscape: {listener.GetPreviousLandscape()?.Name}\n";

            _debugText.text += $"Motifs:\n";

            foreach (DryadMotif motif in listener.GetMotifs())
                _debugText.text += $"  - {motif.Name}\n";
        }
    }
}
