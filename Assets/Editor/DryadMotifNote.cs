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
    private enum StretchDirection
    {
        None,
        Front,
        Back
    };

    private StretchDirection StretchedDirection = StretchDirection.None;

    private Rect FrontStretcherRect;
    private Rect BackStretcherRect;

    Action<DryadMotifNote> OnFrontClicked;
    Action<DryadMotifNote> OnBackClicked;
    Action<DryadMotifNote> OnRemoveNote;
    public int TonicOffset;
    public uint ScoreTime;
    public uint NoteDuration;

    private DryadMotifEditor motifEditor;

    #endregion

    #region Initialization

    public DryadMotifNote(DryadMotifEditor editor, uint duration, uint scoreTime, int tonicOffset, Action<DryadMotifNote> OnClickRemoveNote)
        : base(staticIdSource++)
    {
        SetupStyles();
        motifEditor = editor;
        NoteDuration = duration;
        ScoreTime = scoreTime;
        TonicOffset = tonicOffset;
        OnRemoveNote = OnClickRemoveNote;
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

        DebugLabel($"Time: {ScoreTime}  Offset: {TonicOffset} Duration: {NoteDuration}");
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
                        Stretch(e.delta);
                        e.Use();
                        return true;
                    }
                }
                break;
        }

        return false;
    }

    void Stretch(Vector2 delta)
    {
        switch(StretchedDirection)
        {
            case StretchDirection.Front:
                Rect.x += delta.x;
                Rect.width -= delta.x;
                break;

            case StretchDirection.Back:
                Rect.width += delta.x;
                break;

            case StretchDirection.None:
                break;
        }
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
        Rect.width = NoteDuration / Dryad.Duration.Sixteenth * EditorGridUnitSize();
        Rect.height = EditorGridUnitSize();
    }

    void UpdateNoteDataFromRect()
    {
        (ScoreTime, TonicOffset) = motifEditor.PositionToScoreTimeAndTonicOffset(Rect.position);
        NoteDuration = (uint) Mathf.RoundToInt(Rect.width / EditorGridUnitSize()) * Dryad.Duration.Sixteenth;
    }

    #endregion

}
