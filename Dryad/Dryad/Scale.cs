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

public class Scale : global::System.IDisposable {
  private global::System.Runtime.InteropServices.HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal Scale(global::System.IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
  }

  internal static global::System.Runtime.InteropServices.HandleRef getCPtr(Scale obj) {
    return (obj == null) ? new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero) : obj.swigCPtr;
  }

  ~Scale() {
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
          DryadFacadePINVOKE.delete_Scale(swigCPtr);
        }
        swigCPtr = new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero);
      }
    }
  }

  public uint rootNote {
    set {
      DryadFacadePINVOKE.Scale_rootNote_set(swigCPtr, value);
    } 
    get {
      uint ret = DryadFacadePINVOKE.Scale_rootNote_get(swigCPtr);
      return ret;
    } 
  }

  public FlatOrSharp flatOrSharp {
    set {
      DryadFacadePINVOKE.Scale_flatOrSharp_set(swigCPtr, (int)value);
    } 
    get {
      FlatOrSharp ret = (FlatOrSharp)DryadFacadePINVOKE.Scale_flatOrSharp_get(swigCPtr);
      return ret;
    } 
  }

  public string name {
    set {
      DryadFacadePINVOKE.Scale_name_set(swigCPtr, value);
      if (DryadFacadePINVOKE.SWIGPendingException.Pending) throw DryadFacadePINVOKE.SWIGPendingException.Retrieve();
    } 
    get {
      string ret = DryadFacadePINVOKE.Scale_name_get(swigCPtr);
      if (DryadFacadePINVOKE.SWIGPendingException.Pending) throw DryadFacadePINVOKE.SWIGPendingException.Retrieve();
      return ret;
    } 
  }

  public IntervalList intervals {
    set {
      DryadFacadePINVOKE.Scale_intervals_set(swigCPtr, IntervalList.getCPtr(value));
    } 
    get {
      global::System.IntPtr cPtr = DryadFacadePINVOKE.Scale_intervals_get(swigCPtr);
      IntervalList ret = (cPtr == global::System.IntPtr.Zero) ? null : new IntervalList(cPtr, false);
      return ret;
    } 
  }

  public Scale() : this(DryadFacadePINVOKE.new_Scale(), true) {
  }

}

}
