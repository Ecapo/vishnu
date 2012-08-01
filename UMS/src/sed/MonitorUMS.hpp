/* This file is a part of VISHNU.

* Copyright SysFera SAS (2011) 

* contact@sysfera.com

* This software is a computer program whose purpose is to provide 
* access to distributed computing resources.
*
* This software is governed by the CeCILL  license under French law and
* abiding by the rules of distribution of free software.  You can  use, 
* modify and/ or redistribute the software under the terms of the CeCILL
* license as circulated by CEA, CNRS and INRIA at the following URL
* "http://www.cecill.info". 

* As a counterpart to the access to the source code and  rights to copy,
* modify and redistribute granted by the license, users are provided only
* with a limited warranty  and the software's author,  the holder of the
* economic rights,  and the successive licensors  have only  limited
* liability. 
*
* In this respect, the user's attention is drawn to the risks associated
* with loading,  using,  modifying and/or developing or reproducing the
* software by the user in light of its specific status of free software,
* that may mean  that it is complicated to manipulate,  and  that  also
* therefore means  that it is reserved for developers  and  experienced
* professionals having in-depth computer knowledge. Users are therefore
* encouraged to load and test the software's suitability as regards their
* requirements in conditions enabling the security of their systems and/or 
* data to be ensured and,  more generally, to use and operate it in the 
* same conditions as regards security. 
*
* The fact that you are presently reading this means that you have had
* knowledge of the CeCILL license and that you accept its terms.
*/

/**
 * \file MonitorUMS.hpp
 * \brief This file presents the implementation of the UMS Monitor
 * \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
 * \date 31/01/2001
*/

#ifndef _MONITORUMS_H_
#define _MONITORUMS_H_

#include <string>
#include "internalApi.hpp"

/**
 * \class MonitorUMS
 * \brief This class describes the UMS Monitor
 */
class MonitorUMS {
public :
  /**
   * \brief Constructor, raises an exception on error
   * \fn MonitorUMS(int interval)
   * \param interval The interval in seconds to check the database
   */
  explicit MonitorUMS(int interval = 1);

  /**
   * \brief Destructor
   * \fn ~MonitorUMS()
   */
  ~MonitorUMS();

   /**
   * \brief To initialize the UMS monitor with individual parameters instead of configuration file
   * \param vishnuId The password of the root user vishnu_user for the connection with the database
   * \param dbConfig The configuration of the database
   * \return raises an execption
   */
  void
  init(int vishnuId, DbConfiguration dbConfig);

   /**
   * \brief To launch the UMS Monitor
   * \fn int run()
   * \return raises an exception
   */
  int
  run();

private :

  /////////////////////////////////
  // Attributes
  /////////////////////////////////

  /**
  * \brief The interval in seconds to check the database
  */
  int minterval;
  /**
  * \brief An instance of vishnu database
  */
  Database *mdatabaseVishnu;

};
#endif // MONITORUMS
