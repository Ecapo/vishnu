/**
* \file ServerFMS.cpp
* \brief This file presents the implementation of the FMS server.
* \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
* \date 16/05/2011
*/

#include "ServerFMS.hpp"
//#include "internalApi.hpp"
#include "utilVishnu.hpp"
#include "DbFactory.hpp"
#include <boost/scoped_ptr.hpp>
#include "SystemException.hpp"
#include "services.hh"
//#include "transferSrv.hh"

using namespace vishnu;
using namespace FMS_Data;
Database *ServerFMS::mdatabaseVishnu = NULL;
ServerFMS* ServerFMS::minstance = NULL;
FMSMapper *ServerFMS::mmapper = NULL;

/**
 * \brief To get the unique instance of the server
 */
ServerFMS*
ServerFMS::getInstance() {
  if (minstance == NULL) {
    minstance = new ServerFMS();
  }
  return minstance;
}

/**
* \brief To get the vishnuId
* \fn int getVishnuId()
* \return the path of the configuration file
*/
int
ServerFMS::getVishnuId() const {
  return mvishnuId;
}


/**
* \brief Constructor (private)
* \fn ServerFMS()
*/
ServerFMS::ServerFMS() : mprofile(NULL) {
}

/**
* \brief To initialize the FMS server with individual parameters
* \fn int init(std::string vishnuid)
* \param vishnuId The id of the vishnu configuration registered in the database
* \param dbConfig  The configuration of the database
* \return an error code (0 if success and 1 if an error occurs)
*/
int
ServerFMS::init(int vishnuId,
                DbConfiguration dbConfig) {


  DbFactory factory;

  try {
    mdatabaseVishnu = factory.createDatabaseInstance(dbConfig);

    mvishnuId = vishnuId;

    std::string sqlCommand("SELECT * FROM vishnu where vishnuid="+convertToString(mvishnuId));

    /*connection to the database*/
    mdatabaseVishnu->connect();

    mmapper = new FMSMapper(MapperRegistry::getInstance(), FMSMAPPERNAME);
    mmapper->registerMapper();

    /* Checking of vishnuid on the database */
    boost::scoped_ptr<DatabaseResult> result(mdatabaseVishnu->getResult(sqlCommand.c_str()));
    if (result->getResults().size() == 0) {
      SystemException e(ERRCODE_DBERR, "The vishnuid is unrecognized");
      throw e;
    }

  } catch (VishnuException& e) {
      std::cout << e.what() << std::endl;
      exit(0);
  }

  // initialization of the service table
  diet_service_table_init(NB_SRV);

  // getFileInfos

  mprofile = getInfosProfile();

  if(diet_service_table_add(mprofile, NULL, get_infos)){

    return 1;
  }
  diet_profile_desc_free(mprofile);
  
  // content of file

  mprofile = getContentProfile();

  if ( diet_service_table_add(mprofile, NULL, contentFile)){
    return 1;

  }

  diet_profile_desc_free(mprofile);


  // Head of file

  mprofile = getHeadProfile();

  if ( diet_service_table_add(mprofile, NULL, headFile)){
    return 1;

  }

  diet_profile_desc_free(mprofile);

  // Tail of file

  mprofile = getTailProfile();

  if ( diet_service_table_add(mprofile, NULL, tailFile)){

    return 1;

  }

  diet_profile_desc_free(mprofile);


  // Create file

  mprofile = getCreateFileProfile();

  if ( diet_service_table_add(mprofile, NULL, solveCreateFile)){

    return 1;

  }

  diet_profile_desc_free(mprofile);

// Create dir

  mprofile = getCreateDirProfile();

  if ( diet_service_table_add(mprofile, NULL, solveCreateDir)){

    return 1;

  }

  diet_profile_desc_free(mprofile);


  // Remove file

  mprofile = getRemoveFileProfile();

  if ( diet_service_table_add(mprofile, NULL, solveRemoveFile)){

    return 1;

  }

  diet_profile_desc_free(mprofile);

// Remove dir

  mprofile = getRemoveDirProfile();

  if ( diet_service_table_add(mprofile, NULL, solveRemoveDir)){

    return 1;

  }

  diet_profile_desc_free(mprofile);

// Change group file

  mprofile = getChangeGroupProfile();

  if ( diet_service_table_add(mprofile, NULL, solveChangeGroup)){

    return 1;

  }

  diet_profile_desc_free(mprofile);

// Change mode file

  mprofile = getChangeModeProfile();

  if ( diet_service_table_add(mprofile, NULL, solveChangeMode)){

    return 1;

  }

  diet_profile_desc_free(mprofile);

// List the directory content

  mprofile = getListDirProfile();

  if ( diet_service_table_add(mprofile, NULL, solveListDir)){

    return 1;

  }

  diet_profile_desc_free(mprofile);


// Copy file

  mprofile = getTransferFileProfile("FileCopy");

  
  if ( diet_service_table_add(mprofile, NULL, solveTransferFile<File::copy>)){

    return 1;
  }

  diet_profile_desc_free(mprofile);

// Copy Remote file

  mprofile = getTransferRemoteFileProfile("RemoteFileCopy");

  if ( diet_service_table_add(mprofile, NULL, solveTransferRemoteFile<File::copy>)){

    return 1;
  }

  diet_profile_desc_free(mprofile);

// Move file

  mprofile = getTransferFileProfile("FileMove");

  
  if ( diet_service_table_add(mprofile, NULL, solveTransferFile<File::move>)){

    return 1;
  }

  diet_profile_desc_free(mprofile);

// Move Remote file

  mprofile = getTransferRemoteFileProfile("RemoteFileMove");

  if ( diet_service_table_add(mprofile, NULL, solveTransferRemoteFile<File::move>)){

    return 1;
  }

  diet_profile_desc_free(mprofile);

  // List file transfer

  mprofile = getFileTransfersListProfile();

  if ( diet_service_table_add(mprofile, NULL, solveGetListOfFileTransfers)){

    return 1;
  }

  diet_profile_desc_free(mprofile);



  return 0;
}

/**
* \fn ~ServerFMS()
* \brief Destructor, raises an exception on error
*/
ServerFMS::~ServerFMS() {
  if (mmapper != NULL) {
    delete mmapper;
  }
  if (mdatabaseVishnu != NULL) {
    delete mdatabaseVishnu;
  }
}

