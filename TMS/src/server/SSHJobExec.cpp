#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <sys/stat.h>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem.hpp>
//EMF
#include <ecore.hpp> // Ecore metamodel
#include <ecorecpp.hpp> // EMF4CPP utils

#include "TMS_Data.hpp"
#include "utilVishnu.hpp"
#include "utilServer.hpp"
#include "SystemException.hpp"
#include "TMSVishnuException.hpp"
#include "UMSVishnuException.hpp"
#include "SSHJobExec.hpp"
#include "ServerTMS.hpp"


const std::string TMS_SERVER_FILES_DIR="/tmp";

SSHJobExec::SSHJobExec() {
  mscript_path = NULL;
  mjobSerialized = "";
  msubmitOptionsSerialized = "";
  mbatchType = UNDEFINED;
  merrorInfo = "";
  muser = "";
  mhostname = "";
  msshKey = "";
}

SSHJobExec::SSHJobExec(char* script_path,
                       const std::string& jobSerialized,
                       const std::string& submitOptionsSerialized,
                       const std::string& user,
                       const std::string& hostname,
                       const std::string& sshKey,
                       BatchType batchType)
{
  mscript_path = script_path;
  mjobSerialized = jobSerialized;
  msubmitOptionsSerialized = submitOptionsSerialized;
  mbatchType = batchType;
  merrorInfo = "";
  muser = user;
  mhostname = hostname;
  msshKey = sshKey;
}

std::string SSHJobExec::getJobSerialized() {
 return mjobSerialized;
}

std::string SSHJobExec::getErrorInfo() {
 return merrorInfo;
}

void SSHJobExec::createTmpFile(char* fileName, const std::string& file_content) {

  int  file_descriptor = mkstemp( fileName ) ;
  size_t file_size = file_content.size();
  if( file_descriptor == -1 ) {
    std::cerr << "SSHJobExec: Cannot create new tmp file" << std::endl;
    throw SystemException(ERRCODE_SYSTEM, "SSHJobExec::createTmpFile: Cannot create new tmp file");
  }

  if(fchmod(file_descriptor, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH )!=0) {
     std::cerr << "SSHJobExec: WARNING: reading or writing rights have not been change on the path. This can lead to an error" << std::endl;
     throw SystemException(ERRCODE_SYSTEM, "SSHJobExec::createTmpFile: reading or writing rights have not been change on the path. This can lead to an error");
  }

  if( write(file_descriptor, file_content.c_str(), file_size) != file_size ) {
    std::cerr << "SSHJobExec: Cannot write the content int to new created file" << std::endl;
    throw SystemException(ERRCODE_SYSTEM, "SSHJobExec::createTmpFile: Cannot write the content int to new created file");
  }

  close(file_descriptor);
}

void SSHJobExec::createTmpFile(char* fileName) {

  int  file_descriptor = mkstemp( fileName ) ;
  if( file_descriptor == -1 ) {
    std::cerr << "SSHJobExec: Cannot create new tmp file" << std::endl;
    throw SystemException(ERRCODE_SYSTEM, "SSHJobExec::createTmpFile: Cannot create new tmp file");
  }

  if(fchmod(file_descriptor, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH ) !=0) {
     std::cerr << "SSHJobExec: WARNING: reading or writing rights rights have not been set on the path. This can lead to an error" << std::endl;
     throw SystemException(ERRCODE_SYSTEM, "SSHJobExec::createTmpFile: reading or writing rights have not been change on the path. This can lead to an error");
  }

  close(file_descriptor);
}

int SSHJobExec::deleteFile(const char* fileName) {
  return unlink(fileName);
}

