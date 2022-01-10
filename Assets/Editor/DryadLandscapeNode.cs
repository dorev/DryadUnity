using System;
using System.Collections.Generic;
using UnityEditor;
using UnityEngine;

public class DryadLandscapeNode : DryadEditorObjectBase
{
    #region Members

    public List<uint> Edges = new List<uint>();
    public Dryad.Chord Chord;
    public GUIStyle Style;
    Action<DryadLandscapeNode> OnRemoveNode;
    Action<DryadLandscapeNode> OnCreateLink;
    Action<DryadLandscapeNode> OnNodeClicked;

    static GUIStyle defaultNodeStyle;
    static GUIStyle selectedNodeStyle;
    static GUIStyle titleStyle;
    static readonly float width = 230;
    static readonly float height = 145;
    static readonly float labelWidthValue = 70;
    static readonly float valueWidthValue = 110;
    static readonly GUILayoutOption labelWidth = GUILayout.Width(labelWidthValue);
    static readonly GUILayoutOption valueWidth = GUILayout.Width(valueWidthValue);
    static uint staticIdSource = 0;

    #endregion

    #region Initialization

    static DryadLandscapeNode()
    {
        defaultNodeStyle = new GUIStyle();
        defaultNodeStyle.normal.background = EditorGUIUtility.Load("builtin skins/darkskin/images/node1.png") as Texture2D;
        defaultNodeStyle.border = new RectOffset(12, 12, 12, 12);
        defaultNodeStyle.padding = new RectOffset(12, 0, 4, 0);
        defaultNodeStyle.alignment = TextAnchor.MiddleCenter;

        selectedNodeStyle = new GUIStyle();
        selectedNodeStyle.normal.background = EditorGUIUtility.Load("builtin skins/darkskin/images/node1 on.png") as Texture2D;
        selectedNodeStyle.border = new RectOffset(12, 12, 12, 12);
        selectedNodeStyle.padding = new RectOffset(12, 0, 4, 0);
        selectedNodeStyle.alignment = TextAnchor.MiddleCenter;

        titleStyle = new GUIStyle();
        titleStyle.normal.textColor = Color.white;
        titleStyle.alignment = TextAnchor.MiddleCenter;
        titleStyle.fontStyle = FontStyle.Bold;
        titleStyle.fontSize = 16;
    }

    public DryadLandscapeNode(
        Dryad.Chord chord,
        Vector2 position,
        Action<DryadLandscapeNode> OnClickCreateLink,
        Action<DryadLandscapeNode> OnClickInNode,
        Action<DryadLandscapeNode> OnClickRemoveNode)
        : base(staticIdSource++)
    {
        Chord = chord;
        Rect = new Rect(position.x, position.y, width, height);

        Style = defaultNodeStyle;
        OnRemoveNode = OnClickRemoveNode;
        OnCreateLink = OnClickCreateLink;
        OnNodeClicked = OnClickInNode;
    }

    public DryadLandscapeNode(
        LandscapeNodeData nodeData,
        Action<DryadLandscapeNode> OnClickCreateLink,
        Action<DryadLandscapeNode> OnClickInNode,
        Action<DryadLandscapeNode> OnClickRemoveNode)
        : base(nodeData.Id)
    {
        Chord = nodeData.Chord;
        Rect = nodeData.Rect;
        Edges = nodeData.Edges;

        Style = defaultNodeStyle;
        OnRemoveNode = OnClickRemoveNode;
        OnCreateLink = OnClickCreateLink;
        OnNodeClicked = OnClickInNode;
    }

    #endregion

    #region Drawing

    public void Draw()
    {
        GUILayout.BeginArea(Rect, Style);

        GUILayout.Space(5);
        GUILayout.Label(Chord.Name, titleStyle, GUILayout.Width(labelWidthValue + valueWidthValue));
        GUILayout.Space(5);

        GUILayout.BeginHorizontal();
        GUILayout.Label("Voicing", labelWidth);
        Chord.TriadVoicing = (Dryad.TriadVoicing)EditorGUILayout.EnumPopup(Chord.TriadVoicing, valueWidth);
        GUILayout.EndHorizontal();

        GUILayout.BeginHorizontal();
        GUILayout.Label("Extension", labelWidth);
        Chord.Extension = (Dryad.ChordExtension)EditorGUILayout.EnumPopup(Chord.Extension, valueWidth);
        GUILayout.EndHorizontal();

        GUILayout.BeginHorizontal();
        GUILayout.Label("Inversion", labelWidth);
        Chord.Inversion = EditorGUILayout.IntSlider(Chord.Inversion, 0, 3, valueWidth);
        GUILayout.EndHorizontal();

        GUILayout.BeginHorizontal();
        GUILayout.Label("Shift", labelWidth);
        Chord.Shift = EditorGUILayout.IntSlider(Chord.Shift, -2, 2, valueWidth);
        GUILayout.EndHorizontal();

        GUILayout.BeginHorizontal();
        GUILayout.Label("Entry", labelWidth);
        Chord.Entry = EditorGUILayout.Toggle(Chord.Entry, valueWidth);
        GUILayout.EndHorizontal();

        GUILayout.EndArea();
    }

    private void ProcessContextMenu()
    {
        GenericMenu genericMenu = new GenericMenu();
        genericMenu.AddItem(new GUIContent("Remove node"), false, OnClickRemoveNode);
        genericMenu.AddItem(new GUIContent("Create directional link"), false, OnClickCreateLink);
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
                    if (Rect.Contains(e.mousePosition))
                    {
                        isDragged = true;
                        GUI.changed = true;
                        isSelected = true;
                        Style = selectedNodeStyle;
                        OnNodeClicked?.Invoke(this);
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

    private void OnClickRemoveNode()
    {
        OnRemoveNode?.Invoke(this);
    }

    private void OnClickCreateLink()
    {
        OnCreateLink?.Invoke(this);
    }

    #endregion

}
