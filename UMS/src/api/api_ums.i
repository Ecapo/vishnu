/* ----------------------------------------------------
 * LICENCE
 *
 * api_ums.i
 *
 * SWIG file for the VISHNU UMS API
 */

%module VISHNU_UMS



 //%include "vishnu.i"

%{
#define SWIG_FILE_UMS_WITH_INIT
#include "UMS_Data.hpp"
#include "api_ums.hpp"
#include "VishnuException.hpp"
#include "UserException.hpp"
#include "SystemException.hpp"
#include "UMSVishnuException.hpp"
#include "TMSVishnuException.hpp"
#include "IMSVishnuException.hpp"
%}


// Instantiate the template for all lists
// the templates used within the list template must be instantiated first
 //
#ifdef SWIGPYTHON

// Remove output parameters from the command
%typemap(in, numinputs=0) UMS_Data::Session& session(UMS_Data::Session temp) {
  $1 = &temp;
}

// Add the output parameters to the result
%typemap(argout) UMS_Data::Session& session {
  PyObject *o = PyString_FromString($1->getSessionKey().c_str());
  $result = SWIG_Python_AppendOutput($result, o);
}

// Exception rule for user exception
%typemap (throws) UserException{
    SWIG_Python_Raise(SWIG_NewPointerObj((new UMSVishnuException(static_cast< const UMSVishnuException& >(_e))),SWIGTYPE_p_UserException,SWIG_POINTER_OWN), "UserException", SWIGTYPE_p_UserException); SWIG_fail;
}

#endif

#ifdef SWIGJAVA
%include "various.i"
// Use a specific typemap for strings passed by reference
%include "string.i"
%apply std::string &INOUT { std::string& tmpPassword };
%apply std::string &INOUT { std::string& sshPublicKey };

// Exception rule for system exception
%typemap (throws) SystemException{
    jclass clazz = jenv->FindClass("com/sysfera/vishnu/api/ums/internal/InternalUMSException");
    std::string ret = $1.buildExceptionString() + "#" + $1.getMsg();
    if (clazz) {
      jenv->ThrowNew(clazz, ret.c_str());
    }
    return $null;
 }

// Exception rule for user exception
%typemap (throws) UserException{
    jclass clazz = jenv->FindClass("com/sysfera/vishnu/api/ums/internal/InternalUMSException");
    std::string ret = $1.buildExceptionString() + "#" + $1.getMsg();
    if (clazz) {
      jenv->ThrowNew(clazz, ret.c_str());
    }
    return $null;
 }

// Add throw to method declaration
%javaexception ("InternalUMSException") { $action }

#endif

// Remove the parameters of vishnuInitialize
%typemap(in, numinputs=0) int argc {
  $1 = 0;
}
%typemap(in, numinputs=0) char** argv {
  $1 = NULL;
}

%include "api_ums.hpp"

#ifdef SWIGPYTHON
%include "VishnuException.hpp"
%include "UserException.hpp"
%include "SystemException.hpp"
%include "UMSVishnuException.hpp"
%include "TMSVishnuException.hpp"
%include "IMSVishnuException.hpp"
#endif
