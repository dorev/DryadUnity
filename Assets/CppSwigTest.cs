using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Runtime.InteropServices;
using System.IO;

public class CppSwigTest : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        OperationValues operationValues = new OperationValues();
        operationValues.a = 2;
        operationValues.b = 4;
        int result = Calculator.Operation(operationValues, OperationType.Add);
        Debug.Log("Result " + result);

    }
}
