using UnityEngine;
using UnityEditor;
using System.Collections.Generic;

enum LandscapeScale
{
    Major,
    MinorNatural,
    MinorMelodic,
    Custom
}


[CustomEditor(typeof(DryadLandscape))]
public class DryadLandscapeInspector : Editor
{
    DryadLandscape script;
    LandscapeScale landscapeScale = LandscapeScale.Major;
    Dryad.Note landscapeScaleRoot;
    string customScaleName = "Custom scale";
    int customSecondInterval = 2;
    int customThirdInterval = 4;
    int customFourthInterval = 5;
    int customFifthInterval = 7;
    int customSixthInterval = 9;
    int customSeventhInterval = 11;
    Dryad.FlatOrSharp customFlatOrSharp = Dryad.FlatOrSharp.Unspecified;
    List<string> warnings = new List<string>();

    private void OnEnable()
    {
        script = target as DryadLandscape;
    }

    public override void OnInspectorGUI()
    {
        base.OnInspectorGUI();
        
        serializedObject.Update();
        DryadLandscape castedTarget = target as DryadLandscape;

        script.Name = EditorGUILayout.TextField("Name", script.Name);
        //EditorGUILayout.PropertyField(serializedObject.FindProperty("Name"));

        landscapeScaleRoot = (Dryad.Note)EditorGUILayout.EnumPopup("Root", landscapeScaleRoot);
        landscapeScale = (LandscapeScale)EditorGUILayout.EnumPopup("Scale", landscapeScale);

        switch (landscapeScale)
        {
            case LandscapeScale.Major:
                script.Scale = Dryad.Scale.Major;
                break;
            case LandscapeScale.MinorNatural:
                script.Scale = Dryad.Scale.MinorNatural;
                break;
            case LandscapeScale.MinorMelodic:
                script.Scale = Dryad.Scale.MinorMelodic;
                break;
            case LandscapeScale.Custom:
                EditorGUI.indentLevel++;
                customScaleName = EditorGUILayout.TextField("Name", customScaleName);
                customSecondInterval = EditorGUILayout.IntField("Second", customSecondInterval);
                customThirdInterval = EditorGUILayout.IntField("Third", customThirdInterval);
                customFourthInterval = EditorGUILayout.IntField("Fourth", customFourthInterval);
                customFifthInterval = EditorGUILayout.IntField("Fifth", customFifthInterval);
                customSixthInterval = EditorGUILayout.IntField("Sixth", customSixthInterval);
                customSeventhInterval = EditorGUILayout.IntField("Seventh", customSeventhInterval);
                customFlatOrSharp = (Dryad.FlatOrSharp)EditorGUILayout.EnumPopup("Flat or Sharp", customFlatOrSharp);
                EditorGUI.indentLevel--;

                if(!CustomScaleHasError())
                    script.Scale = new Dryad.Scale(
                        new uint[6]
                        {
                            (uint)customSecondInterval,
                            (uint)customThirdInterval,
                            (uint)customFourthInterval,
                            (uint)customFifthInterval,
                            (uint)customSixthInterval,
                            (uint)customSeventhInterval
                        },
                        48 + (uint)landscapeScaleRoot,
                        customFlatOrSharp);
                break;
        }

        EditorGUI.BeginDisabledGroup(CustomScaleHasError());
        if (GUILayout.Button("Open Landscape Graph Editor"))
            script.OpenGraphEditor();

        EditorGUI.EndDisabledGroup();
        AddWarningsToGUI();

        //serializedObject.ApplyModifiedProperties();

        if (GUI.changed)
            EditorUtility.SetDirty(castedTarget);
    }

    bool CustomScaleHasError()
    {
        if (landscapeScale != LandscapeScale.Custom)
            return false;

        warnings.Clear();

        if (customSecondInterval <= 0 || customThirdInterval <= 0 || customFourthInterval <= 0 || customFifthInterval <= 0 || customSixthInterval <= 0 || customSeventhInterval <= 0)
            warnings.Add("Scale contains one or more negative or null interval");
        
        if (customSecondInterval >= 12 || customThirdInterval >= 12 || customFourthInterval >= 12 || customFifthInterval >= 12 || customSixthInterval >= 12 || customSeventhInterval >= 12)
            warnings.Add("Scale contains one or more interval equal or bigger than an octave");

        if (customThirdInterval <= customSecondInterval)
            warnings.Add("Third interval should be higher than the second");

        if (customFourthInterval <= customThirdInterval)
            warnings.Add("Fourth interval should be higher than the third");

        if (customFifthInterval <= customFourthInterval)
            warnings.Add("Fifth interval should be higher than the fourth");

        if (customSixthInterval <= customFifthInterval)
            warnings.Add("Sixth interval should be higher than the fifth");

        if (customSeventhInterval <= customSixthInterval)
            warnings.Add("Seventh interval should be higher than the sixth");

        return warnings.Count > 0;
    }

    void AddWarningsToGUI()
    {
        if (landscapeScale != LandscapeScale.Custom)
            return;
        foreach (string warning in warnings)
            EditorGUILayout.HelpBox(warning, MessageType.Error);
    }
}