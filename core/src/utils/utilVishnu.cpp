/**
* \file utilVishnu.cpp
* \brief This file implements the utils functions of the vishnu system
* \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
* \date 15/02/2011
*/

#include "utilVishnu.hpp"
#include <stdexcept>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <sstream>
#include <sys/stat.h>

#include "UserException.hpp"
#include "SystemException.hpp"
#include "TMS_Data.hpp"

namespace bfs=boost::filesystem; // an alias for boost filesystem namespace

/**
* \brief Function to convert a string to int
* \param  val a value to convert to int
* \return int value of the corresponding string
*/
int
vishnu::convertToInt(std::string val) {
  int intValue;
  std::istringstream str(val);
  str >> intValue;
  return intValue;
}

/**
 * \brief To crypt a password
 * \fn std::string cryptPassword(const std::string& salt, const std::string password)
 * \param salt The salt to use to crypt
 * \param password The password to crypt
 * \return The crypted password
 */

std::string
vishnu::cryptPassword(const std::string& salt, const std::string password) {

  std::string saltTmp="$6$"+salt+"$";
  std::string encryptedPassword=crypt(password.c_str(),saltTmp.c_str());

  return encryptedPassword.substr(saltTmp.size());
}

/**
* \brief Function to get a random number
* \fn    int generate_numbers()
* \return the number generated
*/
int
vishnu::generateNumbers() {

  boost::mt19937 gen;
  gen.seed(static_cast<boost::uint32_t>(std::time(0)));
  boost::uniform_int<> dist(1, 100000);
  boost::variate_generator<boost::mt19937&, boost::uniform_int<> > die(gen, dist);
  return die();
}

/**
 * \brief To retrieve the password
 * \param prompt: The message inviting the user to enter his/her password
 * \return The password entered.
 */
std::string
vishnu::takePassword(const std::string& prompt){
  std::string password=getpass(prompt.c_str());
  return password;

}

/**
 * \brief Simple function to convert time
 * from string format (YYYY-MM-DD H:M:S) to
 * long integer format in seconds
 * \param ts: the time in string format
 * \return the time in long integer format in seconds
 */
std::time_t
vishnu::string_to_time_t(const std::string& ts){

  // two aliases for convenience

  namespace bps= boost::posix_time;
  namespace bg=boost::gregorian;

  bps::ptime t;

  if (std::string::npos==ts.find(":")){
    t=bps::ptime(bg::from_string(ts));

  }
  else{
    t= bps::ptime (bps::time_from_string(ts));
  }

  bps::ptime epoch(bg::date(1970,1,1));


  bps::time_duration::sec_type x = (t - epoch).total_seconds();

  return std::time_t(x);


}

/**
* \brief Simple function to read the content of the regular file
* \param filePath: the path to the file
* \return The content of the file
*/
std::string
vishnu::get_file_content(const std::string& filePath){

  bfs::path file (filePath);

  // Check the existence of file
  if (((false==bfs::exists(file)) || (true==bfs::is_empty(file)))
    || (false==bfs::is_regular_file(file))) {
    throw UserException(ERRCODE_INVALID_PARAM, "can not read the file: " + filePath);
  }

  bfs::ifstream ifs (file);

  // Read the whole file into string

  std::stringstream ss;
  ss << ifs.rdbuf();

  return ss.str();

}

/**
* \brief Function to move file
* \param src: the path of the file to move
* \param dest: the destination where the file will be moved
* \param file: the name of the file in dest
* \return raises an exception on error
*/
int
vishnu::moveFile(std::string src, std::string dest, const std::string& filename) {

  bfs::path filePath(src);
  bfs::path fileDestPath(dest);
  bfs::path fileNewPath(src);
  if(filename.size()!=0) {
    fileNewPath = bfs::path(filename);
  }

  try {
    //If the destination does not exist, the file is created in the current directory
    if(!bfs::exists(fileDestPath)) {
      bfs::rename(filePath, bfs::path(bfs::current_path().string() / fileNewPath.filename() /*filePath.filename()*/));
    }
    else {
      bfs::rename(filePath, bfs::path(fileDestPath / fileNewPath.filename() /*filePath.filename()*/));
    }
  } catch (std::exception& e) {
      throw UserException(ERRCODE_INVALID_PARAM, e.what());
  }
  return 0;
}

