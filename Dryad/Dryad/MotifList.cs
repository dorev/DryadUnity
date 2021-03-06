//------------------------------------------------------------------------------
// <auto-generated />
//
// This file was automatically generated by SWIG (http://www.swig.org).
// Version 4.0.2
//
// Do not make changes to this file unless you know what you are doing--modify
// the SWIG interface file instead.
//------------------------------------------------------------------------------

namespace Dryad {

public class MotifList : global::System.IDisposable, global::System.Collections.IEnumerable, global::System.Collections.Generic.IEnumerable<MotifNote>
 {
  private global::System.Runtime.InteropServices.HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal MotifList(global::System.IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
  }

  internal static global::System.Runtime.InteropServices.HandleRef getCPtr(MotifList obj) {
    return (obj == null) ? new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero) : obj.swigCPtr;
  }

  ~MotifList() {
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
          DryadFacadePINVOKE.delete_MotifList(swigCPtr);
        }
        swigCPtr = new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero);
      }
    }
  }

  public MotifList(global::System.Collections.IEnumerable c) : this() {
    if (c == null)
      throw new global::System.ArgumentNullException("c");
    foreach (MotifNote element in c) {
      this.Add(element);
    }
  }

  public MotifList(global::System.Collections.Generic.IEnumerable<MotifNote> c) : this() {
    if (c == null)
      throw new global::System.ArgumentNullException("c");
    foreach (MotifNote element in c) {
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

  public MotifNote this[int index]  {
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

  public void CopyTo(MotifNote[] array)
  {
    CopyTo(0, array, 0, this.Count);
  }

  public void CopyTo(MotifNote[] array, int arrayIndex)
  {
    CopyTo(0, array, arrayIndex, this.Count);
  }

  public void CopyTo(int index, MotifNote[] array, int arrayIndex, int count)
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

  public MotifNote[] ToArray() {
    MotifNote[] array = new MotifNote[this.Count];
    this.CopyTo(array);
    return array;
  }

  global::System.Collections.Generic.IEnumerator<MotifNote> global::System.Collections.Generic.IEnumerable<MotifNote>.GetEnumerator() {
    return new MotifListEnumerator(this);
  }

  global::System.Collections.IEnumerator global::System.Collections.IEnumerable.GetEnumerator() {
    return new MotifListEnumerator(this);
  }

  public MotifListEnumerator GetEnumerator() {
    return new MotifListEnumerator(this);
  }

  // Type-safe enumerator
  /// Note that the IEnumerator documentation requires an InvalidOperationException to be thrown
  /// whenever the collection is modified. This has been done for changes in the size of the
  /// collection but not when one of the elements of the collection is modified as it is a bit
  /// tricky to detect unmanaged code that modifies the collection under our feet.
  public sealed class MotifListEnumerator : global::System.Collections.IEnumerator
    , global::System.Collections.Generic.IEnumerator<MotifNote>
  {
    private MotifList collectionRef;
    private int currentIndex;
    private object currentObject;
    private int currentSize;

    public MotifListEnumerator(MotifList collection) {
      collectionRef = collection;
      currentIndex = -1;
      currentObject = null;
      currentSize = collectionRef.Count;
    }

    // Type-safe iterator Current
    public MotifNote Current {
      get {
        if (currentIndex == -1)
          throw new global::System.InvalidOperationException("Enumeration not started.");
        if (currentIndex > currentSize - 1)
          throw new global::System.InvalidOperationException("Enumeration finished.");
        if (currentObject == null)
          throw new global::System.InvalidOperationException("Collection modified.");
        return (MotifNote)currentObject;
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
    DryadFacadePINVOKE.MotifList_Clear(swigCPtr);
  }

  public void Add(MotifNote x) {
    DryadFacadePINVOKE.MotifList_Add(swigCPtr, MotifNote.getCPtr(x));
    if (DryadFacadePINVOKE.SWIGPendingException.Pending) throw DryadFacadePINVOKE.SWIGPendingException.Retrieve();
  }

  private uint size() {
    uint ret = DryadFacadePINVOKE.MotifList_size(swigCPtr);
    return ret;
  }

  private uint capacity() {
    uint ret = DryadFacadePINVOKE.MotifList_capacity(swigCPtr);
    return ret;
  }

  private void reserve(uint n) {
    DryadFacadePINVOKE.MotifList_reserve(swigCPtr, n);
  }

  public MotifList() : this(DryadFacadePINVOKE.new_MotifList__SWIG_0(), true) {
  }

  public MotifList(MotifList other) : this(DryadFacadePINVOKE.new_MotifList__SWIG_1(MotifList.getCPtr(other)), true) {
    if (DryadFacadePINVOKE.SWIGPendingException.Pending) throw DryadFacadePINVOKE.SWIGPendingException.Retrieve();
  }

  public MotifList(int capacity) : this(DryadFacadePINVOKE.new_MotifList__SWIG_2(capacity), true) {
    if (DryadFacadePINVOKE.SWIGPendingException.Pending) throw DryadFacadePINVOKE.SWIGPendingException.Retrieve();
  }

  private MotifNote getitemcopy(int index) {
    MotifNote ret = new MotifNote(DryadFacadePINVOKE.MotifList_getitemcopy(swigCPtr, index), true);
    if (DryadFacadePINVOKE.SWIGPendingException.Pending) throw DryadFacadePINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  private MotifNote getitem(int index) {
    MotifNote ret = new MotifNote(DryadFacadePINVOKE.MotifList_getitem(swigCPtr, index), false);
    if (DryadFacadePINVOKE.SWIGPendingException.Pending) throw DryadFacadePINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  private void setitem(int index, MotifNote val) {
    DryadFacadePINVOKE.MotifList_setitem(swigCPtr, index, MotifNote.getCPtr(val));
    if (DryadFacadePINVOKE.SWIGPendingException.Pending) throw DryadFacadePINVOKE.SWIGPendingException.Retrieve();
  }

  public void AddRange(MotifList values) {
    DryadFacadePINVOKE.MotifList_AddRange(swigCPtr, MotifList.getCPtr(values));
    if (DryadFacadePINVOKE.SWIGPendingException.Pending) throw DryadFacadePINVOKE.SWIGPendingException.Retrieve();
  }

  public MotifList GetRange(int index, int count) {
    global::System.IntPtr cPtr = DryadFacadePINVOKE.MotifList_GetRange(swigCPtr, index, count);
    MotifList ret = (cPtr == global::System.IntPtr.Zero) ? null : new MotifList(cPtr, true);
    if (DryadFacadePINVOKE.SWIGPendingException.Pending) throw DryadFacadePINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public void Insert(int index, MotifNote x) {
    DryadFacadePINVOKE.MotifList_Insert(swigCPtr, index, MotifNote.getCPtr(x));
    if (DryadFacadePINVOKE.SWIGPendingException.Pending) throw DryadFacadePINVOKE.SWIGPendingException.Retrieve();
  }

  public void InsertRange(int index, MotifList values) {
    DryadFacadePINVOKE.MotifList_InsertRange(swigCPtr, index, MotifList.getCPtr(values));
    if (DryadFacadePINVOKE.SWIGPendingException.Pending) throw DryadFacadePINVOKE.SWIGPendingException.Retrieve();
  }

  public void RemoveAt(int index) {
    DryadFacadePINVOKE.MotifList_RemoveAt(swigCPtr, index);
    if (DryadFacadePINVOKE.SWIGPendingException.Pending) throw DryadFacadePINVOKE.SWIGPendingException.Retrieve();
  }

  public void RemoveRange(int index, int count) {
    DryadFacadePINVOKE.MotifList_RemoveRange(swigCPtr, index, count);
    if (DryadFacadePINVOKE.SWIGPendingException.Pending) throw DryadFacadePINVOKE.SWIGPendingException.Retrieve();
  }

  public static MotifList Repeat(MotifNote value, int count) {
    global::System.IntPtr cPtr = DryadFacadePINVOKE.MotifList_Repeat(MotifNote.getCPtr(value), count);
    MotifList ret = (cPtr == global::System.IntPtr.Zero) ? null : new MotifList(cPtr, true);
    if (DryadFacadePINVOKE.SWIGPendingException.Pending) throw DryadFacadePINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public void Reverse() {
    DryadFacadePINVOKE.MotifList_Reverse__SWIG_0(swigCPtr);
  }

  public void Reverse(int index, int count) {
    DryadFacadePINVOKE.MotifList_Reverse__SWIG_1(swigCPtr, index, count);
    if (DryadFacadePINVOKE.SWIGPendingException.Pending) throw DryadFacadePINVOKE.SWIGPendingException.Retrieve();
  }

  public void SetRange(int index, MotifList values) {
    DryadFacadePINVOKE.MotifList_SetRange(swigCPtr, index, MotifList.getCPtr(values));
    if (DryadFacadePINVOKE.SWIGPendingException.Pending) throw DryadFacadePINVOKE.SWIGPendingException.Retrieve();
  }

}

}
