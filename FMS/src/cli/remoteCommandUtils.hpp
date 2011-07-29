/**
 * \file remoteCommandUtils.hpp
 * \brief This file declare useful functions and types for the VISHNU remote
 * commands
 * \author Ibrahima Cissé (ibrahima.cisse@sysfera.com)
 */



#ifndef REMOTECOMMANDUTILS_HH
#define REMOTECOMMANDUTILS_HH


#include<iostream>
#include<boost/shared_ptr.hpp>

#include <ecore.hpp> // Ecore metamodel
#include <ecorecpp.hpp> // EMF4CPP utils
#include "FMS_Data.hpp"

#include "utilVishnu.hpp"
#include "cliError.hpp"
#include "cliUtil.hpp"
#include "cmdArgs.hpp"
#include "CLICmd.hpp"
#include "GenericCli.hpp"
class Options;

using namespace std;
using namespace vishnu;

/**
 * \brief To build options for the VISHNU command line
 * \param pgName : The name of the command
 * \param dietConfig Represents the VISHNU config file
 * \param path  Represents the path of the file
 * \return  The built command
 */
boost::shared_ptr<Options>
makeRemoteCommandOpt(string pgName,
           string& dietConfig,
           string& path){


  boost::shared_ptr<Options> opt=processOpt(pgName, dietConfig);
  
  opt->add("path,p",
      "represents the path of the file",
      HIDDEN,
      path,1);

  opt->setPosition("path",1);


  return opt;
}


/**
 * \brief A generic functio to parse remote command involving a file 
 * \param argc The number of command line arguments
 * \param argv the command line arguments
 * \param dietConfig the diet client config file
 * \param path the path of the file 
 */
void ParseRemoteCommandOptions (int argc, char* argv[],std::string& dietConfig, std::string& path){

// build remote command options

boost::shared_ptr<Options> opt=processOpt(argv[0], dietConfig);
  
opt->add("path,p",
      "The file following the pattern [host:]file path.",
      HIDDEN,
      path,1);
  opt->setPosition("path",1);

bool isEmpty;
GenericCli().processListOpt( opt, isEmpty,argc,argv,"path");

}

/**
 * \brief  A define type
 */

typedef enum{
  CREATEFILE,
  CREATEDIR,
  REMOVEFILE,
  REMOVEDIR
} FileActionType;

/**
 * \brief A functor to handle file action (create/remove) api function
 */
template<FileActionType fileActionType>
struct FileActionFunc {

  std::string mpath;
  RmFileOptions moptions;

  FileActionFunc(const std::string& path):mpath(path){}
  FileActionFunc(const std::string& path,const RmFileOptions& options):mpath(path),moptions(options){}

  int operator()(std::string sessionKey) {

    int res = -1;
    switch(fileActionType) {
      case CREATEFILE:
        res = createFile(sessionKey,mpath);
        break;
      case CREATEDIR:
        res = createDir(sessionKey,mpath);
        break;
      case REMOVEFILE:
        res = removeFile(sessionKey,mpath,moptions);
        break;
      case REMOVEDIR:
        res = removeDir(sessionKey,mpath);
        break;
      default:
        break;
    }

    return res;
  }
};

#endif
