/**
 * \file configurationProxy.cpp
 * \brief This file contains the VISHNU configurationProxy class.
 * \author Daouda Traore (daouda.traore@sysfera.com)
 * \date February 2011
 */
#include "UMSVishnuException.hpp"
#include "utilsClient.hpp"

#include "ConfigurationProxy.hpp"

/**
 * \fn ConfigurationProxy(const std::string& filePath,
 *                        const SessionProxy& session)
 * \param filePath The file containing the configuration (serialized)
 * \param session The object which encapsulates the session information (ex: identifier of the session)
 * \brief Constructor, raises an exception on error
 */
ConfigurationProxy::ConfigurationProxy(const std::string& filePath, const SessionProxy& session):
  mfilePath(filePath), msessionProxy(session)
{
  mconfiguration = NULL;
}

/**
 * \fn ConfigurationProxy(UMS_Data::Configuration* config,
 *                        const SessionProxy& session)
 * \param config The object which encapsulates the configuration description 
 * \param session The object which encapsulates the session information (ex: identifier of the session)
 * \brief Constructor, raises an exception on error
 */ 
ConfigurationProxy::ConfigurationProxy(UMS_Data::Configuration* config, const SessionProxy& session):
  mconfiguration(config), msessionProxy(session)
{
}

/**
 * \brief Function to save the configuration of VISHNU 
 * \fn  int save()
 * \return raises an exception on error
 */ 
int ConfigurationProxy::save()
{
  diet_profile_t* profile = NULL;
  std::string sessionKey;
  char* errorInfo;
  char* configurationInString;
  std::string msg = "call of function diet_string_set is rejected ";

  profile = diet_profile_alloc("configurationSave", 0, 0, 2);
  //To get the encrypted identifier of the session generated by VISHNU
  sessionKey = msessionProxy.getSessionKey();

  //IN Parameters
  if(diet_string_set(diet_parameter(profile,0), strdup(sessionKey.c_str()), DIET_VOLATILE)) {
    msg += "with sessionKey parameter "+sessionKey;
    raiseDietMsgException(msg);
  }

  //OUT Parameters
  diet_string_set(diet_parameter(profile,1), NULL, DIET_VOLATILE);
  diet_string_set(diet_parameter(profile,2), NULL, DIET_VOLATILE);

  if(!diet_call(profile)) {
    if(diet_string_get(diet_parameter(profile,1), &configurationInString, NULL)){
      msg += "by receiving configurationInString message";
      raiseDietMsgException(msg);      
    }
    if(diet_string_get(diet_parameter(profile,2), &errorInfo, NULL)){
      msg += "by receiving errorInfo message";
      raiseDietMsgException(msg);
    }
  }
  else {
    raiseDietMsgException("DIET call failure");
  }

  /*To raise a vishnu exception if the receiving message is not empty*/
  raiseExceptionIfNotEmptyMsg(errorInfo);

  // CREATE DATA MODEL
  UMS_Data::UMS_DataPackage_ptr ecorePackage = UMS_Data::UMS_DataPackage::_instance();
  ecorecpp::MetaModelRepository::_instance()->load(ecorePackage);

  // Parse the model
  ecorecpp::parser::parser parser;
  //To set the mconfiguration 
  mconfiguration = parser.load(configurationInString)->as< UMS_Data::Configuration >(); 

  //To save the configuration in the file
  std::ofstream ofile(mfilePath.c_str());
  ofile << configurationInString;
  ofile.close();
  mconfiguration->setFilePath(mfilePath);
  return 0;
}

/**
 * \brief Function to combine restoreFromFile() and restoreFromData() into one function 
 * \fn  int restore(bool fromFile=true)
 * \param fromFile To select the call of restoreFromFile or restoreFromData function 
 * \return raises an exception on error
 */
int ConfigurationProxy::restore(bool fromFile)
{
  diet_profile_t* profile = NULL;
  std::string sessionKey;
  char* errorInfo;
  char* configurationInString = NULL;
  std::string msg = "call of function diet_string_set is rejected "; 

  if(fromFile) { // To get the content of the mfile
    size_t length;
    std::ifstream ifile(mfilePath.c_str()); 

    if(!ifile.is_open()) {
      throw UMSVishnuException(1, "can't open file "+mfilePath);
    }

    ifile.seekg(0, std::ios::end);
    length = ifile.tellg();
    ifile.seekg(0, std::ios::beg);

    configurationInString = new char[length];
    //Put the content of the mfile in to configurationIntring variable
    ifile.read(configurationInString, length); 
    ifile.close();
  }
  else { //To serialize the mconfiguration object in to configurationInString 
    const char* name = "ConfigurationRestore";
    ::ecorecpp::serializer::serializer _ser(name);
    configurationInString =  strdup((_ser.serialize(mconfiguration)).c_str());
  }
  //To intialize the service description 
  profile = diet_profile_alloc("configurationRestore", 1, 1, 2);
  //To get the encrypted identifier of the session generated by VISHNU
  sessionKey = msessionProxy.getSessionKey();

  //IN Parameters
  if(diet_string_set(diet_parameter(profile,0), strdup(sessionKey.c_str()), DIET_VOLATILE)) {
    msg += "with sessionKey parameter "+sessionKey;
    raiseDietMsgException(msg); 
  }
  if(diet_string_set(diet_parameter(profile,1), configurationInString, DIET_VOLATILE)) {
    msg += "with configurationInString parameter "+std::string(configurationInString);
    raiseDietMsgException(msg);
  }
  //OUT Parameters
  diet_string_set(diet_parameter(profile,2), NULL, DIET_VOLATILE);

  if(!diet_call(profile)) {
    if(diet_string_get(diet_parameter(profile,2), &errorInfo, NULL)){
      msg += "by receiving errorInfo message";
      raiseDietMsgException(msg); 
    }
  }
  else {
    raiseDietMsgException("DIET call failure");
  }

  /*To raise a vishnu exception if the receiving message is not empty*/
  raiseExceptionIfNotEmptyMsg(errorInfo);

  if(fromFile && (configurationInString!=NULL)) {
    delete [] configurationInString;
  }

  return 0;
}

/**
 * \brief Function to restore the configuration of VISHNU from a file 
 * \fn  int restoreFromFile()
 * \return raises an exception on error
 */
int ConfigurationProxy::restoreFromFile()
{
  return restore();
}

/**
 * \brief Function to restore the configuration of VISHNU from an object (Data) 
 * \fn  int restoreFromFile()
 * \return raises an exception on error
 */ 
int ConfigurationProxy::restoreFromData()
{
  return restore(false);
}

/**
 * \brief Function get the saved configuration of VISHNU 
 * \fn  UMS_Data::Configuration* getData()
 * \return saved configuration 
 * \return raises an exception on error
 */
UMS_Data::Configuration* ConfigurationProxy::getData()
{
  return mconfiguration;
}

/**
 * \brief Function get SessionProxy object which contains the VISHNU session identifier 
 * \fn SessionProxy getSessionProxy() 
 * \return a SessionProy object which contains the VISHNU session information 
 * \return raises an exception on error
 */
SessionProxy ConfigurationProxy::getSessionProxy()
{
  return msessionProxy;
}

/**
 * \fn ~ConfigurationProxy()
 * \brief Destructor, raises an exception on error
 */
ConfigurationProxy::~ConfigurationProxy()
{
}
