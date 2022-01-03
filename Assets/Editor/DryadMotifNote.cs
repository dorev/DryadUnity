using System;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;

// Snap breaks when zooming in... lock zoom???
// Add notedata in rectangles for debug
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
    static GUIStyle defaultNoteStyle;
    static GUIStyle selectedNoteStyle;

    Action<DryadMotifNote> OnNoteClicked;
    Action<DryadMotifNote> OnRemoveNote;
    public int TonicOffset;
    public uint ScoreTime;
    public uint NoteDuration;

    private DryadMotifEditor motifEditor;


    #endregion

    #region Initialization

    private void SetupStyles()
    {
        
        defaultNoteStyle = new GUIStyle();
        defaultNoteStyle.normal.background = MakeTextureColor(Color.green);
        defaultNoteStyle.border = new RectOffset(12, 12, 12, 12);
        defaultNoteStyle.padding = new RectOffset(12, 0, 4, 0);

        selectedNoteStyle = new GUIStyle();
        selectedNoteStyle.normal.background = MakeTextureColor(Color.blue); ;
        selectedNoteStyle.border = new RectOffset(12, 12, 12, 12);
        selectedNoteStyle.padding = new RectOffset(12, 0, 4, 0);
        
        Style = defaultNoteStyle;
    }

    public DryadMotifNote(DryadMotifEditor editor, Vector2 position, uint duration, uint scoreTime, int tonicOffset)
        : base(staticIdSource++)
    {
        SetupStyles();
        motifEditor = editor;
        NoteDuration = duration;
        ScoreTime = scoreTime;
        TonicOffset = tonicOffset;
        PositionRect = new Rect(position, new Vector2( duration / Sixteenth * EditorGridUnitSize(), EditorGridUnitSize()));
    }

    #endregion

    #region Drawing

    public void Draw()
    {
        if(!isDragged)
        {
            PositionRect.width = NoteDuration / Sixteenth * EditorGridUnitSize();
            PositionRect.height = EditorGridUnitSize();
            UpdatePositionInEditorFromNoteData();
        }

        GUILayout.BeginArea(PositionRect, Style);
        GUILayout.EndArea();
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
                    if (PositionRect.Contains(e.mousePosition))
                    {
                        isDragged = true;
                        GUI.changed = true;
                        isSelected = true;
                        Style = selectedNoteStyle;
                        OnNoteClicked?.Invoke(this);
                    }
                    else
                    {
                        GUI.changed = true;
                        isSelected = false;
                    }
                }
                if (e.button == 1 && PositionRect.Contains(e.mousePosition))
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

                Style = defaultNoteStyle;
                isDragged = false;
                break;

            case EventType.MouseDrag:
                if (e.button == 0 && isDragged)
                {
                    Drag(e.delta);
                    e.Use();
                    return true;
                }
                break;
        }

        return false;
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
        PositionRect.position = motifEditor.ScoreTimeAndTonicOffsetToPosition(ScoreTime, TonicOffset);
    }

    void UpdateNoteDataFromPosition()
    {
        (ScoreTime, TonicOffset) = motifEditor.PositionToScoreTimeAndTonicOffset(PositionRect.position);
    }

    #endregion

}
