using System;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;

public class DryadMotifNote : DryadEditorObjectBase
{
    private static uint staticIdSource = 0;

    public GUIStyle Style;
    static GUIStyle defaultNodeStyle;
    static GUIStyle selectedNodeStyle;

    Action<DryadMotifNote> OnNoteClicked;
    Action<DryadMotifNote> OnRemoveNote;

    static DryadMotifNote()
    {
        defaultNodeStyle = new GUIStyle();
        defaultNodeStyle.normal.background = MakeTextureColor(Color.green);
        defaultNodeStyle.border = new RectOffset(12, 12, 12, 12);
        defaultNodeStyle.padding = new RectOffset(12, 0, 4, 0);

        selectedNodeStyle = new GUIStyle();
        selectedNodeStyle.normal.background = MakeTextureColor(Color.blue); ;
        selectedNodeStyle.border = new RectOffset(12, 12, 12, 12);
        selectedNodeStyle.padding = new RectOffset(12, 0, 4, 0);
    }

    public DryadMotifNote()
        : base(staticIdSource++)
    {

    }

    public void Draw()
    {
        
    }

    public void SnapOnGrid(float offset, float gridSize)
    {

    }

    public bool ProcessEvents(Event e)
    {
        switch (e.type)
        {
            case EventType.MouseDown:
                if (e.button == 0)
                {
                    if (Rect.Contains(e.mousePosition))
                    {
                        isDragged = true;
                        GUI.changed = true;
                        isSelected = true;
                        Style = selectedNodeStyle;
                        OnNoteClicked?.Invoke(this);
                    }
                    else
                    {
                        GUI.changed = true;
                        isSelected = false;
                        Style = defaultNodeStyle;
                    }
                }
                if (e.button == 1 && Rect.Contains(e.mousePosition))
                {
                    ProcessContextMenu();
                    e.Use();
                }
                break;

            case EventType.MouseUp:
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

    void ProcessContextMenu()
    {

        GenericMenu genericMenu = new GenericMenu();
        genericMenu.AddItem(new GUIContent("Delete"), false, OnClickRemoveNote);
        genericMenu.ShowAsContext();
    }

    void OnClickRemoveNote()
    {
        OnRemoveNote?.Invoke(this);
    }

}
