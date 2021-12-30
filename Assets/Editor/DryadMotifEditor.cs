using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;

/*
 * Complete motif
 *  - midi like UI?
 *  - data persistence
 * IDryadPlayer / DryadGlobalTick
 * Test audio output with MPTK
 * C++ interop
 */

public class DryadMotifEditor : EditorWindow
{
    [SerializeField]
    public DryadMotif Motif;

    private bool dataHasChanged = false;

    private Vector2 drag;
    private Vector2 offset;

    [InitializeOnLoadMethod]
    static void StaticInit()
    {
        DryadMotif.OnOpenMotifEditor = ShowWindow;
    }

    public static void ShowWindow(DryadMotif motif)
    {
        DryadMotifEditor window = GetWindow<DryadMotifEditor>();
        window.InitMotifEditor(motif);
        window.titleContent = new GUIContent("Motif Editor");
    }

    void ClearMotifEditor()
    {

    }

    private void OnSelectionChange()
    {
        if (Selection.activeTransform == null)
            return;

        GameObject obj = Selection.activeTransform.gameObject;

        if (obj == null)
            return;

        DryadMotif motifSelected = obj.GetComponent<DryadMotif>();

        if(motifSelected != null && motifSelected != Motif)
        {
            if(Motif != null)
            {
                SaveMotifData();
                ClearMotifEditor();
            }
            InitMotifEditor(motifSelected);
            Repaint();
        }
    }


    private void InitMotifEditor(DryadMotif motif)
    {
        if (motif == null)
            throw new System.Exception("Null motif provided to editor initializer");

        Motif = motif;

    }
 

    private void OnGUI()
    {
        if (dataHasChanged)
            SaveMotifData();

        DrawGrid(20, 0.2f, Color.gray);
        DrawGrid(100, 0.4f, Color.gray);

        if (Motif == null)
        {
            EditorGUILayout.HelpBox("No DryadMotif selected",MessageType.Warning);
            return;
        }

        GUILayout.Label(Motif.Name, EditorStyles.boldLabel);

        if (GUI.changed)
            Repaint();
    }

    void SaveMotifData()
    {
        EditorUtility.SetDirty(Motif);
        dataHasChanged = false;
    }

    private void DrawGrid(float gridSpacing, float gridOpacity, Color gridColor)
    {
        int widthDivs = Mathf.CeilToInt(position.width / gridSpacing);
        int heightDivs = Mathf.CeilToInt(position.height / gridSpacing);
 
        Handles.BeginGUI();
        Handles.color = new Color(gridColor.r, gridColor.g, gridColor.b, gridOpacity);
 
        offset += drag * 0.5f;
        Vector3 newOffset = new Vector3(offset.x % gridSpacing, offset.y % gridSpacing, 0);
 
        for (int i = 0; i < widthDivs; i++)
            Handles.DrawLine(new Vector3(gridSpacing * i, -gridSpacing, 0) + newOffset, new Vector3(gridSpacing * i, position.height, 0f) + newOffset);
 
        for (int j = 0; j < heightDivs; j++)
            Handles.DrawLine(new Vector3(-gridSpacing, gridSpacing * j, 0) + newOffset, new Vector3(position.width, gridSpacing * j, 0f) + newOffset);
 
        Handles.color = Color.white;
        Handles.EndGUI();
    }
}
