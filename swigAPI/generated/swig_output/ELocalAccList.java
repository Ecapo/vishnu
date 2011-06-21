/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 1.3.40
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.sysfera.vishnu.api.vishnu.internal;

public class ELocalAccList {
  private long swigCPtr;
  protected boolean swigCMemOwn;

  protected ELocalAccList(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  protected static long getCPtr(ELocalAccList obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        VISHNUJNI.delete_ELocalAccList(swigCPtr);
      }
      swigCPtr = 0;
    }
  }

  public void insert_all(ELocalAccList _q) {
    VISHNUJNI.ELocalAccList_insert_all(swigCPtr, this, ELocalAccList.getCPtr(_q), _q);
  }

  public void insert_at(long _pos, LocalAccount _obj) {
    VISHNUJNI.ELocalAccList_insert_at(swigCPtr, this, _pos, LocalAccount.getCPtr(_obj), _obj);
  }

  public LocalAccount get(long _index) {
    long cPtr = VISHNUJNI.ELocalAccList_get(swigCPtr, this, _index);
    return (cPtr == 0) ? null : new LocalAccount(cPtr, false);
  }

  public void push_back(LocalAccount _obj) {
    VISHNUJNI.ELocalAccList_push_back(swigCPtr, this, LocalAccount.getCPtr(_obj), _obj);
  }

  public long size() {
    return VISHNUJNI.ELocalAccList_size(swigCPtr, this);
  }

  public void clear() {
    VISHNUJNI.ELocalAccList_clear(swigCPtr, this);
  }

}