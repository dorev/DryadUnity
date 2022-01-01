using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;

public class DryadEditorBase : EditorWindow
{
    // Only construcable through inheritance
    protected DryadEditorBase() { }

    protected Vector2 drag;
    protected Vector2 offset;
    protected bool dataHasChanged = false;

    protected GameObjectType GetGameObjectFromSelection<GameObjectType>()
    {
        if (Selection.activeTransform == null)
            return default(GameObjectType);

        GameObject obj = Selection.activeTransform.gameObject;

        if (obj == null)
            return default(GameObjectType);

        return obj.GetComponent<GameObjectType>();
    }

    protected Rect DefaultLabelRect(float x, float y)
    {
        return new Rect(x, y, EditorGUIUtility.labelWidth, EditorGUIUtility.singleLineHeight);
    }
}
