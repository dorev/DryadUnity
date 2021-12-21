using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;


public class DryadLandscapeEditor : EditorWindow
{
    [SerializeField]
    public DryadLandscape Landscape;

    private List<DryadLandscapeNode> nodes = new List<DryadLandscapeNode>();
    private List<DryadLandscapeEdge> edges = new List<DryadLandscapeEdge>();
    private bool dataHasChanged = false;

    private DryadLandscapeNode selectedEdgeSourceNode;
    private Vector2 drag;
    private Vector2 offset;

    public static void ShowWindow(DryadLandscape landscape)
    {
        DryadLandscapeEditor window = GetWindow<DryadLandscapeEditor>();
        window.InitLandscapeGraph(landscape);
        window.titleContent = new GUIContent("Landscape Graph Editor");
    }

    private void InitLandscapeGraph(DryadLandscape landscape)
    {
        Landscape = landscape;

        // Add all nodes
        foreach(LandscapeNodeData nodeData in Landscape.NodesData)
            OnClickAddNode(nodeData);

        // Add all edges
        DryadLandscapeNode destinationNode;
        foreach (DryadLandscapeNode node in nodes.ToArray())
        {
            if(node.Edges.Count > 0)
            {
                foreach (uint edgeId in node.Edges.ToArray())
                {
                    if (FindNodeById(edgeId, out destinationNode))
                        CreateEdge(node, destinationNode);
                }
            }
        }
    }

    [InitializeOnLoadMethod]
    static void Init()
    {
        DryadLandscape.OnOpenGraphEditor = ShowWindow; // note: there is no () on this
    }   

    private void OnGUI()
    {
        if (dataHasChanged)
            SaveLandscapeData();

        DrawGrid(20, 0.2f, Color.gray);
        DrawGrid(100, 0.4f, Color.gray);

        if (Landscape == null)
        {
            EditorGUILayout.HelpBox(
                "No DryadLandscape selected\n" +
                "Select a DryadLanscape from the inspector\n" +
                "and click Open Landscape Graph Editor.",
                MessageType.Warning);
            return;
        }

        GUILayout.Label(Landscape.Name, EditorStyles.boldLabel);

        // Links are under the nodes
        DrawEdge(Event.current);
        DrawLinks();
        DrawNodes();

        ProcessNodeEvents(Event.current);
        ProcessEvents(Event.current);

        if (GUI.changed)
            Repaint();
    }

    void SaveLandscapeData()
    {
        Landscape.NodesData.Clear();

        foreach(DryadLandscapeNode node in nodes)
            Landscape.NodesData.Add(new LandscapeNodeData(node.Id, node.Chord, node.Edges, node.Rect));

        EditorUtility.SetDirty(Landscape);
        dataHasChanged = false;
    }

    void ProcessEvents(Event e)
    {
        drag = Vector2.zero;

        switch (e.type)
        {
            case EventType.MouseDown:
                if (e.button == 0)
                    ClearNodeSelection();
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
        foreach (DryadLandscapeNode node in nodes) // might need to do the iteration backward to redraw in order of addition
        {
            if (node.ProcessEvents(e))
                GUI.changed = true;
        }
    }

    private void ProcessContextMenu(Vector2 mousePosition)
    {
        GenericMenu genericMenu = new GenericMenu();
        genericMenu.AddItem(new GUIContent($"Add {Landscape.Scale.Tonic.Name}"), false, () => OnClickAddNode(mousePosition, Landscape.Scale.Tonic));
        genericMenu.AddItem(new GUIContent($"Add {Landscape.Scale.Second.Name}"), false, () => OnClickAddNode(mousePosition, Landscape.Scale.Second));
        genericMenu.AddItem(new GUIContent($"Add {Landscape.Scale.Third.Name}"), false, () => OnClickAddNode(mousePosition, Landscape.Scale.Third));
        genericMenu.AddItem(new GUIContent($"Add {Landscape.Scale.Fourth.Name}"), false, () => OnClickAddNode(mousePosition, Landscape.Scale.Fourth));
        genericMenu.AddItem(new GUIContent($"Add {Landscape.Scale.Fifth.Name}"), false, () => OnClickAddNode(mousePosition, Landscape.Scale.Fifth));
        genericMenu.AddItem(new GUIContent($"Add {Landscape.Scale.Sixth.Name}"), false, () => OnClickAddNode(mousePosition, Landscape.Scale.Sixth));
        genericMenu.AddItem(new GUIContent($"Add {Landscape.Scale.Seventh.Name}"), false, () => OnClickAddNode(mousePosition, Landscape.Scale.Seventh));
        genericMenu.ShowAsContext();
    }

    private void CreateEdge(DryadLandscapeNode sourceNode, DryadLandscapeNode destinationNode)
    {
        edges.Add(new DryadLandscapeEdge(sourceNode, destinationNode, OnClickRemoveEdge));
        sourceNode.Edges.Add(destinationNode.Id);
        ClearNodeSelection();
        dataHasChanged = true;
    }

    bool FindNodeById(uint id, out DryadLandscapeNode nodeFound)
    {
        foreach(DryadLandscapeNode node in nodes)
        {
            if (node.Id == id)
            {
                nodeFound = node;
                return true;
            }
        }

        nodeFound = null;
        return false;
    }

    private void ClearNodeSelection()
    {
        selectedEdgeSourceNode = null;
    }

    void DrawEdge(Event e)
    {
        if (selectedEdgeSourceNode == null)
            return;

        Handles.DrawLine(selectedEdgeSourceNode.Rect.center, e.mousePosition, 2f);
        GUI.changed = true;
    }

    private void OnClickAddNode(Vector2 mousePosition, Dryad.Chord chord)
    {
        nodes.Add(new DryadLandscapeNode(new Dryad.Chord(chord), mousePosition, OnClickInitNewEdge, OnClickInNode, OnClickRemoveNode));
        dataHasChanged = true;
    }
    
    private void OnClickAddNode(LandscapeNodeData nodeData)
    {
        nodes.Add(new DryadLandscapeNode(nodeData, OnClickInitNewEdge, OnClickInNode, OnClickRemoveNode));
        dataHasChanged = true;
    }

    private void OnClickInitNewEdge(DryadLandscapeNode node)
    {
        selectedEdgeSourceNode = node;
    }

    private void OnClickInNode(DryadLandscapeNode destinationNode)
    {
        if (selectedEdgeSourceNode == null)
            return;

        if (selectedEdgeSourceNode != destinationNode)
            CreateEdge(selectedEdgeSourceNode, destinationNode);
    }

    private void OnClickRemoveEdge(DryadLandscapeEdge edge)
    {
        edges.Remove(edge);
        edge.sourceNode.Edges.Remove(edge.destinationNode.Id);
        dataHasChanged = true;
    }

    private void OnClickRemoveNode(DryadLandscapeNode node)
    {
        if (edges.Count > 0)
        {
            List<DryadLandscapeEdge> edgesToRemove = new List<DryadLandscapeEdge>();
 
            foreach(DryadLandscapeEdge edge in edges)
            {
                if (edge.sourceNode == node || edge.destinationNode == node)
                    edgesToRemove.Add(edge);
            }
 
            foreach(DryadLandscapeEdge link in edgesToRemove)
                edges.Remove(link);
        }
 
        nodes.Remove(node);
        dataHasChanged = true;
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

    void DrawNodes()
    {
        foreach (DryadLandscapeNode node in nodes)
            node.Draw();
    }

    void DrawLinks()
    {
        foreach (DryadLandscapeEdge edge in edges.ToArray())
            edge.Draw();
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
