using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;

public class DryadMotifNote
{
    public void Drag(Vector2 delta)
    {

    }
}

public class DryadMotifGridRectangle
{

}

public class DryadMotifEditor : DryadEditorBase
{
    [SerializeField]
    public DryadMotif Motif;

    private float GridUnitSize = 24;
    private float MinGridUnitSize = 12;
    private float MaxGridUnitSize = 48;

    private List<DryadMotifNote> notes;

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

        DryadMotif motifSelected = GetGameObjectFromSelection<DryadMotif>(); ;

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


    // Add MotifLength line
    // Motif notes
    //  - Shape must "snap" to grid
    //  - Must be able to stretch notes (draggable "On" and "Off" regions)
    //  - Right-click to create or delete note ('C' and 'D' shortcuts?)


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

        GUILayout.Label(Motif.Name, EditorStyles.boldLabel);

        ProcessEvents(Event.current);

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

        // Scale offset labels
        DrawLabels(Color.white);
        GUI.Label(DefaultLabelRect(0, 0), $"Offset: {offset.ToString()}");
    }

    void ProcessEvents(Event e)
    {
        drag = Vector2.zero;

        switch (e.type)
        {
            case EventType.MouseDown:
                if (e.button == 1)
                    AddNote(e.mousePosition);
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

    void OnMouseScroll(float delta)
    {
        if ((GridUnitSize < MinGridUnitSize && delta > 0)
        ||  (GridUnitSize > MaxGridUnitSize && delta < 0))
            return;

        GridUnitSize -= delta;
        GUI.changed = true;
    }

    void AddNote(Vector2 mousePosition)
    {

    }

    private void OnDrag(Vector2 delta)
    {
        drag = delta;

        if (notes != null)
        {
            foreach (DryadMotifNote note in notes)
                note.Drag(delta);
        }

        GUI.changed = true;
    }

    void SaveMotifData()
    {
        EditorUtility.SetDirty(Motif);
        dataHasChanged = false;
    }

    private void DrawVerticalLine(float x, float lineOpacity, Color lineColor)
    {
        Handles.BeginGUI();
        Handles.color = new Color(lineColor.r, lineColor.g, lineColor.b, lineOpacity);

        offset += drag * 0.5f;

        if (offset.x > GridUnitSize)
            offset.x = GridUnitSize;

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

        if (offset.x > GridUnitSize)
            offset.x = GridUnitSize;

        Vector3 newOffset = new Vector3(offset.x % gridSpacing, offset.y % gridSpacing, 0.0f);

        for (int i = 0; i < widthDivs; i++)
            Handles.DrawLine(new Vector3(gridSpacing * i + shift, -gridSpacing, 0.0f) + newOffset, new Vector3(gridSpacing * i + shift, position.height, 0.0f) + newOffset);

        Handles.color = Color.white;
        Handles.EndGUI();
    }

    void DrawLabels(Color labelColor)
    {

        int labelCount = Mathf.FloorToInt(position.height / GridUnitSize);
        offset += drag * 0.5f;
        int topLabel = Mathf.FloorToInt(offset.y / GridUnitSize);

        for (int i = 0; i < labelCount; ++i)
        {
            float nudge= GridUnitSize / 4;
            float y = i * GridUnitSize + (offset.y % GridUnitSize) + nudge;

            Rect labelRect = new Rect(nudge, y, EditorGUIUtility.labelWidth, EditorGUIUtility.singleLineHeight);
            GUI.Label(labelRect, (topLabel - i).ToString());
        }
    }
}
