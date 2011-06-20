/**
 * \file slave.cpp
 * \brief This file contains the VISHNU TMS slave main function.
 * \author Daouda Traore (daouda.traore@sysfera.com)
 * \date April 2011
 */


#include <iostream>
#include <sstream>
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <iomanip>

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

//EMF
#include <ecore.hpp> // Ecore metamodel
#include <ecorecpp.hpp> // EMF4CPP utils

#include "TMS_Data.hpp"

#include "DIET_server.h"

#include "utilServer.hpp"
#include "BatchServer.hpp"
#include "VishnuException.hpp"
#include "BatchFactory.hpp"
#include "TMSVishnuException.hpp"
#include "UMSVishnuException.hpp"
#include "utilVishnu.hpp"

namespace bfs=boost::filesystem; // an alias for boost filesystem namespac
using namespace std;
using namespace vishnu;


/**
 * \brief To show how to use the slave
 * \fn int usage(char* cmd)
 * \param cmd The name of the program
 * \return Always 1
 */
void usage(char* cmd) {
  cerr << "Usage: " << cmd << "COMMANDE_TYPE[SUBMIT] <BatchType> <JobSerializedPath> <SlaveErrorPath> <JobUpdatedSerializedPath>";
  cerr << " <SubmitOptionsSerializedPath> <job_script_path>" << endl;
  cerr << "\t\t\t\t\t" << " or " << endl;
  cerr << "Usage: " << cmd << "COMMANDE_TYPE[CANCEL] <BatchType> <JobSerializedPath>  <SlaveErrorPath>" << endl;
  exit(EXIT_FAILURE);
}

/**
 * \brief The main function
 * \param argc Number of parameter
 * \param argv List of argument
 * \param envp Array of environment variables
 * \return The result of the diet sed call
 */
int
main(int argc, char* argv[], char* envp[])
{
  std::string action;
  char* jobSerializedPath = NULL;
  char* optionsPath = NULL;
  char* slaveJobFile = NULL;
  char* slaveErrorPath = NULL;
  char* jobScriptPath = NULL;
  BatchType batchType ;
  std::string batchTypeStr;

  std::ostringstream os1;
  std::ostringstream os2;//temporaire

  if(argc < 2) {
    usage(argv[0]);
  }
  action = std::string(argv[1]);
  if(action.compare("SUBMIT")==0) {
    if(argc < 8) {
       usage(argv[0]);
     }
    slaveJobFile = argv[5];
    optionsPath = argv[6];
    jobScriptPath = argv[7];
    os2 << slaveJobFile << " " << optionsPath << " " << jobScriptPath << std::endl;
  }
  else if(action.compare("CANCEL")==0) {
    if(argc < 5) {
     usage(argv[0]);
    }
  } else {
    usage(argv[0]);
  }

  // Other command-line parameters
  batchTypeStr = argv[2];
  if (batchTypeStr == "TORQUE") {
    batchType = TORQUE;
  } else if (batchTypeStr == "LOADLEVELER") {
    batchType = LOADLEVELER;
  } else {
    std::cerr << "Error: invalid value for batch type parameter (must be 'TORQUE' or 'LOADLEVLER')" << std::endl;
    throw UMSVishnuException(ERRCODE_INVALID_PARAM, "slave: invalid value for batch type parameter (must be 'TORQUE' or 'LOADLEVLER')");
  }

  jobSerializedPath = argv[3];
  slaveErrorPath = argv[4];
  os1 << argv[0] << " " << action << " " << batchTypeStr << " " << jobSerializedPath << " " << slaveErrorPath << " ";

  TMS_Data::Job_ptr job = NULL;
  TMS_Data::SubmitOptions_ptr submitOptions = NULL;
  BatchServer* batchServer;

  try {

    //To create batchServer Factory
    BatchFactory factory;
    batchServer = factory.getBatchServerInstance(batchType);
    if(batchServer==NULL) {
      throw UMSVishnuException(ERRCODE_INVALID_PARAM, "slave: getBatchServerInstance return NULL instance");
    }

    std::string jobSerialized = vishnu::get_file_content(jobSerializedPath);
    if(!parseEmfObject(std::string(jobSerialized), job)) {
      throw UMSVishnuException(ERRCODE_INVALID_PARAM, "slave: job object is not well built");
    }

    if(action.compare("SUBMIT")==0) {
      std::string options  = vishnu::get_file_content(optionsPath);
      if(!parseEmfObject(std::string(options), submitOptions)) {
        throw UMSVishnuException(ERRCODE_INVALID_PARAM, "slave: SubmitOptions object is not well built");
      }

      //Submits the job
      if(batchServer->submit(jobScriptPath, *submitOptions, *job, envp)==0){;

        //To serialize the job object
        ::ecorecpp::serializer::serializer _ser;
        std::string slaveJob = strdup(_ser.serialize_str(job).c_str());

        ::ecorecpp::serializer::serializer _ser2;
        //FIXME next variable is unused
        std::string slaveOptions = strdup(_ser2.serialize_str(submitOptions).c_str());

        std::ofstream os_slaveJobFile(slaveJobFile);
        os_slaveJobFile << slaveJob;
        os_slaveJobFile.close();
      }
    } else if(action.compare("CANCEL")==0) {
      //To cancel the job
      batchServer->cancel((*job).getJobId().c_str());

    }
  } catch (VishnuException& ve) {
    std::string errorInfo =  ve.buildExceptionString();
    std::ofstream os_error(slaveErrorPath);
    os_error << errorInfo;
    os_error.close();
  } catch (std::exception& e) {
    //FIXME next variable is unused
    std::string errorInfo = e.what();
  }

  delete job;
  delete submitOptions;
  delete batchServer;

  return 0;
}

