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

public class NoteToPlay : global::System.IDisposable {
  private global::System.Runtime.InteropServices.HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal NoteToPlay(global::System.IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
  }

  internal static global::System.Runtime.InteropServices.HandleRef getCPtr(NoteToPlay obj) {
    return (obj == null) ? new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero) : obj.swigCPtr;
  }

  ~NoteToPlay() {
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
          DryadFacadePINVOKE.delete_NoteToPlay(swigCPtr);
        }
        swigCPtr = new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero);
      }
    }
  }

  public uint scoreTime {
    set {
      DryadFacadePINVOKE.NoteToPlay_scoreTime_set(swigCPtr, value);
    } 
    get {
      uint ret = DryadFacadePINVOKE.NoteToPlay_scoreTime_get(swigCPtr);
      return ret;
    } 
  }

  public uint midiValue {
    set {
      DryadFacadePINVOKE.NoteToPlay_midiValue_set(swigCPtr, value);
    } 
    get {
      uint ret = DryadFacadePINVOKE.NoteToPlay_midiValue_get(swigCPtr);
      return ret;
    } 
  }

  public uint duration {
    set {
      DryadFacadePINVOKE.NoteToPlay_duration_set(swigCPtr, value);
    } 
    get {
      uint ret = DryadFacadePINVOKE.NoteToPlay_duration_get(swigCPtr);
      return ret;
    } 
  }

  public uint parentMotif {
    set {
      DryadFacadePINVOKE.NoteToPlay_parentMotif_set(swigCPtr, value);
    } 
    get {
      uint ret = DryadFacadePINVOKE.NoteToPlay_parentMotif_get(swigCPtr);
      return ret;
    } 
  }

  public NoteToPlay() : this(DryadFacadePINVOKE.new_NoteToPlay(), true) {
  }

}

}