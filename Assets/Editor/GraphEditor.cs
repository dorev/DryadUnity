using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;

public class GraphEditor : EditorWindow
{
    //https://gram.gs/gramlog/creating-node-based-editor-unity/

    string graphName = "";
    List<Node> nodes;

    private List<Connection> connections;

    private GUIStyle nodeStyle;
    private GUIStyle selectedNodeStyle;
    private GUIStyle inPointStyle;
    private GUIStyle outPointStyle;

    private ConnectionPoint selectedInPoint;
    private ConnectionPoint selectedOutPoint;

    private Vector2 drag;
    private Vector2 offset;

    private void OnEnable()
    {
        nodeStyle = new GUIStyle();
        nodeStyle.normal.background = EditorGUIUtility.Load("builtin skins/darkskin/images/node1.png") as Texture2D;
        nodeStyle.border = new RectOffset(12, 12, 12, 12);

        selectedNodeStyle = new GUIStyle();
        selectedNodeStyle.normal.background = EditorGUIUtility.Load("builtin skins/darkskin/images/node1 on.png") as Texture2D;
        selectedNodeStyle.border = new RectOffset(12, 12, 12, 12);

        inPointStyle = new GUIStyle();
        inPointStyle.normal.background = EditorGUIUtility.Load("builtin skins/darkskin/images/btn left.png") as Texture2D;
        inPointStyle.active.background = EditorGUIUtility.Load("builtin skins/darkskin/images/btn left on.png") as Texture2D;
        inPointStyle.border = new RectOffset(4, 4, 12, 12);

        outPointStyle = new GUIStyle();
        outPointStyle.normal.background = EditorGUIUtility.Load("builtin skins/darkskin/images/btn right.png") as Texture2D;
        outPointStyle.active.background = EditorGUIUtility.Load("builtin skins/darkskin/images/btn right on.png") as Texture2D;
        outPointStyle.border = new RectOffset(4, 4, 12, 12);
    }

    // target landscape
    DryadLandscape targetLandscape;

    [MenuItem("Dryad/Harmony Graph Creator")]
    public static void ShowWindow()
    {
        GraphEditor window = GetWindow<GraphEditor>();
        window.titleContent = new GUIContent("Harmony Graph Creator");
    }

    [InitializeOnLoadMethod]
    static void Init()
    {
        DryadLandscape.OnOpenGraphEditor = ShowWindow; // note: there is no () on this
    }

    private void OnGUI()
    {
        DrawGrid(20, 0.2f, Color.gray);
        DrawGrid(100, 0.4f, Color.gray);

        GUILayout.Label("Dryad Graph Creator", EditorStyles.boldLabel);
        graphName = EditorGUILayout.TextField("Name", graphName);
        targetLandscape = EditorGUILayout.ObjectField("Target landscape", targetLandscape, typeof(DryadLandscape), true) as DryadLandscape;
        EditorGUILayout.HelpBox("Target landscape is required!", MessageType.Warning, false);
        EditorGUILayout.Space();

        DrawNodes();
        DrawConnections();
        DrawConnectionLine(Event.current);

        ProcessNodeEvents(Event.current);
        ProcessEvents(Event.current);
        if (GUI.changed)
            Repaint();
    }

    void DrawNodes()
    {
        if (nodes == null)
            return;
        foreach(Node node in nodes)
            node.Draw();
    }

    void DrawConnections()
    {
        if (connections == null)
            return;
        foreach (Connection connection in connections)
            connection.Draw();
    }

    void ProcessEvents(Event e)
    {
        drag = Vector2.zero;

        switch (e.type)
        {
            case EventType.MouseDown:
                if (e.button == 0)
                    ClearConnectionSelection();
                else if (e.button == 1)
                    ProcessContextMenu(e.mousePosition);
                break;
            case EventType.MouseDrag:
                if (e.button == 0)
                    OnDrag(e.delta);
                break;
        }
    }
    private void ProcessNodeEvents(Event e)
    {
        if (nodes == null)
            return;

        foreach (Node node in nodes) // might need to do the iteration backward to redraw in order of addition
        {
            if (node.ProcessEvents(e))
                GUI.changed = true;
        }
    }

