using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;


class NodeSlice
{
    uint height;
    GUIStyle style;
    DryadLandscapeNode parent; // for width
}

public class DryadLandscapeEditor : EditorWindow
{
    string graphName = "";
    List<DryadLandscapeNode> nodes;

    private DryadLandscape dryadLandscape;
    private List<DryadLandscapeConnectionLine> connections;

    private GUIStyle nodeStyle;
    private List<NodeSlice> nodeSlices;
    private GUIStyle selectedNodeStyle;
    private GUIStyle inPointStyle;
    private GUIStyle outPointStyle;

    private DryadLandscapeConnectionPoint selectedInPoint;
    private DryadLandscapeConnectionPoint selectedOutPoint;

    private Vector2 drag;
    private Vector2 offset;

    private void OnEnable()
    {

    }

    public static void ShowWindow(DryadLandscape landscape)
    {
        DryadLandscapeEditor window = GetWindow<DryadLandscapeEditor>();
        window.SetLandscape(landscape);
        window.titleContent = new GUIContent("Landscape Graph Editor");
    }

    private void SetLandscape(DryadLandscape landscape)
    {
        dryadLandscape = landscape;
        // get scale degrees
        // setup all nodes and connections
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
        GUILayout.Label(dryadLandscape.Name, EditorStyles.boldLabel);
        
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
        foreach(DryadLandscapeNode node in nodes)
            node.Draw();
    }

    void DrawConnections()
    {
        if (connections == null)
            return;
        foreach (DryadLandscapeConnectionLine connection in connections)
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

        foreach (DryadLandscapeNode node in nodes) // might need to do the iteration backward to redraw in order of addition
        {
            if (node.ProcessEvents(e))
                GUI.changed = true;
        }
    }

    private void ProcessContextMenu(Vector2 mousePosition)
    {
        GenericMenu genericMenu = new GenericMenu();
        genericMenu.AddItem(new GUIContent("Add root"), false, () => OnClickAddNode(mousePosition));
        genericMenu.AddItem(new GUIContent("Add second"), false, () => OnClickAddNode(mousePosition));
        genericMenu.AddItem(new GUIContent("Add third"), false, () => OnClickAddNode(mousePosition));
        genericMenu.AddItem(new GUIContent("Add fourth"), false, () => OnClickAddNode(mousePosition));
        genericMenu.AddItem(new GUIContent("Add fifth"), false, () => OnClickAddNode(mousePosition));
        genericMenu.AddItem(new GUIContent("Add sixth"), false, () => OnClickAddNode(mousePosition));
        genericMenu.AddItem(new GUIContent("Add seventh"), false, () => OnClickAddNode(mousePosition));
        genericMenu.ShowAsContext();
    }

    private void OnClickAddNode(Vector2 mousePosition)
    {
        if (nodes == null)
            nodes = new List<DryadLandscapeNode>();

        nodes.Add(new DryadLandscapeNode(mousePosition, OnClickInPoint, OnClickOutPoint, OnClickRemoveNode));
    }

    private void OnClickInPoint(DryadLandscapeConnectionPoint inPoint)
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

    private void OnClickOutPoint(DryadLandscapeConnectionPoint outPoint)
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

    private void OnClickRemoveConnection(DryadLandscapeConnectionLine connection)
    {
        connections.Remove(connection);
    }

    private void OnClickRemoveNode(DryadLandscapeNode node)
    {
        if (connections != null)
        {
            List<DryadLandscapeConnectionLine> connectionsToRemove = new List<DryadLandscapeConnectionLine>();
 
            foreach(DryadLandscapeConnectionLine connection in connections)
            {
                if (connection.inPoint == node.inPoint || connection.outPoint == node.outPoint)
                    connectionsToRemove.Add(connection);
            }
 
            foreach(DryadLandscapeConnectionLine connection in connectionsToRemove)
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
            foreach(DryadLandscapeNode node in nodes)
                node.Drag(delta);
        }

        GUI.changed = true;
    }

    private void CreateConnection()
    {
        if (connections == null)
            connections = new List<DryadLandscapeConnectionLine>();

        connections.Add(new DryadLandscapeConnectionLine(selectedInPoint, selectedOutPoint, OnClickRemoveConnection));
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