int SSHJobExec::sshexec(const std::string& action) {

  std::string jobSerializedPath;
  std::string submitOptionsSerializedPath;
  std::string jobUpdateSerializedPath;
  std::string errorPath;
  std::string stderrFilePath;
  bool wellSubmitted = false;

  jobSerializedPath = TMS_SERVER_FILES_DIR+"/jobSerializedXXXXXX";
  createTmpFile(const_cast<char*>(jobSerializedPath.c_str()), mjobSerialized);

  submitOptionsSerializedPath = TMS_SERVER_FILES_DIR+"/submitOptionsSerializedXXXXXX";
  createTmpFile(const_cast<char*>(submitOptionsSerializedPath.c_str()), msubmitOptionsSerialized);

  jobUpdateSerializedPath = TMS_SERVER_FILES_DIR+"/jobUpdateSerializedXXXXXX";
  createTmpFile(const_cast<char*>(jobUpdateSerializedPath.c_str()));

  errorPath = TMS_SERVER_FILES_DIR+"/errorPathXXXXXX";
  createTmpFile(const_cast<char*>(errorPath.c_str()));

  std::ostringstream cmd;
  cmd << "ssh -l " << muser << " " << mhostname << " ";
  //cmd << "/usr/bin/ssh localhost ";
  cmd << ServerTMS::getSlaveDirectory() << "/tmsSlave ";
  cmd << action << " ";
  cmd << convertBatchTypeToString(mbatchType) << " ";
  cmd << jobSerializedPath << " " <<  errorPath << " ";
  if(action.compare("SUBMIT")==0) {
    cmd <<  jobUpdateSerializedPath << " " <<  submitOptionsSerializedPath;
    cmd << " " << mscript_path;
  }

  stderrFilePath = TMS_SERVER_FILES_DIR+"/stderrFilePathXXXXXX";
  createTmpFile(const_cast<char*>(stderrFilePath.c_str()));
  cmd << " 2> " << stderrFilePath;

  std::cout << cmd.str() << std::endl;
  if(system((cmd.str()).c_str())) { //A REMPLACER PAR exec
    std::cerr << "can't execute " << cmd.str() << std::endl;
    deleteFile(jobSerializedPath.c_str());
    deleteFile(submitOptionsSerializedPath.c_str());
    deleteFile(jobUpdateSerializedPath.c_str());
    deleteFile(errorPath.c_str());
    boost::filesystem::path stderrFile(stderrFilePath.c_str());
    if(!boost::filesystem::is_empty(stderrFile)) {
      std::string stderrMsg = "SSHJobExec::sshexec: can't execute tmsSlave executable with ssh -l, the error are: \n";
      merrorInfo = vishnu::get_file_content(stderrFilePath);
      stderrMsg.append(merrorInfo);
      merrorInfo = stderrMsg;
      //merrorInfo = stderrMsg.substr(0, stderrMsg.find_last_of('\n'));
      std::cout << "merrorInfo = " << merrorInfo << std::endl;
    }
    deleteFile(stderrFilePath.c_str());
    throw UMSVishnuException(ERRCODE_INVALID_PARAM, merrorInfo);
  }

  boost::filesystem::path jobUpdateSerializedFile(jobUpdateSerializedPath);
  if(!boost::filesystem::is_empty(jobUpdateSerializedFile)) {
    std::string jobSerialized = vishnu::get_file_content(jobUpdateSerializedPath);
    TMS_Data::Job_ptr job = NULL;
    if(!vishnu::parseEmfObject(std::string(jobSerialized), job)) {
      deleteFile(jobSerializedPath.c_str());
      deleteFile(submitOptionsSerializedPath.c_str());
      deleteFile(jobUpdateSerializedPath.c_str());
      deleteFile(errorPath.c_str());
      deleteFile(stderrFilePath.c_str());
      throw UMSVishnuException(ERRCODE_INVALID_PARAM, "SSHJobExec::sshexec: job object is not well built");
    }
    ::ecorecpp::serializer::serializer _ser("job");
    mjobSerialized = strdup(_ser.serialize(job).c_str());
    wellSubmitted = true;
    delete job;
  }

  boost::filesystem::path errorFile(errorPath.c_str());
  if(!boost::filesystem::is_empty(errorFile)) {
    merrorInfo = vishnu::get_file_content(errorPath);
    merrorInfo = merrorInfo.substr(0, merrorInfo.find_last_of('\n'));
    std::cout << "merrorInfo = " << merrorInfo << std::endl;
  }

  if((mbatchType==LOADLEVELER) && (wellSubmitted==false)) {
    boost::filesystem::path stderrFile(stderrFilePath.c_str());
    if(!boost::filesystem::is_empty(stderrFile)) {
      merrorInfo = vishnu::get_file_content(stderrFilePath);

      std::ostringstream errorMsgSerialized;
      errorMsgSerialized << ERRCODE_BATCH_SCHEDULER_ERROR << "#" << "LOADLEVELER ERROR: ";
      errorMsgSerialized << merrorInfo;
      merrorInfo = errorMsgSerialized.str();
      merrorInfo = merrorInfo.substr(0, merrorInfo.find_last_of('\n'));
      std::cout << "merrorInfo = " << merrorInfo << std::endl;
    }
  }


  deleteFile(jobSerializedPath.c_str());
  deleteFile(submitOptionsSerializedPath.c_str());
  deleteFile(jobUpdateSerializedPath.c_str());
  deleteFile(errorPath.c_str());
  deleteFile(stderrFilePath.c_str());
}

std::string SSHJobExec::convertBatchTypeToString(BatchType batchType) {

  switch(batchType) {
    case TORQUE:
      return "TORQUE";
    case LOADLEVELER:
      return "LOADLEVELER";
    default:
      return "";
  }
}

int SSHJobExec::copyFiles(const std::string& outputPath, const std::string& errorPath , const char* copyOfOutputPath, const char* copyOfErrorPath) {

  std::ostringstream cmd1;
  cmd1 << "scp " << muser << "@" << mhostname << ":" << outputPath << " " << copyOfOutputPath;
  if(system((cmd1.str()).c_str())) { //A REMPLACER PAR exec
    std::cerr << "can't execute " << cmd1.str() << std::endl;
    throw SystemException(ERRCODE_SYSTEM, "SSHJobExec::copyFiles: problem to get the output or error file on this user local account");
  }

  std::ostringstream cmd2;
  cmd2 << "scp " << muser << "@" << mhostname << ":" << errorPath << " " << copyOfErrorPath;
  if(system((cmd2.str()).c_str())) { //A REMPLACER PAR exec
    std::cerr << "can't execute " << cmd2.str() << std::endl;
    throw SystemException(ERRCODE_SYSTEM, "SSHJobExec::copyFiles: problem to get the output or error file on this user local account");
  }
}

SSHJobExec::~SSHJobExec() {

}
