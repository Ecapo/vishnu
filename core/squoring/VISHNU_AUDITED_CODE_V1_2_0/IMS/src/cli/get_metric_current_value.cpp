/**
 * \file get_current_metric.cpp
 * This file defines the VISHNU command to get the metric history
 * \author Coulomb Kevin (kevin.coulomb@sysfera.com)
 */


#include "CLICmd.hpp"
#include "utilVishnu.hpp"
#include "cliError.hpp"
#include "cliUtil.hpp"
#include "api_ums.hpp"
#include "api_ims.hpp"
#include "sessionUtils.hpp"
#include "displayer.hpp"
#include <boost/bind.hpp>

namespace po = boost::program_options;

using namespace std;
using namespace vishnu;


boost::shared_ptr<Options>
makeGCMOp(string pgName,
	  boost::function1<void, IMS_Data::MetricType>& ftype,
	  string& dietConfig){
  boost::shared_ptr<Options> opt(new Options(pgName));

  // Environement option
  opt->add("dietConfig,c",
           "The diet config file",
           ENV,
           dietConfig);

  // All cli options

  opt->add("type,t",
	   "The type of the metric",
	   CONFIG,
	   ftype);

  return opt;
}

int main (int argc, char* argv[]){

  int ret; // Return value

  /******* Parsed value containers ****************/
  string dietConfig;
  string sessionKey;
  string mid;

  /********** EMF data ************/
  IMS_Data::CurMetricOp op;

  /******** Callback functions ******************/
  boost::function1<void,IMS_Data::MetricType> ftype(boost::bind(&IMS_Data::CurMetricOp::setMetricType,boost::ref(op),_1));

  /*********** Out parameters *********************/
  IMS_Data::ListMetric met;

  /**************** Describe options *************/
  boost::shared_ptr<Options> opt = makeGCMOp(argv[0], ftype,  dietConfig);

  // All cli obligatory parameters
  opt->add("machineId,m",
	   "represents the id of the machine",
	   HIDDEN,
	   mid,1);
  opt->setPosition("machineId",1);

  CLICmd cmd = CLICmd (argc, argv, opt);

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

    // DIET call : get job output
    if(false==sessionKey.empty()){
      printSessionKeyMessage();
      getMetricCurrentValue(sessionKey, mid, met, op);
    }
    displayListMetric(&met);
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