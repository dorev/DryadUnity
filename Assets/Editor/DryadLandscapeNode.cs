using System;
using UnityEditor;
using UnityEngine;

public class DryadLandscapeNode
{
    public Rect rect;
    Rect controlsRect;
    public string title = "Node title";
    public bool isDragged;
    public bool isSelected;
    public readonly uint nodeId;

    public DryadLandscapeConnectionPoint inPoint;
    public DryadLandscapeConnectionPoint outPoint;

    GUIStyle style;
    GUIStyle defaultNodeStyle;
    GUIStyle selectedNodeStyle;
    GUIStyle titleStyle;

    Action<DryadLandscapeNode> OnRemoveNode;

    public static readonly float width = 230;
    public static readonly float height = 145;
    static readonly float spacing = EditorGUIUtility.standardVerticalSpacing;
    static readonly float lineHeight = EditorGUIUtility.singleLineHeight;
    static readonly float labelWidthValue = 70;
    static readonly float valueWidthValue = 110;
    static readonly GUILayoutOption labelWidth = GUILayout.Width(labelWidthValue);
    static readonly GUILayoutOption valueWidth = GUILayout.Width(valueWidthValue);
    static uint ID = 0;

    Dryad.TriadVoicing Voicing;
    Dryad.Extension Extension;
    int Inversion = 0;
    int Shift = 0;

    public DryadLandscapeNode(
        Vector2 position,
        Action<DryadLandscapeConnectionPoint> OnClickInPoint,
        Action<DryadLandscapeConnectionPoint> OnClickOutPoint,
        Action<DryadLandscapeNode> OnClickRemoveNode)
    {
        nodeId = ID++;
        rect = new Rect(position.x, position.y, width, height);
        controlsRect = new Rect(rect.x + 20, rect.y, rect.width - 20, rect.height - 20);

        defaultNodeStyle = style;
        OnRemoveNode = OnClickRemoveNode;

        style = new GUIStyle();
        style.normal.background = EditorGUIUtility.Load("builtin skins/darkskin/images/node1.png") as Texture2D;
        style.border = new RectOffset(12, 12, 12, 12);
        style.padding = new RectOffset(12, 0, 4, 0);

        //selectedNodeStyle = new GUIStyle();
        //selectedNodeStyle.normal.background = EditorGUIUtility.Load("builtin skins/darkskin/images/node1 on.png") as Texture2D;
        //selectedNodeStyle.border = new RectOffset(12, 12, 12, 12);

        titleStyle = new GUIStyle();
        titleStyle.normal.textColor = Color.white;
        titleStyle.alignment = TextAnchor.MiddleCenter;
        titleStyle.fontStyle = FontStyle.Bold;
        titleStyle.fontSize = 16;
    }

    public void Drag(Vector2 delta)
    {
        rect.position += delta;
        controlsRect.position += delta;
    }
    public static GUIStyle GetStyle(Color color)
    {
        GUIStyle style = new GUIStyle();
        Texture2D texture = new Texture2D(1, 1);
        texture.SetPixel(0, 0, color);
        texture.Apply();
        style.normal.background = texture;
        return style;
    }

    public void Draw()
    {
        GUILayout.BeginArea(controlsRect, style);
        GUILayout.Space(5);
        GUILayout.Label(title, titleStyle, GUILayout.Width(labelWidthValue + valueWidthValue + 20));
        GUILayout.Space(5);
        AddEnum("Voicing", ref Voicing);
        AddEnum("Extension", ref Extension);
        AddSlider("Inversion", ref Inversion, 0, 3);
        AddSlider("Shift", ref Shift, -2, 2);
        GUILayout.EndArea();
    }

    void AddEnum<Enum>(string label, ref Enum enumObject) where Enum : System.Enum
    {
        GUILayout.BeginHorizontal();
        GUILayout.Label(label, labelWidth);
        enumObject = (Enum)EditorGUILayout.EnumPopup(enumObject, valueWidth);
        GUILayout.EndHorizontal();
    }

    void AddSlider(string label, ref int value, int min, int max)
    {
        GUILayout.BeginHorizontal();
        GUILayout.Label(label, labelWidth);
        value = EditorGUILayout.IntSlider(value, min, max, valueWidth);
        GUILayout.EndHorizontal();
    }

    public bool ProcessEvents(Event e)
    {
        switch (e.type)
        {
            case EventType.MouseDown:
                if (e.button == 0)
                {
                    if (rect.Contains(e.mousePosition))
                    {
                        isDragged = true;
                        GUI.changed = true;
                        isSelected = true;
                        //style = selectedNodeStyle;
                    }
                    else
                    {
                        GUI.changed = true;
                        isSelected = false;
                        //style = defaultNodeStyle;
                    }
                }
                if (e.button == 1 /* && isSelected */ && rect.Contains(e.mousePosition))
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

    private void ProcessContextMenu()
    {
        GenericMenu genericMenu = new GenericMenu();
        genericMenu.AddItem(new GUIContent("Remove node"), false, OnClickRemoveNode);
        genericMenu.ShowAsContext();
    }
 
    private void OnClickRemoveNode()
    {
        if (OnRemoveNode != null)
            OnRemoveNode(this);
    }
}
