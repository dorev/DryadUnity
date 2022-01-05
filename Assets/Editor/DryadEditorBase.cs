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
}
