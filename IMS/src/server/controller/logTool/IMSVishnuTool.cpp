#include "IMSVishnuTool.hpp"
#include "LogORBMgr.hh"
#include "SystemException.hpp"
#include "UserServer.hpp"
#include "IMSVishnuException.hpp"
#include "DIET_server.h"
// For gethostname
#include <unistd.h>
#include "controller/process/ProcessCtl.hpp"

using namespace vishnu;

IMSVishnuTool::IMSVishnuTool(int argc, char** argv):mproc(UserServer(SessionServer(""))){
  try {
    LogORBMgr::init(argc, argv);
  } catch (...) {
    cerr << "ORB initialization failed" << endl;
  }
  setFilter("");
  mfilename = mname+".log";
  LogORBMgr::getMgr()->activate((IMSVishnuTool*)this);
  // Getting the hostname
  if (gethostname(msyshName, HNAMESIZE-1)==-1){
    cerr << "Cannot get hostname to check process" << endl;;
  }
}

IMSVishnuTool::~IMSVishnuTool(){
}

void 
IMSVishnuTool::run(){
  // Connexion to the LCT to get messages
  mLCT = LogORBMgr::getMgr()->resolve<LogCentralTool, LogCentralTool_ptr>("LogServiceT", "LCT");
  if (CORBA::is_nil(mLCT)){
    throw SystemException(ERRCODE_SYSTEM, "Failed to narrow the LCT ! \n");
  }

  // Binding the tool
  try{
    LogORBMgr::getMgr()->bind("LogServiceT", mname, _this(), true);
    LogORBMgr::getMgr()->fwdsBind("LogServiceT", mname,
				  LogORBMgr::getMgr()->getIOR(_this()));
  }
  catch (...){
    throw SystemException(ERRCODE_SYSTEM, "Failed to bind ! \n");
  }

  // Connecting the tool
  try{
    char* name = CORBA::string_dup(mname.c_str());
    mLCT->connectTool (name, mname.c_str());
    mLCT->addFilter(mname.c_str(), mfilter);
  }
  catch (...){
    throw SystemException(ERRCODE_SYSTEM, "Failed to connect the tool ! \n");
  }
}

int
IMSVishnuTool::disconnect(){
  return mLCT->disconnectTool (mname.c_str());
}

