/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 1.3.40
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.sysfera.vishnu.api.vishnu.internal;

public class EProcessesList {
  private long swigCPtr;
  protected boolean swigCMemOwn;

  protected EProcessesList(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  protected static long getCPtr(EProcessesList obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        VISHNUJNI.delete_EProcessesList(swigCPtr);
      }
      swigCPtr = 0;
    }
  }

  public void insert_all(EProcessesList _q) {
    VISHNUJNI.EProcessesList_insert_all(swigCPtr, this, EProcessesList.getCPtr(_q), _q);
  }

  public void insert_at(long _pos, Process _obj) {
    VISHNUJNI.EProcessesList_insert_at(swigCPtr, this, _pos, Process.getCPtr(_obj), _obj);
  }

  public Process at(long _index) {
    long cPtr = VISHNUJNI.EProcessesList_at(swigCPtr, this, _index);
    return (cPtr == 0) ? null : new Process(cPtr, false);
  }

  public Process get(long _index) {
    long cPtr = VISHNUJNI.EProcessesList_get(swigCPtr, this, _index);
    return (cPtr == 0) ? null : new Process(cPtr, false);
  }

  public void push_back(Process _obj) {
    VISHNUJNI.EProcessesList_push_back(swigCPtr, this, Process.getCPtr(_obj), _obj);
  }

  public long size() {
    return VISHNUJNI.EProcessesList_size(swigCPtr, this);
  }

  public void clear() {
    VISHNUJNI.EProcessesList_clear(swigCPtr, this);
  }

}