/**
 * \brief Function to check a numerical value
 * \param value The value to check
 * \return raises an exception on error
 */
bool vishnu::isNumericalValue(const std::string value) {
  bool ret = (value.find_first_not_of("0123456789")==std::string::npos);
  if(!ret) {
    throw UserException(ERRCODE_INVALID_PARAM, ("Invalid numerical value: "+value));
  }
  return ret;
}


/**
 * \brief Function a given walltime into string
 * \param walltime The walltime to convert
 * \return the walltime converted to string
 */
std::string vishnu::convertWallTimeToString(const long& walltime) {

  long totalTime = walltime;
  long j = totalTime/86400;
  totalTime = walltime-j*86400;
  long h =  totalTime/3600;
  totalTime =  totalTime-h*3600;
  long m =  totalTime/60;
  long s =  totalTime-m*60;

  std::ostringstream StrWallTime;
  if(j > 0) {
    if(j < 10) {
      StrWallTime << "0" << j << ":";
    } else {
      StrWallTime <<  j << ":";
    }
  }

  if(h < 10) {
    StrWallTime << "0" << h << ":";
  } else {
    StrWallTime <<  h << ":";
  }

  if(m < 10) {
    StrWallTime << "0" << m << ":";
  } else {
    StrWallTime << m << ":";
  }

  if(s < 10) {
    StrWallTime << "0" << s;
  } else {
    StrWallTime << s ;
  }

  return StrWallTime.str();
}

/**
 * \brief Function a given walltime into seconds
 * \param walltime The walltime to convert
 * \return the walltime converted to seconds
 */
long vishnu::convertStringToWallTime(const std::string& walltime) {

  int seconds = 0;
  int minute = 0;
  int heure = 0;
  int jour = 0;
  std::string value;
  size_t found;

  size_t size = walltime.size();
  size_t pos = walltime.rfind(":");
  if(pos!=std::string::npos) {
    if((size-pos > 1)) {
      value = walltime.substr(pos+1, size-1-pos);
      if(isNumericalValue(value)) {
        seconds = convertToInt(value);
      }
    }
  } else {
    if(walltime.size() > 0) {
      value = walltime;
      if(isNumericalValue(value)) {
        seconds = convertToInt(value);
      }
    }
  }

  if((pos!=std::string::npos) && (pos > 0)) {
    size = pos;
    pos =  walltime.rfind(":", size-1);
    if(pos!=std::string::npos) {
      if((size-pos > 1)) {
        value = walltime.substr(pos+1, size-pos-1);
        if(isNumericalValue(value)) {
          minute = convertToInt(value);
        }
      }
    } else {
      value = walltime.substr(0, size);
      if(isNumericalValue(value)) {
        minute = convertToInt(value);
      }
    }
  }

  if((pos!=std::string::npos) && (pos > 0)) {
    size = pos;
    pos =  walltime.rfind(":", size-1);
    if(pos!=std::string::npos) {
      if((size-pos > 1)) {
        value = walltime.substr(pos+1, size-pos-1);
        if(isNumericalValue(value)) {
          heure = convertToInt(value);
        }
      }
    } else {
      value = walltime.substr(0, size);
      if(isNumericalValue(value)) {
        heure = convertToInt(value);
      }
    }
  }

  if((pos!=std::string::npos) && (pos > 0)) {
    size = pos;
    pos =  walltime.rfind(":", size-1);
    if(pos!=std::string::npos) {
      if((size-pos > 1)) {
        throw std::runtime_error("Invalid wallltime value: "+walltime);
      }
    } else {
      value = walltime.substr(0, size);
      if(isNumericalValue(value)) {
        jour = convertToInt(value);
      }
    }
  }

  long walltimeInSeconds = (jour*86400+heure*3600+minute*60+seconds);

  return walltimeInSeconds;

}

/**
* \brief Function to check the job status
* \param status the status of the job
* \return raises an exception on error
*/
void
vishnu::checkJobStatus(const int& status) {

  if ((status < -1) || (status > 8)) {
    throw UserException(ERRCODE_INVALID_PARAM, "The status value is incorrect");
  }
}

