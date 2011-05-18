/**
 * \file ServerFMS.hpp
 * \brief This file presents the implementation of the FMS server.
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 * \date 16/05/2011
*/

#ifndef _SERVERFMS_H_
#define _SERVERFMS_H_

#include <string>
/**
 * \brief Number of service in FMS
 */
#define NB_SRV 14
#include "DIET_server.h"
#include "FMSMapper.hpp"
#include "MapperRegistry.hpp"
#include "DbConfiguration.hpp"

class Database;

static const char* SRV[NB_SRV] = {
"FileGetInfos",
"FileHead",
"FileTail"
};
/**
 * \class ServerFMS
 * \brief This class describes the FMS server
 */
class ServerFMS {
public :

  /**
   * \brief To get the unique instance of the server
   */
  static ServerFMS*  getInstance();

  /**
  * \brief To get the vishnuId
  * \fn int getVishnuId()
  * \return the path of the configuration file
  */
  int
  getVishnuId() const;


   /**
   * \brief To initialize the FMS server
   * \param vishnuId  The identifier of the vishnu instance
   * \param dbConfig  The configuration of the database
   * \return an error code (0 if success and 1 if an error occurs)
   */
  int
  init(int vishnuId, DbConfiguration dbConfig);

  /**
   * \brief Destructor, raises an exception on error
   * \fn ~ServerFMS()
   */
  ~ServerFMS();

private :

  /**
   * \brief Constructor, private because class is singleton
   */
  ServerFMS();

  /**
   * \brief operator=
   */
  ServerFMS& operator=(const ServerFMS&);

  /////////////////////////////////
  // Attributes
  /////////////////////////////////
  /**
   * \brief The singleton reference
   */
  static ServerFMS *minstance;
  
  /**
  * \brief The vishnu id
  */
  int mvishnuId;
  /**
  * \brief Structure representing a profile description
  */
  diet_profile_desc_t* mprofile;
  /**
  * \brief Instance of Database
  */
  static Database *mdatabaseVishnu;
  /**
  * \brief Instance of FMSMapper
  */
 // static FMSMapper *mmapper;
};
#endif // SERVERFMS
