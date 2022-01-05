using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;

public class DryadEditorObjectBase
{
    public Rect Rect;
    public bool isDragged;
    public bool isSelected;
    public readonly uint Id;
    public Vector2 debugDrag;
    public Vector2 debugOffset = Vector2.zero;

    // Only constructible through inheritance
    protected DryadEditorObjectBase() { }

    protected DryadEditorObjectBase(uint id)
    {
        Id = id;
    }

    public virtual void Drag(Vector2 delta)
    {
        debugDrag = delta;
        debugOffset += delta;
        Rect.position += delta;
    }

    protected void DebugLabel(string debugText)
    {
        Color defaultColor = GUI.color;
        GUI.color = Color.red;
        GUI.Label(new Rect(Rect.position, new Vector2(500, EditorGUIUtility.singleLineHeight)), debugText);
        GUI.color = defaultColor;
    }
}
