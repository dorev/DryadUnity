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

    // Only constructible through inheritance
    protected DryadEditorObjectBase() { }

    protected DryadEditorObjectBase(uint id)
    {
        Id = id;
    }

    public void Drag(Vector2 delta)
    {
        Rect.position += delta;
    }

    static protected Texture2D MakeTextureColor(Color color)
    {
        Texture2D result = new Texture2D(1, 1);
        result.SetPixel(0,0,color);
        result.Apply();
        return result;
    }
}
