/**
 * \file ServerIMS.hpp
 * \brief This file presents the implementation of the IMS server.
 * \author Kevin COULOMB (kevin.coulomb@sysfera.com)
 * \date 12/03/2011
*/

#ifndef _SERVERIMS_H_
#define _SERVERIMS_H_

#include <string>
#include "DbConfiguration.hpp"
#include "Database.hpp"
#include "UMSMapper.hpp"
#include "TMSMapper.hpp"
#include "FMSMapper.hpp"
#include "IMSMapper.hpp"

/**
 * \brief Number of service in IMS
 */
#define NB_SRV 17
#include "DIET_server.h"

static const char* SRV[NB_SRV] = {
  "int_exportCommands",
  "int_getMetricCurentValue",
  "int_getMetricHistory",
  "int_getProcesses",
  "int_setSystemInfo",
  "int_setSystemThreshold",
  "int_getSystemThreshold",
  "int_defineUserIdentifier",
  "int_defineJobIdentifier",
  "int_defineTransferIdentifier",
  "int_defineMachineIdentifier",
  "int_loadShed",
  "int_setUpdateFrequency",
  "int_getUpdateFrequency",
  "int_restart",
  "int_stop",
  "int_getSystemInfo"};
/**
 * \class ServerIMS
 * \brief This class describes the IMS server
 */
class ServerIMS {
public :

  /**
   * \brief To get the unique instance of the server
   */
  static ServerIMS*  getInstance();

  /**
  * \brief To get the vishnuId
  * \fn int getVishnuId()
  * \return the path of the configuration file
  */
  int
  getVishnuId();

  /**
  * \brief To get the path to the sendmail script
  * \fn std::string getSendmailScriptPath()
  * \return the path of the configuration file
  */
  std::string
  getSendmailScriptPath();

   /**
   * \brief To initialize the IMS server
   * \fn int init(int vishnuId, DbConfiguration dbConfig, std::string sendmailScriptPath)
   * \param vishnuId The password of the root user vishnu_user for the connection with the database
   * \param dbConfig  The configuration of the database
   * \param sendmailScriptPath The path to the script for sending emails
   * \return an error code (0 if success and 1 if an error occurs)
   */
  int
  init(int vishnuId, DbConfiguration dbConfig, string sendmailScriptPath);

  /**
   * \brief Destructor, raises an exception on error
   * \fn ~ServerIMS()
   */
  ~ServerIMS();

private :

  /**
   * \brief Constructor, private because class is singleton
   */
  ServerIMS();

  /////////////////////////////////
  // Attributes
  /////////////////////////////////
  /**
  * \brief The path to the sendmail script
  */
  std::string msendmailScriptPath;
  /**
   * \brief The singleton reference
   */
  static ServerIMS *minstance;
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
  * \brief Instance of UMSMapper
  */
  static UMSMapper *mmapperUMS;
  /**
  * \brief Instance of UMSMapper
  */
  static TMSMapper *mmapperTMS;
  /**
  * \brief Instance of UMSMapper
  */
  static FMSMapper *mmapperFMS;
  /**
  * \brief Instance of UMSMapper
  */
  static IMSMapper *mmapperIMS;
};
#endif // SERVERIMS
