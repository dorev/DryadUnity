using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;

public class DryadMotifEditor : DryadEditorBase
{
    #region Members

    [SerializeField]
    public DryadMotif Motif;

    public float GridUnitSize = 24;

    public Rect MotifDurationRect;
    public GUIStyle MotifDurationStyle;
    public bool isDraggingMotifDuration;
    public uint Duration = 0;

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
    }

    private void InitMotifEditor(DryadMotif motif)
    {
        if (motif == null)
            throw new System.Exception("Null motif provided to editor initializer");

        Motif = motif;
        MotifDurationStyle = new GUIStyle();
        MotifDurationStyle.normal.background = DryadEditorUtility.ColorTexture(Color.white, 0.5f);
        MotifDurationRect = new Rect(0, 0, GridUnitSize, Screen.height);

        MotifDurationRect.x = (Motif.Duration + 1) / Dryad.Duration.Sixteenth * GridUnitSize;
        MotifDurationRect.y = 0;
        MotifDurationRect.width = GridUnitSize;
        MotifDurationRect.height = Screen.height;

        Duration = motif.Duration;

        foreach(MotifNoteData noteData in motif.NotesData)
            notes.Add(new DryadMotifNote(noteData, this, OnClickRemoveNote, OnDragStretchNote));

        OnCenterEditor();

        GUI.changed = true;
        dataHasChanged = true;
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

        CheckAndFixGlitchedData();

        DrawNotes();
        ProcessNoteEvents(Event.current);
        ProcessEvents(Event.current);

        if (Motif != null)
            DrawMotifDurationEnd();
        
        GUI.Label(DryadEditorUtility.DebugTextRect(GridUnitSize, GridUnitSize), $"Offset: {offset} Duration: {Duration}");

        if (GUI.changed)
            Repaint();
    }

    void CheckAndFixGlitchedData()
    {
        if (Duration > 9999)
        {
            Duration = 0;
            dataHasChanged = true;
            GUI.changed = true;
        }

        List<DryadMotifNote> notesToDelete = new List<DryadMotifNote>();
        foreach(DryadMotifNote note in notes)
        {
            if (note.ScoreTime < 0
            || note.ScoreTime > 9999
            || note.Duration < 0
            || note.Duration > 9999)
                notesToDelete.Add(note);
        }

        foreach (DryadMotifNote note in notesToDelete)
            notes.Remove(note);

        if(notesToDelete.Count > 0)
        {
            dataHasChanged = true;
            GUI.changed = true;
        }
    }

    void DrawGrid()
    {
        // Force limit on dragging right
        if (offset.x > GridUnitSize)
            offset.x = GridUnitSize;

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

    void DrawMotifDurationEnd()
    {
        // Check if newly stretched notes extented the motif duration
        if(!isDraggingMotifDuration)
        {
            uint lastNoteEndTime = 0;
            foreach (DryadMotifNote note in notes)
            {
                uint noteEndTime = note.ScoreTime + note.Duration;
                if (noteEndTime > lastNoteEndTime)
                    lastNoteEndTime = noteEndTime;
            }

            if (Duration < lastNoteEndTime)
            { 
                Duration = lastNoteEndTime;
                dataHasChanged = true;
            }

            MotifDurationRect.x = Duration / Dryad.Duration.Sixteenth * GridUnitSize + offset.x;
        }

        MotifDurationRect.y = 0;
        MotifDurationRect.width = GridUnitSize;
        MotifDurationRect.height = Screen.height;
        GUI.Box(MotifDurationRect, "", MotifDurationStyle);
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
                else if (e.button == 0)
                {
                    if (MotifDurationRect.Contains(e.mousePosition))
                        isDraggingMotifDuration = true;
                    else if (e.clickCount == 2)
                        OnCenterEditor();
                }
                break;

            case EventType.MouseUp:
                if (isDraggingMotifDuration)
                {
                    UpdateMotifDurationAfterDrag();
                    isDraggingMotifDuration = false;
                }
                break;

            case EventType.MouseDrag:
                if (e.button == 0)
                {
                    if (isDraggingMotifDuration)
                        OnDragMotifDuration(e.delta);
                    else
                        OnDragGrid(e.delta);
                }
                break;
        }
    }

    private void ProcessNoteEvents(Event e)
    {
        foreach (DryadMotifNote note in notes) // might need to do the iteration backward to redraw in order of addition
        {
            if (note.ProcessEvents(e))
            {
                GUI.changed = true;
                dataHasChanged = true;
            }
        }
    }

    void OnCenterEditor()
    {
        offset.x = GridUnitSize;
        offset.y = Screen.height / 2.0f;
        GUI.changed = true;
    }

    void OnDragMotifDuration(Vector2 delta)
    {
        MotifDurationRect.position += delta;
        GUI.changed = true;
    }

    void OnDragStretchNote(DryadMotifNote note, DryadMotifNote.StretchDirection stretchDirection, Vector2 delta)
    {
        switch (stretchDirection)
        {
            case DryadMotifNote.StretchDirection.Front:
                note.Rect.x += delta.x;
                note.Rect.width -= delta.x;
                break;

            case DryadMotifNote.StretchDirection.Back:
                note.Rect.width += delta.x;
                break;

            case DryadMotifNote.StretchDirection.None:
                break;
        }

        dataHasChanged = true;
    }

    void OnAddNote(Vector2 mousePosition)
    {
        (uint scoreTime, int tonicOffset) noteData = PositionToScoreTimeAndTonicOffset(mousePosition);

        notes.Add(new DryadMotifNote(
            this,
            Dryad.Duration.Quarter,
            noteData.scoreTime,
            noteData.tonicOffset,
            OnClickRemoveNote,
            OnDragStretchNote
        ));

        GUI.changed = true;
        dataHasChanged = true;
    }

    private void OnClickRemoveNote(DryadMotifNote note)
    {
        notes?.Remove(note);
    }

    private void OnDragGrid(Vector2 delta)
    {
        drag = delta;

        if ((drag.x + offset.x) > GridUnitSize)
            drag.x = 0;

        foreach (DryadMotifNote note in notes)
            note.Drag(drag * 5.0f);

        GUI.changed = true;
    }

    #endregion

    #region Utilities

    void ClearMotifEditor()
    {
        notes.Clear();
        Motif = null;
        isDraggingMotifDuration = false;
        Duration = 0;
        Repaint();
    }

    private void OnSelectionChange()
    {
        DryadMotif motifSelected = DryadEditorUtility.GetComponentFromSelection<DryadMotif>();

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
        Motif.NotesData.Clear();

        foreach(DryadMotifNote note in notes)
            Motif.NotesData.Add(new MotifNoteData(note.Id, note.ScoreTime, note.TonicOffset, note.Duration));

        Motif.Duration = Duration;
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
        scoreTime *= Dryad.Duration.Sixteenth;

        NormalizeScoreTime(ref scoreTime);

        return (scoreTime, tonicOffset);
    }

    public Vector2 ScoreTimeAndTonicOffsetToPosition(uint scoreTime, int tonicOffset)
    {
        float x = scoreTime / Dryad.Duration.Sixteenth * GridUnitSize;
        float y = tonicOffset * -1.0f * GridUnitSize;
        return new Vector2(x, y) + offset;
    }

    void NormalizeScoreTime(ref uint scoreTime)
    {
        // Fit to grid Dryad.Duration unit value
        if (scoreTime % Dryad.Duration.Sixteenth != 0)
            scoreTime -= scoreTime % Dryad.Duration.Sixteenth;
    }

    void UpdateMotifDurationAfterDrag()
    {
        MotifDurationRect.x -= MotifDurationRect.x % GridUnitSize;
        Duration = (uint) Mathf.FloorToInt(MotifDurationRect.x / GridUnitSize) * Dryad.Duration.Sixteenth;
        NormalizeScoreTime(ref Duration);
        GUI.changed = true;
        dataHasChanged = true;
    }

    #endregion

}
