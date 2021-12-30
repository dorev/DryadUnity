using UnityEngine;
using UnityEditor;
using System.Collections.Generic;

[CustomEditor(typeof(DryadMotif))]
public class DryadMotifInspector : Editor
{
    DryadMotif script;

    private void OnEnable()
    {
        script = target as DryadMotif;
    }

    public override void OnInspectorGUI()
    {
        base.OnInspectorGUI();

        if (GUILayout.Button("Open Motif Editor"))
            script.OpenMotifEditor();

        if (GUI.changed)
            EditorUtility.SetDirty(script);
    }

    private void DrawGrid()
    {
        int widthDivs = 4;
        int heightDivs = 10;

        Handles.BeginGUI();
        Handles.color = Color.gray;
        float gridSpacing = 20;

        Vector2 offset = Vector2.zero;
        Vector2 position = Vector2.zero;
        Vector3 newOffset = new Vector3(offset.x % gridSpacing, offset.y % gridSpacing, 0);

        for (int i = 0; i < widthDivs; i++)
            Handles.DrawLine(new Vector3(gridSpacing * i, -gridSpacing, 0) + newOffset, new Vector3(gridSpacing * i, position.y, 0f) + newOffset);

        for (int j = 0; j < heightDivs; j++)
            Handles.DrawLine(new Vector3(-gridSpacing, gridSpacing * j, 0) + newOffset, new Vector3(position.x, gridSpacing * j, 0f) + newOffset);

        Handles.color = Color.white;
        Handles.EndGUI();
    }
}
