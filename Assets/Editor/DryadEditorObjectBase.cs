using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;

public class DryadEditorObjectBase
{
    public Rect PositionRect;
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
        PositionRect.position += delta;
    }

    static protected Texture2D MakeTextureColor(Color color)
    {
        Texture2D result = new Texture2D(1, 1);
        result.SetPixel(0,0,color);
        result.Apply();
        return result;
    }
}
