/**
 * \file utilsTMSClient.hpp
 * \brief This file contains the TMS package utils functions for VISHNU client Proxy classes
 * \author Daouda Traore (daouda.traore@sysfera.com)
 * and Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
 * \date February 2011
 */
#ifndef  _UTILSTMSCLIENT_H_
#define  _UTILSTMSCLIENT_H_

#include <string>
#include <iostream>
#include <sstream>

//EMF
#include <ecore.hpp> // Ecore metamodel
#include <ecorecpp.hpp> // EMF4CPP utils
#include "UMS_Data.hpp"

#include "SystemException.hpp"
#include "UMSVishnuException.hpp"
#include "TMSVishnuException.hpp"

namespace TMSUtils {

  /**
  * \brief Function to split the receiving message into error code and message error
  * \fn void raiseExceptionIfNotEmptyMsg(const std::string& msg)
  * \param msg to split
  */
  void raiseTMSExceptionIfNotEmptyMsg(const std::string& msg);

}

#endif//UTILSTMSCLIENT
