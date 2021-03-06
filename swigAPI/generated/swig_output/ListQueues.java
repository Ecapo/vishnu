/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 1.3.40
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.sysfera.vishnu.api.vishnu.internal;

public class ListQueues extends EObject {
  private long swigCPtr;

  protected ListQueues(long cPtr, boolean cMemoryOwn) {
    super(VISHNUJNI.SWIGListQueuesUpcast(cPtr), cMemoryOwn);
    swigCPtr = cPtr;
  }

  protected static long getCPtr(ListQueues obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        VISHNUJNI.delete_ListQueues(swigCPtr);
      }
      swigCPtr = 0;
    }
    super.delete();
  }

  public ListQueues() {
    this(VISHNUJNI.new_ListQueues(), true);
  }

  public void _initialize() {
    VISHNUJNI.ListQueues__initialize(swigCPtr, this);
  }

  public int getNbQueues() {
    return VISHNUJNI.ListQueues_getNbQueues(swigCPtr, this);
  }

  public void setNbQueues(int _nbQueues) {
    VISHNUJNI.ListQueues_setNbQueues(swigCPtr, this, _nbQueues);
  }

  public EQueueList getQueues() {
    return new EQueueList(VISHNUJNI.ListQueues_getQueues(swigCPtr, this), false);
  }

  public SWIGTYPE_p_ecorecpp__mapping__any eGet(int _featureID, boolean _resolve) {
    return new SWIGTYPE_p_ecorecpp__mapping__any(VISHNUJNI.ListQueues_eGet(swigCPtr, this, _featureID, _resolve), true);
  }

  public void eSet(int _featureID, SWIGTYPE_p_ecorecpp__mapping__any _newValue) {
    VISHNUJNI.ListQueues_eSet(swigCPtr, this, _featureID, SWIGTYPE_p_ecorecpp__mapping__any.getCPtr(_newValue));
  }

  public boolean eIsSet(int _featureID) {
    return VISHNUJNI.ListQueues_eIsSet(swigCPtr, this, _featureID);
  }

  public void eUnset(int _featureID) {
    VISHNUJNI.ListQueues_eUnset(swigCPtr, this, _featureID);
  }

  public SWIGTYPE_p_ecore__EClass _eClass() {
    long cPtr = VISHNUJNI.ListQueues__eClass(swigCPtr, this);
    return (cPtr == 0) ? null : new SWIGTYPE_p_ecore__EClass(cPtr, false);
  }

}