    private void ProcessContextMenu(Vector2 mousePosition)
    {
        GenericMenu genericMenu = new GenericMenu();
        genericMenu.AddItem(new GUIContent("Add node"), false, () => OnClickAddNode(mousePosition));
        genericMenu.ShowAsContext();
    }

    private void OnClickAddNode(Vector2 mousePosition)
    {
        if (nodes == null)
            nodes = new List<Node>();

        nodes.Add(new Node(mousePosition, 200, 50, nodeStyle, selectedNodeStyle, inPointStyle, outPointStyle, OnClickInPoint, OnClickOutPoint, OnClickRemoveNode));
    }

    private void OnClickInPoint(ConnectionPoint inPoint)
    {
        selectedInPoint = inPoint;

        if (selectedOutPoint != null)
        {
            if (selectedOutPoint.node != selectedInPoint.node)
            {
                CreateConnection();
                ClearConnectionSelection();
            }
            else
            {
                ClearConnectionSelection();
            }
        }
    }

    private void OnClickOutPoint(ConnectionPoint outPoint)
    {
        selectedOutPoint = outPoint;

        if (selectedInPoint != null)
        {
            if (selectedOutPoint.node != selectedInPoint.node)
            {
                CreateConnection();
                ClearConnectionSelection();
            }
            else
            {
                ClearConnectionSelection();
            }
        }
    }

    private void OnClickRemoveConnection(Connection connection)
    {
        connections.Remove(connection);
    }

    private void OnClickRemoveNode(Node node)
    {
        if (connections != null)
        {
            List<Connection> connectionsToRemove = new List<Connection>();
 
            foreach(Connection connection in connections)
            {
                if (connection.inPoint == node.inPoint || connection.outPoint == node.outPoint)
                    connectionsToRemove.Add(connection);
            }
 
            foreach(Connection connection in connectionsToRemove)
                connections.Remove(connection);
 
            connectionsToRemove = null;
        }
 
        nodes.Remove(node);
    }
    private void OnDrag(Vector2 delta)
    {
        drag = delta;

        if (nodes != null)
        {
            foreach(Node node in nodes)
                node.Drag(delta);
        }

        GUI.changed = true;
    }

    private void CreateConnection()
    {
        if (connections == null)
            connections = new List<Connection>();

        connections.Add(new Connection(selectedInPoint, selectedOutPoint, OnClickRemoveConnection));
    }

    private void ClearConnectionSelection()
    {
        selectedInPoint = null;
        selectedOutPoint = null;
    }

    private void DrawConnectionLine(Event e)
    {
        if (selectedInPoint != null && selectedOutPoint == null)
        {
            Handles.DrawBezier(
                selectedInPoint.rect.center,
                e.mousePosition,
                selectedInPoint.rect.center + Vector2.left * 25f,
                e.mousePosition - Vector2.left * 25f,
                Color.white,
                null,
                2f
            );
 
            GUI.changed = true;
        }
        else if (selectedOutPoint != null && selectedInPoint == null)
        {
            Handles.DrawBezier(
                selectedOutPoint.rect.center,
                e.mousePosition,
                selectedOutPoint.rect.center - Vector2.left * 25f,
                e.mousePosition + Vector2.left * 25f,
                Color.white,
                null,
                2f
            );
 
            GUI.changed = true;
        }
    }

    private void DrawGrid(float gridSpacing, float gridOpacity, Color gridColor)
    {
        int widthDivs = Mathf.CeilToInt(position.width / gridSpacing);
        int heightDivs = Mathf.CeilToInt(position.height / gridSpacing);
 
        Handles.BeginGUI();
        Handles.color = new Color(gridColor.r, gridColor.g, gridColor.b, gridOpacity);
 
        offset += drag * 0.5f;
        Vector3 newOffset = new Vector3(offset.x % gridSpacing, offset.y % gridSpacing, 0);
 
        for (int i = 0; i < widthDivs; i++)
            Handles.DrawLine(new Vector3(gridSpacing * i, -gridSpacing, 0) + newOffset, new Vector3(gridSpacing * i, position.height, 0f) + newOffset);
 
        for (int j = 0; j < heightDivs; j++)
            Handles.DrawLine(new Vector3(-gridSpacing, gridSpacing * j, 0) + newOffset, new Vector3(position.width, gridSpacing * j, 0f) + newOffset);
 
        Handles.color = Color.white;
        Handles.EndGUI();
    }
}