/**
* \brief Function to check the job priority
* \param priority the priority of the job
* \return raises an exception on error
*/
void
vishnu::checkJobPriority(const int& priority) {

  if ((priority < -1) || (priority > 5)) {
    throw UserException(ERRCODE_INVALID_PARAM, "The priority value is incorrect");
  }
}

/**
 * \brief Function to check the job nbNodesAndCpuPerNode 
 * \param nbNodesAndNbCpuPerNode the number of nodes and cpu per node
 * \return raises an exception on error
 */
void
vishnu::checkJobNbNodesAndNbCpuPerNode(const std::string& nbNodesAndCpuPerNode) {

  if(nbNodesAndCpuPerNode.size()!=0) {
    size_t posNbNodes;
    size_t posCpuPerNode;
    try {
      posNbNodes = nbNodesAndCpuPerNode.find(":");
      if(posNbNodes!=std::string::npos) {
       
        std::string nbNodes = nbNodesAndCpuPerNode.substr(0, posNbNodes);
        isNumericalValue(nbNodes);
        
        std::string cpuPerNode = nbNodesAndCpuPerNode.substr(posNbNodes+1, posCpuPerNode);
        isNumericalValue(cpuPerNode);
      } else {
        throw UserException(ERRCODE_INVALID_PARAM, ("Invalid NbNodesAndNbCpuPerNode value: "+nbNodesAndCpuPerNode));
      }
    } catch(UserException ue) {
      throw UserException(ERRCODE_INVALID_PARAM, ("Invalid NbNodesAndNbCpuPerNode value: "+nbNodesAndCpuPerNode));
    }
  }
}

/**
* \brief Function to get current time in seconds (UTC)
* \return the time as the number of seconds since the Epoch, 1970-01-01 00:00:00 +0000 (UTC)
*/
time_t vishnu::getCurrentTimeInUTC() {

  //the current time
  boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();

  time_t localtime = string_to_time_t(to_simple_string(now));

  return std::time(&localtime);

}

/**
* \brief Function to convert UTC time into localtime (seconds)
* \return the correspondant localtime (seconds)
*/
time_t vishnu::convertUTCtimeINLocaltime(const time_t& utctime) {

  //the current time
  boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();

  time_t currentTime = string_to_time_t(to_simple_string(now));

  long diff = currentTime-std::time(0);
  return (utctime+diff);
}

/**
* \brief Function to localtime into UTC (seconds)
* \return the diffence time (seconds)
*/
time_t vishnu::convertLocaltimeINUTCtime(const time_t& localtime) {

  //the current time
  boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();

  time_t currentTime = string_to_time_t(to_simple_string(now));

  long diff = currentTime-std::time(0);
  return (localtime-diff);
}

void vishnu::createTmpFile(char* fileName, const std::string& file_content) {

  int  file_descriptor = mkstemp( fileName ) ;
  size_t file_size = file_content.size();
  if( file_descriptor == -1 ) {
    throw SystemException(ERRCODE_SYSTEM, "SSHJobExec::createTmpFile: Cannot create new tmp file");
  }

  if(fchmod(file_descriptor, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH )!=0) {
     throw SystemException(ERRCODE_SYSTEM, "SSHJobExec::createTmpFile: reading or writing rights have" 
                                           " not been change on the path. This can lead to an error");
  }

  if( write(file_descriptor, file_content.c_str(), file_size) != file_size ) {
    throw SystemException(ERRCODE_SYSTEM, "SSHJobExec::createTmpFile: Cannot write the content int to" 
                                          "  new created file");
  }

  close(file_descriptor);
}

void vishnu::createTmpFile(char* fileName) {

  int  file_descriptor = mkstemp( fileName ) ;
  if( file_descriptor == -1 ) {
    throw SystemException(ERRCODE_SYSTEM, "SSHJobExec::createTmpFile: Cannot create new tmp file");
  }

  if(fchmod(file_descriptor, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH ) !=0) {
     throw SystemException(ERRCODE_SYSTEM, "SSHJobExec::createTmpFile: reading or writing rights have not been" 
                                           "  change on the path. This can lead to an error");
  }

  close(file_descriptor);
}

int vishnu::deleteFile(const char* fileName) {
    return unlink(fileName);
}

