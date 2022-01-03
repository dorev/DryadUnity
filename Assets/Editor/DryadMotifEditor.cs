using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;

// Add MotifLength line
// Motif notes
//  - Shape must "snap" to grid
//  - Must be able to stretch notes (draggable "On" and "Off" regions)
//  - Right-click to create or delete note ('C' and 'D' shortcuts?)

public class DryadMotifEditor : DryadEditorBase
{
    #region Members

    public DryadMotif Motif;

    public float GridUnitSize = 24;
    public float MinGridUnitSize = 12;
    public float MaxGridUnitSize = 48;
    public Vector2 debugDrag;

    private List<DryadMotifNote> notes = new List<DryadMotifNote>();

    #endregion

    #region Initialization

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
        window.minSize = new Vector2(100, 100);
        window.position = new Rect(0, 0, 100, 100);
    }

    private void InitMotifEditor(DryadMotif motif)
    {
        if (motif == null)
            throw new System.Exception("Null motif provided to editor initializer");

        Motif = motif;

    }

    #endregion

    #region Drawing

    private void OnGUI()
    {
        if (dataHasChanged)
            SaveMotifData();

        DrawGrid();

        if (Motif == null)
        {
            EditorGUILayout.HelpBox("No DryadMotif selected", MessageType.Warning);
            return;
        }

        GUI.Label(new Rect(GridUnitSize, 0, 500, EditorGUIUtility.singleLineHeight), Motif.Name, EditorStyles.boldLabel);

        DrawNotes();

        ProcessNoteEvents(Event.current);
        ProcessEvents(Event.current);

        GUI.Label(new Rect(GridUnitSize, GridUnitSize, 500, EditorGUIUtility.singleLineHeight),
            $"Offset: {offset}   Drag: {debugDrag}");

        if (GUI.changed)
            Repaint();
    }

    void DrawGrid()
    {
        // Grid indicating measures and octaves
        DrawHorizontalGrid(8 * GridUnitSize, 0.8f, Color.gray);
        DrawHorizontalGrid(8 * GridUnitSize, 0.8f, Color.gray, 1 * GridUnitSize);
        DrawHorizontalGrid(1 * GridUnitSize, 0.2f, Color.gray);
        DrawVerticalGrid(16 * GridUnitSize, 0.8f, Color.gray);
        DrawVerticalGrid(4 * GridUnitSize, 0.4f, Color.gray);
        DrawVerticalGrid(1 * GridUnitSize, 0.2f, Color.gray);

        // Line indicating the beginning and center of the motif
        DrawVerticalLine(0, 1.0f, Color.red);
        DrawHorizontalLine(0, 1.0f, Color.red);
        DrawHorizontalLine(GridUnitSize, 1.0f, Color.red);

        // Tonic offset labels
        DrawLabels(Color.white);
    }

    void DrawNotes()
    {
        foreach (DryadMotifNote note in notes)
            note.Draw();
    }

    private void DrawVerticalLine(float x, float lineOpacity, Color lineColor)
    {
        Handles.BeginGUI();
        Handles.color = new Color(lineColor.r, lineColor.g, lineColor.b, lineOpacity);

        offset += drag * 0.5f;

        Vector3 newOffset = new Vector3(offset.x, offset.y, 0);

        Handles.DrawLine(new Vector3(x + newOffset.x, 0.0f, 0.0f), new Vector3(x + newOffset.x, position.height, 0.0f));

        Handles.color = Color.white;
        Handles.EndGUI();
    }

    private void DrawHorizontalLine(float y, float lineOpacity, Color lineColor)
    {
        Handles.BeginGUI();
        Handles.color = new Color(lineColor.r, lineColor.g, lineColor.b, lineOpacity);

        offset += drag * 0.5f;

        Vector3 newOffset = new Vector3(offset.x, offset.y, 0);

        Handles.DrawLine(new Vector3(0.0f, y + newOffset.y, 0.0f), new Vector3(position.width, y + newOffset.y, 0.0f));

        Handles.color = Color.white;
        Handles.EndGUI();
    }

    private void DrawHorizontalGrid(float gridSpacing, float gridOpacity, Color gridColor, float shift = 0.0f)
    {
        int heightDivs = Mathf.CeilToInt(position.height / gridSpacing);

        Handles.BeginGUI();
        Handles.color = new Color(gridColor.r, gridColor.g, gridColor.b, gridOpacity);

        offset += drag * 0.5f;

        Vector3 newOffset = new Vector3(offset.x % gridSpacing, offset.y % gridSpacing, 0.0f);

        for (int j = 0; j < heightDivs; j++)
            Handles.DrawLine(new Vector3(-gridSpacing, gridSpacing * j + shift, 0.0f) + newOffset, new Vector3(position.width, gridSpacing * j + shift, 0.0f) + newOffset);

        Handles.color = Color.white;
        Handles.EndGUI();
    }

    private void DrawVerticalGrid(float gridSpacing, float gridOpacity, Color gridColor, float shift = 0.0f)
    {
        int widthDivs = Mathf.CeilToInt(position.width / gridSpacing);

        Handles.BeginGUI();
        Handles.color = new Color(gridColor.r, gridColor.g, gridColor.b, gridOpacity);

        offset += drag * 0.5f;

        Vector3 newOffset = new Vector3(offset.x % gridSpacing, offset.y % gridSpacing, 0.0f);

        for (int i = 0; i < widthDivs; i++)
            Handles.DrawLine(new Vector3(gridSpacing * i + shift, -gridSpacing, 0.0f) + newOffset, new Vector3(gridSpacing * i + shift, position.height, 0.0f) + newOffset);

        Handles.color = Color.white;
        Handles.EndGUI();
    }

    private void DrawLabels(Color labelColor)
    {

        int labelCount = Mathf.FloorToInt(position.height / GridUnitSize);
        offset += drag * 0.5f;
        int topLabel = Mathf.FloorToInt(offset.y / GridUnitSize);

        for (int i = 0; i < labelCount; ++i)
        {
            float nudge = GridUnitSize / 4;
            float y = i * GridUnitSize + (offset.y % GridUnitSize) + nudge;

            Rect labelRect = new Rect(nudge, y, EditorGUIUtility.labelWidth, EditorGUIUtility.singleLineHeight);
            GUI.Label(labelRect, (topLabel - i).ToString());
        }
    }

    #endregion

    #region Events

    void ProcessEvents(Event e)
    {
        drag = Vector2.zero;

        switch (e.type)
        {
            case EventType.MouseDown:
                if (e.button == 1)
                    OnAddNote(e.mousePosition);
                break;

            case EventType.MouseDrag:
                if (e.button == 2)
                    OnDrag(e.delta);
                break;

            case EventType.ScrollWheel:
                OnMouseScroll(e.delta.y);
                break;
        }
    }

    private void ProcessNoteEvents(Event e)
    {
        foreach (DryadMotifNote note in notes) // might need to do the iteration backward to redraw in order of addition
        {
            if (note.ProcessEvents(e))
                GUI.changed = true;
        }
    }

    void OnMouseScroll(float delta)
    {
        if ((GridUnitSize < MinGridUnitSize && delta > 0)
        ||  (GridUnitSize > MaxGridUnitSize && delta < 0))
            return;

        GridUnitSize -= delta;
        GUI.changed = true;
    }

    void OnAddNote(Vector2 mousePosition)
    {
        (uint scoreTime, int tonicOffset) data = PositionToScoreTimeAndTonicOffset(mousePosition);

        notes.Add(new DryadMotifNote(
            this,
            SnapToGrid(mousePosition),
            DryadMotifNote.Quarter,
            data.scoreTime,
            data.tonicOffset
        ));
        GUI.changed = true;
    }

    private void OnDrag(Vector2 delta)
    {
        drag = delta;

        if ((drag.x + offset.x) > GridUnitSize)
            drag.x = 0;

        debugDrag = drag;

        foreach (DryadMotifNote note in notes)
            note.Drag(drag * 5.0f);

        GUI.changed = true;
    }

    #endregion

    #region Utilities

    void ClearMotifEditor()
    {

    }

    private void OnSelectionChange()
    {
        DryadMotif motifSelected = GetComponentFromSelection<DryadMotif>();

        if (motifSelected != null && motifSelected != Motif)
        {
            if (Motif != null)
            {
                SaveMotifData();
                ClearMotifEditor();
            }
            InitMotifEditor(motifSelected);
            Repaint();
        }
    }

    private void SaveMotifData()
    {
        EditorUtility.SetDirty(Motif);
        dataHasChanged = false;
    }

    public Vector2 SnapToGrid(Vector2 position)
    {
        float x = position.x - (position.x % GridUnitSize) + (offset.x % GridUnitSize);
        float y = position.y - (position.y % GridUnitSize) + (offset.y % GridUnitSize);
        return new Vector2(x, y);
    }

    public (uint scoreTime, int tonicOffset) PositionToScoreTimeAndTonicOffset(Vector2 position)
    {
        Vector2 snapPosition = SnapToGrid(position);

        int topmostTonicOffset = Mathf.FloorToInt(offset.y / GridUnitSize);
        int deltaTonicOffset = Mathf.FloorToInt(snapPosition.y / GridUnitSize);
        int tonicOffset = topmostTonicOffset - deltaTonicOffset;

        // Removing 1 because the first grid contains the labels
        uint scoreTime = (uint) (Mathf.FloorToInt(snapPosition.x / GridUnitSize) - 1);
        scoreTime *= DryadMotifNote.Sixteenth;

        // Correct scoreTime to match grid expected value
        if (scoreTime % DryadMotifNote.Sixteenth != 0)
            scoreTime -= scoreTime % DryadMotifNote.Sixteenth;

        return (scoreTime, tonicOffset);
    }

    public Vector2 ScoreTimeAndTonicOffsetToPosition(uint scoreTime, int tonicOffset)
    {
        float x = scoreTime / DryadMotifNote.Sixteenth * GridUnitSize;
        float y = tonicOffset * -1.0f * GridUnitSize;
        return new Vector2(x, y) + offset;
    }

    #endregion

}
