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

    static protected GameObjectType GetComponentFromSelection<GameObjectType>()
    {
        if (Selection.activeTransform == null)
            return default(GameObjectType);

        GameObject gameObject = Selection.activeTransform.gameObject;

        if (gameObject == null)
            return default(GameObjectType);

        return gameObject.GetComponent<GameObjectType>();
    }

    static protected Rect DefaultLabelRect(float x, float y)
    {
        return new Rect(x, y, EditorGUIUtility.labelWidth, EditorGUIUtility.singleLineHeight);
    }
}