// TODO IMPLEMENT THE FUNCTION TO PLAY ITS ROLES WITH THE DATABASE
void
IMSVishnuTool::sendMsg(const log_msg_buf_t& msg){
  // DIET call parameters
  int service_number;
  diet_profile_desc_t **tab;
  IMS_Data::RestartOp resOp;
  // For each message
  for (CORBA::ULong i=0; i<msg.length(); i++){
    string log = "";
    IMS_Data::Process_ptr p = new IMS_Data::Process();
    // componentName
    log.append(msg[i].componentName);
    log.append(":");

    // If a connexion message arrive
    if (string(msg[i].tag).compare("IN")==0){
      string hname = getHostnameFromLog(string(msg[i].msg));

      // If it is on the same machine as the imssed
      if (hname.compare(string(msyshName))==0){
	log += "Connexion of the component with the name : " + string(msg[i].componentName) + " and the message ->" + string(msg[i].msg) + "<- ";

	// Initializing the corresponding process
	p->setProcessName(string(msg[i].componentName));
	try{
	  string c = getMidFromHost(msyshName);
	  p->setMachineId(c);
	}catch(IMSVishnuException& e){
	  log.append("Machine " + string(msg[i].componentName) + " is not registered as a server");
	  ofstream dest(mfilename.c_str(),ios::app);
	  dest << log << endl;
	  dest.close();
	  continue;
	}
	try {
	  // If it is a not a SeD
	  if (diet_get_SeD_services(&service_number, &tab, msg[i].componentName)) {
	    continue;
	  }

	  // Connect the sed
	  mproc.connectProcess(p);

	  // Looking in the list of services of the sed to authentify it
	  for (unsigned int j = 0 ; j < service_number ; j++) {
	    // If ums sed
	    if (string(tab[j]->path).compare("sessionConnect")==0){
	      log.append(msg[i].tag);
	      log.append(":");
	      //msg
	      log.append(string(msg[i].msg)+", with type UMS");
	      p->setMachineId(getMidFromHost(msyshName));
	      p->setDietId(string(msg[i].componentName));
	      p->setProcessName("UMS");
	      mproc.authentifyProcess(p);
	      break;
	    }
	    // else If tms sed
	    else if ((string(tab[j]->path)).find("jobSubmit")!=string::npos) {
	      log.append(msg[i].tag);
	      log.append(":");
	      //msg
	      log.append(string(msg[i].msg)+", with type TMS");
	      p->setMachineId(getMidFromHost(msyshName));
	      p->setDietId(string(msg[i].componentName));
	      p->setProcessName("TMS");
	      mproc.authentifyProcess(p);
	      break;
	    }
	    // else if IMS sed
	    else if (string(tab[j]->path).compare("int_getProcesses")==0) {
	      log.append(msg[i].tag);
	      log.append(":");
	      //msg
	      log.append(string(msg[i].msg)+", with type IMS");
	      string t = getMidFromHost(msyshName);
	      p->setMachineId(t);
	      p->setDietId(string(msg[i].componentName));
	      p->setProcessName("IMS");
	      mproc.authentifyProcess(p);
	      break;
	    }
	    // else if FMS sed
	    else if (string(tab[j]->path).compare("copyFile")==0) {
	      log.append(msg[i].tag);
	      log.append(":");
	      //msg
	      log.append(string(msg[i].msg)+", with type FMS");
	      string t = getMidFromHost(msyshName);
	      p->setMachineId(t);
	      p->setDietId(string(msg[i].componentName));
	      p->setProcessName("FMS");
	      mproc.authentifyProcess(p);
	      break;
	    } // End else if ims
	  }// end for
	  for (unsigned j = 0 ; j<service_number ; j++) {
	    diet_profile_desc_free(tab[j]);
	  }
	  free(tab);
	}catch ( SystemException& e){
	  throw (e);
	}
	ofstream dest(mfilename.c_str(),ios::app);
	dest << log << endl;
	dest.close();
      }
    } // EDNIF IN
    // If message of disconnexion
    if (string(msg[i].tag).compare("OUT")==0){
      p->setDietId(string(msg[i].componentName));
      cout << "SeD: " << string(msg[i].componentName) << endl;
      mproc.getDataFromDietId(p);
      ProcessCtl ctl("", UserServer(SessionServer("")));
      try {
	ctl = ProcessCtl(p->getMachineId(), UserServer(SessionServer("")));
      } catch (VishnuException& e) {
	log.append(e.what());
      }
      // If it is on the same machine as the sed
      log = "Disconnexion of the component with the name : " + string(msg[i].componentName);
      p->setDietId(string(msg[i].componentName));
      try{
	mproc.disconnectProcess(p);
	int ty;
	if(p->getProcessName().compare("UMS")==0) {
	  ty = 1;
	} else if(p->getProcessName().compare("TMS")==0) {
	  ty = 2;
	} else if(p->getProcessName().compare("FMS")==0) {
	  ty = 3;
	} else if(p->getProcessName().compare("IMS")==0) {
	  ty = 4;
	} else {
	  throw SystemException(ERRCODE_SYSTEM, "Unknown component to restart, type is unknown");
	}
	resOp.setSedType(ty);
	resOp.setVishnuConf(p->getScript());
	cout << "SeD type: " << ty << endl;
	// Restart a process disconnected, if the process was stopped with a stop call, no restart will be done
	try {
	  // If local proc to restart
	  if (p->getMachineId().compare(getMidFromHost(msyshName))){
	    ctl.restart(&resOp, false);
	  } else { // Else if ims down and i am elected to relaunch it
	    if (ctl.isIMSSeD(p->getDietId()) && elect()) {
	      cout << "restarting" << endl;
	      ctl.restart(&resOp, false);
	    }
	  }
	} catch (SystemException& e) {
	  cout << "restart op: " << e.what() << endl;
	  log.append(e.what());
	}
      } catch(SystemException& e){
	throw (e);
      }// end catch
      try {
	mproc.setRestarted(p);
      } catch (SystemException& e) {
	log.append(e.what());
      }
      ofstream dest(mfilename.c_str(),ios::app);
      dest << log << endl;
      dest.close();
    }// end if disconnexion
    delete (p);
  }// End for
}

void
IMSVishnuTool::setFilter(string description_file){
  mfilter.filterName = CORBA::string_dup("allFilter");
  mfilter.tagList.length(1);
  mfilter.tagList[0] = CORBA::string_dup("IN");
  mfilter.componentList.length(1);
  mfilter.componentList[0] = CORBA::string_dup("*");
}

void
IMSVishnuTool::setFilename(string name){
  mfilename = name;
}

string
IMSVishnuTool::getHostnameFromLog(string msg){
  // Extracting hostname from msg
  int pos = msg.find_last_of(' ');
  return msg.substr(pos+1);
}

// ELECTION PROCESSUS IMS ACTIF LE PLUS RECENT
bool
IMSVishnuTool::elect() {
  string elect = mproc.getElectedHostname();
  cout << "machine elue: " << msyshName << endl;
  cout << "comparaison: " << elect.compare(msyshName) << endl;
  return (elect.compare(msyshName)==0);
}

