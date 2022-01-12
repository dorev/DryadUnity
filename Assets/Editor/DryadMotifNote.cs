using System;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;

public class DryadMotifNote : DryadEditorObjectBase
{

    #region Members

    private static uint staticIdSource = 0;

    public GUIStyle Style;
    static GUIStyle StretcherStyle;
    static GUIStyle defaultStyle;
    static GUIStyle selectedStyle;

    public bool isStretched;
    public enum StretchDirection
    {
        None,
        Front,
        Back
    };

    private StretchDirection StretchedDirection = StretchDirection.None;

    private Rect FrontStretcherRect;
    private Rect BackStretcherRect;

    Action<DryadMotifNote> OnRemoveNote;
    Action<DryadMotifNote, StretchDirection, Vector2> OnStretchNote;
    public int TonicOffset;
    public uint ScoreTime;
    public uint Duration;

    private DryadMotifEditor motifEditor;

    #endregion

    #region Initialization

    public DryadMotifNote(
        DryadMotifEditor editor,
        uint duration,
        uint scoreTime,
        int tonicOffset,
        Action<DryadMotifNote> OnClickRemoveNote,
        Action<DryadMotifNote, StretchDirection, Vector2> OnDragStretchNote)
        : base(staticIdSource++)
    {
        Duration = duration;
        ScoreTime = scoreTime;
        TonicOffset = tonicOffset;
        Init(editor, OnClickRemoveNote, OnDragStretchNote);
    }

    public DryadMotifNote(
        MotifNoteData noteData,
        DryadMotifEditor editor,
        Action<DryadMotifNote> OnClickRemoveNote,
        Action<DryadMotifNote, StretchDirection, Vector2> OnDragStretchNote)
        : base(staticIdSource++)
    {
        Duration = noteData.Duration;
        ScoreTime = noteData.ScoreTime;
        TonicOffset = noteData.TonicOffset;
        Init(editor, OnClickRemoveNote, OnDragStretchNote);
    }

    void Init(
        DryadMotifEditor editor,
        Action<DryadMotifNote> OnClickRemoveNote,
        Action<DryadMotifNote, StretchDirection, Vector2> OnDragStretchNote)
    {
        motifEditor = editor;
        SetupStyles();
        OnRemoveNote = OnClickRemoveNote;
        OnStretchNote = OnDragStretchNote;
        UpdatePositionInEditorFromNoteData();
        FrontStretcherRect.width = EditorGridUnitSize() * 0.5f;
        FrontStretcherRect.height = EditorGridUnitSize();
        BackStretcherRect.width = EditorGridUnitSize() * 0.5f;
        BackStretcherRect.height = EditorGridUnitSize();
    }

    private void SetupStyles()
    {
        defaultStyle = new GUIStyle();
        defaultStyle.normal.background = DryadEditorUtility.ColorTexture(Color.green, 0.5f);

        selectedStyle = new GUIStyle();
        selectedStyle.normal.background = DryadEditorUtility.ColorTexture(Color.green, 0.2f);

        Style = defaultStyle;
        StretcherStyle = defaultStyle;
    }

    #endregion

    #region Drawing

    public void Draw()
    {
        if(!(isDragged || isStretched))
            UpdatePositionInEditorFromNoteData();

        GUI.Box(Rect, "", Style);

        if(!isDragged && isSelected)
        {
            FrontStretcherRect.position = Rect.position;
            BackStretcherRect.y = Rect.y;
            BackStretcherRect.x = Rect.x + Rect.width - EditorGridUnitSize() * 0.5f;

            GUI.Box(FrontStretcherRect, "", StretcherStyle);
            GUI.Box(BackStretcherRect, "", StretcherStyle);
        }

        DebugLabel($"Time: {ScoreTime}  Offset: {TonicOffset} Duration: {Duration}");
    }

    void ProcessContextMenu()
    {
        GenericMenu genericMenu = new GenericMenu();
        genericMenu.AddItem(new GUIContent("Delete"), false, OnClickRemoveNote);
        genericMenu.ShowAsContext();
    }

    #endregion

    #region Events

    public bool ProcessEvents(Event e)
    {
        switch (e.type)
        {
            case EventType.MouseDown:
                if (e.button == 0)
                {
                    if (FrontStretcherRect.Contains(e.mousePosition))
                    {
                        StretchedDirection = StretchDirection.Front;
                        isStretched = true;
                    }
                    else if (BackStretcherRect.Contains(e.mousePosition))
                    {
                        StretchedDirection = StretchDirection.Back;
                        isStretched = true;
                    }
                    else if (Rect.Contains(e.mousePosition))
                    {
                        isDragged = true;
                        isSelected = true;
                        GUI.changed = true;
                        Style = selectedStyle;
                    }
                    else
                    {
                        GUI.changed = true;
                        isSelected = false;
                    }
                }
                if (e.button == 1 && Rect.Contains(e.mousePosition))
                {
                    ProcessContextMenu();
                    e.Use();
                }
                break;

            case EventType.MouseUp:

                if (isDragged)
                {
                    UpdateNoteDataFromRect();
                    GUI.changed = true;
                }

                if(isStretched)
                {
                    UpdateNoteDataFromRect();
                    StretchedDirection = StretchDirection.None;
                    GUI.changed = true;
                }

                isDragged = false;
                isStretched = false;
                Style = defaultStyle;
                break;

            case EventType.MouseDrag:
                if (e.button == 0)
                {
                    if(isDragged)
                    {
                        Drag(e.delta);
                        e.Use();
                        return true;
                    }
                    else if(isStretched)
                    {
                        OnStretch(e.delta);
                        e.Use();
                        return true;
                    }
                }
                break;
        }

        return false;
    }

    void OnStretch(Vector2 delta)
    {
        OnStretchNote?.Invoke(this, StretchedDirection, delta);
    }

    void OnClickRemoveNote()
    {
        OnRemoveNote?.Invoke(this);
    }

    float EditorGridUnitSize()
    {
        return motifEditor.GridUnitSize;
    }

    void UpdatePositionInEditorFromNoteData()
    {
        Rect.position = motifEditor.ScoreTimeAndTonicOffsetToPosition(ScoreTime, TonicOffset);
        Rect.width = Duration / DryadUnity.Duration.Sixteenth * EditorGridUnitSize();
        Rect.height = EditorGridUnitSize();
    }

    void UpdateNoteDataFromRect()
    {
        (ScoreTime, TonicOffset) = motifEditor.PositionToScoreTimeAndTonicOffset(Rect.position);
        Duration = (uint) Mathf.RoundToInt(Rect.width / EditorGridUnitSize()) * DryadUnity.Duration.Sixteenth;
    }

    #endregion

}
