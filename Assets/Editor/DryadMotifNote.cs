using System;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;

// Add contour to notes
// Capability to stretch notes

public class DryadMotifNote : DryadEditorObjectBase
{

    #region Members

    // DryadCore note duration sizes
    static public readonly uint WholeDotted = 144;
    static public readonly uint Whole = 96;
    static public readonly uint HalfDotted = 72;
    static public readonly uint Half = 48;
    static public readonly uint QuarterDotted = 36;
    static public readonly uint HalfTriplet = 32;
    static public readonly uint Quarter = 24;
    static public readonly uint EighthDotted = 18;
    static public readonly uint QuarterTriplet = 16;
    static public readonly uint Eighth = 12;
    static public readonly uint EighthTriplet = 8;
    static public readonly uint Sixteenth = 6;
    static public readonly uint SixteenthTriplet = 4;
    static public readonly uint ThirtySecond = 3;
    static public readonly uint ThirtySecondTriplet = 2;

    private static uint staticIdSource = 0;

    public GUIStyle Style;
    static GUIStyle StretcherStyle;
    static GUIStyle defaultStyle;
    static GUIStyle selectedStyle;

    public bool isStretched;

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
        defaultStyle.normal.background = MakeTextureColor(Color.green, 0.5f);

        selectedStyle = new GUIStyle();
        selectedStyle.normal.background = MakeTextureColor(Color.green, 0.2f);

        Style = defaultStyle;
        StretcherStyle = defaultStyle;
    }

    #endregion

    #region Drawing

    public void Draw()
    {
        if(!isDragged)
        {
            Rect.width = NoteDuration / Sixteenth * EditorGridUnitSize();
            Rect.height = EditorGridUnitSize();
            UpdatePositionInEditorFromNoteData();
        }

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
                    if (isSelected 
                    && (FrontStretcherRect.Contains(e.mousePosition) || BackStretcherRect.Contains(e.mousePosition)))
                    {
                        isStretched = true;
                        GUI.changed = true;
                        StretcherStyle = selectedStyle;
                    }
                    else if (Rect.Contains(e.mousePosition))
                    {
                        isDragged = true;
                        GUI.changed = true;
                        isSelected = true;
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
                    UpdateNoteDataFromPosition();
                    GUI.changed = true;
                }

                Style = defaultStyle;
                StretcherStyle = defaultStyle;
                isDragged = false;
                isStretched = false;
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
        Rect.width += delta.x;
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
    }

    void UpdateNoteDataFromPosition()
    {
        (ScoreTime, TonicOffset) = motifEditor.PositionToScoreTimeAndTonicOffset(Rect.position);
    }

    #endregion

}
