using System.Collections.Generic;
using UnityEngine;
using UnityEditor;

[CustomEditor(typeof(DryadLandscape))]
public class DryadLandscapeEditor : Editor
{
    DryadLandscape script;

    private void OnEnable()
    {
        script = target as DryadLandscape;
    }

    public override void OnInspectorGUI()
    {
        base.OnInspectorGUI();
        if (GUILayout.Button("Open Graph Editor"))
            script.OpenGraphEditor();
    }
}
