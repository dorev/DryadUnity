//------------------------------------------------------------------------------
// <auto-generated />
//
// This file was automatically generated by SWIG (http://www.swig.org).
// Version 4.0.2
//
// Do not make changes to this file unless you know what you are doing--modify
// the SWIG interface file instead.
//------------------------------------------------------------------------------

namespace DryadUnity {

public class NodeList : global::System.IDisposable, global::System.Collections.IEnumerable, global::System.Collections.Generic.IEnumerable<SWIGTYPE_p_Node>
 {
  private global::System.Runtime.InteropServices.HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal NodeList(global::System.IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
  }

  internal static global::System.Runtime.InteropServices.HandleRef getCPtr(NodeList obj) {
    return (obj == null) ? new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero) : obj.swigCPtr;
  }

  ~NodeList() {
    Dispose(false);
  }

  public void Dispose() {
    Dispose(true);
    global::System.GC.SuppressFinalize(this);
  }

  protected virtual void Dispose(bool disposing) {
    lock(this) {
      if (swigCPtr.Handle != global::System.IntPtr.Zero) {
        if (swigCMemOwn) {
          swigCMemOwn = false;
          DryadApiPINVOKE.delete_NodeList(swigCPtr);
        }
        swigCPtr = new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero);
      }
    }
  }

  public NodeList(global::System.Collections.IEnumerable c) : this() {
    if (c == null)
      throw new global::System.ArgumentNullException("c");
    foreach (SWIGTYPE_p_Node element in c) {
      this.Add(element);
    }
  }

  public NodeList(global::System.Collections.Generic.IEnumerable<SWIGTYPE_p_Node> c) : this() {
    if (c == null)
      throw new global::System.ArgumentNullException("c");
    foreach (SWIGTYPE_p_Node element in c) {
      this.Add(element);
    }
  }

  public bool IsFixedSize {
    get {
      return false;
    }
  }

  public bool IsReadOnly {
    get {
      return false;
    }
  }

  public SWIGTYPE_p_Node this[int index]  {
    get {
      return getitem(index);
    }
    set {
      setitem(index, value);
    }
  }

  public int Capacity {
    get {
      return (int)capacity();
    }
    set {
      if (value < size())
        throw new global::System.ArgumentOutOfRangeException("Capacity");
      reserve((uint)value);
    }
  }

  public int Count {
    get {
      return (int)size();
    }
  }

  public bool IsSynchronized {
    get {
      return false;
    }
  }

  public void CopyTo(SWIGTYPE_p_Node[] array)
  {
    CopyTo(0, array, 0, this.Count);
  }

  public void CopyTo(SWIGTYPE_p_Node[] array, int arrayIndex)
  {
    CopyTo(0, array, arrayIndex, this.Count);
  }

  public void CopyTo(int index, SWIGTYPE_p_Node[] array, int arrayIndex, int count)
  {
    if (array == null)
      throw new global::System.ArgumentNullException("array");
    if (index < 0)
      throw new global::System.ArgumentOutOfRangeException("index", "Value is less than zero");
    if (arrayIndex < 0)
      throw new global::System.ArgumentOutOfRangeException("arrayIndex", "Value is less than zero");
    if (count < 0)
      throw new global::System.ArgumentOutOfRangeException("count", "Value is less than zero");
    if (array.Rank > 1)
      throw new global::System.ArgumentException("Multi dimensional array.", "array");
    if (index+count > this.Count || arrayIndex+count > array.Length)
      throw new global::System.ArgumentException("Number of elements to copy is too large.");
    for (int i=0; i<count; i++)
      array.SetValue(getitemcopy(index+i), arrayIndex+i);
  }

  public SWIGTYPE_p_Node[] ToArray() {
    SWIGTYPE_p_Node[] array = new SWIGTYPE_p_Node[this.Count];
    this.CopyTo(array);
    return array;
  }

  global::System.Collections.Generic.IEnumerator<SWIGTYPE_p_Node> global::System.Collections.Generic.IEnumerable<SWIGTYPE_p_Node>.GetEnumerator() {
    return new NodeListEnumerator(this);
  }

  global::System.Collections.IEnumerator global::System.Collections.IEnumerable.GetEnumerator() {
    return new NodeListEnumerator(this);
  }

  public NodeListEnumerator GetEnumerator() {
    return new NodeListEnumerator(this);
  }

  // Type-safe enumerator
  /// Note that the IEnumerator documentation requires an InvalidOperationException to be thrown
  /// whenever the collection is modified. This has been done for changes in the size of the
  /// collection but not when one of the elements of the collection is modified as it is a bit
  /// tricky to detect unmanaged code that modifies the collection under our feet.
  public sealed class NodeListEnumerator : global::System.Collections.IEnumerator
    , global::System.Collections.Generic.IEnumerator<SWIGTYPE_p_Node>
  {
    private NodeList collectionRef;
    private int currentIndex;
    private object currentObject;
    private int currentSize;

    public NodeListEnumerator(NodeList collection) {
      collectionRef = collection;
      currentIndex = -1;
      currentObject = null;
      currentSize = collectionRef.Count;
    }

    // Type-safe iterator Current
    public SWIGTYPE_p_Node Current {
      get {
        if (currentIndex == -1)
          throw new global::System.InvalidOperationException("Enumeration not started.");
        if (currentIndex > currentSize - 1)
          throw new global::System.InvalidOperationException("Enumeration finished.");
        if (currentObject == null)
          throw new global::System.InvalidOperationException("Collection modified.");
        return (SWIGTYPE_p_Node)currentObject;
      }
    }

    // Type-unsafe IEnumerator.Current
    object global::System.Collections.IEnumerator.Current {
      get {
        return Current;
      }
    }

    public bool MoveNext() {
      int size = collectionRef.Count;
      bool moveOkay = (currentIndex+1 < size) && (size == currentSize);
      if (moveOkay) {
        currentIndex++;
        currentObject = collectionRef[currentIndex];
      } else {
        currentObject = null;
      }
      return moveOkay;
    }

    public void Reset() {
      currentIndex = -1;
      currentObject = null;
      if (collectionRef.Count != currentSize) {
        throw new global::System.InvalidOperationException("Collection modified.");
      }
    }

    public void Dispose() {
        currentIndex = -1;
        currentObject = null;
    }
  }

  public void Clear() {
    DryadApiPINVOKE.NodeList_Clear(swigCPtr);
  }

  public void Add(SWIGTYPE_p_Node x) {
    DryadApiPINVOKE.NodeList_Add(swigCPtr, SWIGTYPE_p_Node.getCPtr(x));
    if (DryadApiPINVOKE.SWIGPendingException.Pending) throw DryadApiPINVOKE.SWIGPendingException.Retrieve();
  }

  private uint size() {
    uint ret = DryadApiPINVOKE.NodeList_size(swigCPtr);
    return ret;
  }

  private uint capacity() {
    uint ret = DryadApiPINVOKE.NodeList_capacity(swigCPtr);
    return ret;
  }

  private void reserve(uint n) {
    DryadApiPINVOKE.NodeList_reserve(swigCPtr, n);
  }

  public NodeList() : this(DryadApiPINVOKE.new_NodeList__SWIG_0(), true) {
  }

  public NodeList(NodeList other) : this(DryadApiPINVOKE.new_NodeList__SWIG_1(NodeList.getCPtr(other)), true) {
    if (DryadApiPINVOKE.SWIGPendingException.Pending) throw DryadApiPINVOKE.SWIGPendingException.Retrieve();
  }

  public NodeList(int capacity) : this(DryadApiPINVOKE.new_NodeList__SWIG_2(capacity), true) {
    if (DryadApiPINVOKE.SWIGPendingException.Pending) throw DryadApiPINVOKE.SWIGPendingException.Retrieve();
  }

  private SWIGTYPE_p_Node getitemcopy(int index) {
    SWIGTYPE_p_Node ret = new SWIGTYPE_p_Node(DryadApiPINVOKE.NodeList_getitemcopy(swigCPtr, index), true);
    if (DryadApiPINVOKE.SWIGPendingException.Pending) throw DryadApiPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  private SWIGTYPE_p_Node getitem(int index) {
    SWIGTYPE_p_Node ret = new SWIGTYPE_p_Node(DryadApiPINVOKE.NodeList_getitem(swigCPtr, index), false);
    if (DryadApiPINVOKE.SWIGPendingException.Pending) throw DryadApiPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  private void setitem(int index, SWIGTYPE_p_Node val) {
    DryadApiPINVOKE.NodeList_setitem(swigCPtr, index, SWIGTYPE_p_Node.getCPtr(val));
    if (DryadApiPINVOKE.SWIGPendingException.Pending) throw DryadApiPINVOKE.SWIGPendingException.Retrieve();
  }

  public void AddRange(NodeList values) {
    DryadApiPINVOKE.NodeList_AddRange(swigCPtr, NodeList.getCPtr(values));
    if (DryadApiPINVOKE.SWIGPendingException.Pending) throw DryadApiPINVOKE.SWIGPendingException.Retrieve();
  }

  public NodeList GetRange(int index, int count) {
    global::System.IntPtr cPtr = DryadApiPINVOKE.NodeList_GetRange(swigCPtr, index, count);
    NodeList ret = (cPtr == global::System.IntPtr.Zero) ? null : new NodeList(cPtr, true);
    if (DryadApiPINVOKE.SWIGPendingException.Pending) throw DryadApiPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public void Insert(int index, SWIGTYPE_p_Node x) {
    DryadApiPINVOKE.NodeList_Insert(swigCPtr, index, SWIGTYPE_p_Node.getCPtr(x));
    if (DryadApiPINVOKE.SWIGPendingException.Pending) throw DryadApiPINVOKE.SWIGPendingException.Retrieve();
  }

  public void InsertRange(int index, NodeList values) {
    DryadApiPINVOKE.NodeList_InsertRange(swigCPtr, index, NodeList.getCPtr(values));
    if (DryadApiPINVOKE.SWIGPendingException.Pending) throw DryadApiPINVOKE.SWIGPendingException.Retrieve();
  }

  public void RemoveAt(int index) {
    DryadApiPINVOKE.NodeList_RemoveAt(swigCPtr, index);
    if (DryadApiPINVOKE.SWIGPendingException.Pending) throw DryadApiPINVOKE.SWIGPendingException.Retrieve();
  }

  public void RemoveRange(int index, int count) {
    DryadApiPINVOKE.NodeList_RemoveRange(swigCPtr, index, count);
    if (DryadApiPINVOKE.SWIGPendingException.Pending) throw DryadApiPINVOKE.SWIGPendingException.Retrieve();
  }

  public static NodeList Repeat(SWIGTYPE_p_Node value, int count) {
    global::System.IntPtr cPtr = DryadApiPINVOKE.NodeList_Repeat(SWIGTYPE_p_Node.getCPtr(value), count);
    NodeList ret = (cPtr == global::System.IntPtr.Zero) ? null : new NodeList(cPtr, true);
    if (DryadApiPINVOKE.SWIGPendingException.Pending) throw DryadApiPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public void Reverse() {
    DryadApiPINVOKE.NodeList_Reverse__SWIG_0(swigCPtr);
  }

  public void Reverse(int index, int count) {
    DryadApiPINVOKE.NodeList_Reverse__SWIG_1(swigCPtr, index, count);
    if (DryadApiPINVOKE.SWIGPendingException.Pending) throw DryadApiPINVOKE.SWIGPendingException.Retrieve();
  }

  public void SetRange(int index, NodeList values) {
    DryadApiPINVOKE.NodeList_SetRange(swigCPtr, index, NodeList.getCPtr(values));
    if (DryadApiPINVOKE.SWIGPendingException.Pending) throw DryadApiPINVOKE.SWIGPendingException.Retrieve();
  }

}

}