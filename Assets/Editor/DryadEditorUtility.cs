using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;

static public class DryadEditorUtility
{
    static public GameObjectType GetComponentFromSelection<GameObjectType>()
    {
        if (Selection.activeTransform == null)
            return default(GameObjectType);

        GameObject gameObject = Selection.activeTransform.gameObject;

        if (gameObject == null)
            return default(GameObjectType);

        return gameObject.GetComponent<GameObjectType>();
    }

    static public Rect DebugTextRect(float x, float y, uint lines = 1)
    {
        return new Rect(x, y, 500, (EditorGUIUtility.singleLineHeight + EditorGUIUtility.standardVerticalSpacing) * lines);
    }

    static public Texture2D ColorTexture(Color color, float opacity = 1)
    {
        Texture2D result = new Texture2D(1, 1);
        result.SetPixel(0, 0, new Color(color.r, color.g, color.b, opacity));
        result.Apply();
        return result;
    }


}
