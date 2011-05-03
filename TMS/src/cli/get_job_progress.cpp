/**
 * \file get_job_progress.cpp
 * This file defines the VISHNU get job progress command
 * \author Coulomb Kevin (kevin.coulomb@sysfera.com)
 */


#include "CLICmd.hpp"
#include "utilVishnu.hpp"
#include "cliError.hpp"
#include "cliUtil.hpp"
#include "api_ums.hpp"
#include "api_tms.hpp"
#include "sessionUtils.hpp"
#include <boost/bind.hpp>
#include "displayer.hpp"

namespace po = boost::program_options;

using namespace std;
using namespace vishnu;


/**
 * \brief To build options for the VISHNU submit job command
 * \param pgName : The name of the command
 * \param fjid: The id of the job
 * \param fown: The owner of the job
 * \param dietConfig: Represents the VISHNU config file
 * \return The description of all options allowed by the command
 */
boost::shared_ptr<Options>
makeGetJobProgOp(string pgName, 
		 boost::function1<void, string>& fjid,
		 boost::function1<void, string>& fown,
		 string& dietConfig){
  boost::shared_ptr<Options> opt(new Options(pgName));

  // Environement option
  opt->add("dietConfig,c",
           "The diet config file",
           ENV,
           dietConfig);

  // All cli options
  opt->add("jobid,i",
	   "The name of the job",
	   CONFIG,
	   fjid);
  opt->add("jobOwner,u",
	   "The owner of the job",
	   CONFIG,
	   fown);

  return opt;
}


int main (int argc, char* argv[]){
  
  int ret; // Return value

  /******* Parsed value containers ****************/
  string dietConfig;
  string sessionKey;
  string machineId;

  /********** EMF data ************/
  TMS_Data::ProgressOptions progOp;

  /******** Callback functions ******************/
  boost::function1<void,string> fjid(boost::bind(&TMS_Data::ProgressOptions::setJobId,boost::ref(progOp),_1));
  boost::function1<void,string> fown(boost::bind(&TMS_Data::ProgressOptions::setJobOwner,boost::ref(progOp),_1));

     
  /*********** Out parameters *********************/
  TMS_Data::ListProgression prog;

  /**************** Describe options *************/
  boost::shared_ptr<Options> opt=makeGetJobProgOp(argv[0], fjid, fown, dietConfig);

  opt->add("machineId,m",
	   "represents the id of the machine",
	   HIDDEN,
	   machineId,1);
  opt->setPosition("machineId",1);

  CLICmd cmd = CLICmd (argc, argv, opt, dietConfig);

  // Parse the cli and setting the options found
  ret = cmd.parse(env_name_mapper());

  if (ret != CLI_SUCCESS){
    helpUsage(*opt,"[options] machineId");
    return ret;
  }

  // PreProcess (adapt some parameters if necessary)
  checkVishnuConfig(*opt);  
  if ( opt->count("help")){
    helpUsage(*opt,"[options] machineId");
    return 0;
  }

  // Process command
  try {

    // initializing DIET
    if (vishnuInitialize(const_cast<char*>(dietConfig.c_str()), argc, argv)) {
      errorUsage(argv[0],dietErrorMsg,EXECERROR);
      return  CLI_ERROR_DIET ;
    }

    // get the sessionKey
    sessionKey=getLastSessionKey(getppid());

    // DIET call : get job progress
    if(false==sessionKey.empty()){
      cout <<currentSessionKeyMsg << sessionKey <<endl;
      getJobProgress(sessionKey, machineId, prog, progOp);
    }

    if(progOp.getJobId().size()==0 && progOp.getJobOwner().size()==0) {
      std::cout << prog << std::endl;
    } else {
      displayJobProgress(prog);
    }

  } catch(VishnuException& e){// catch all Vishnu runtime error
    std::string  msg = e.getMsg()+" ["+e.getMsgComp()+"]";
    errorUsage(argv[0], msg,EXECERROR);
    //check the bad session key
    if (checkBadSessionKeyError(e)){
      removeBadSessionKeyFromFile(getppid());
    }
    return e.getMsgI() ;
  } catch(std::exception& e){// catch all std runtime error
    errorUsage(argv[0],e.what());
    return CLI_ERROR_RUNTIME;
  }

  return 0;
}
