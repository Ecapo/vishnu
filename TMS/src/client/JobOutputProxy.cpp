#include <string>
#include <iostream>
#include "JobOutputProxy.hpp"
#include "utilClient.hpp"
#include "DIET_Dagda.h"
#include <boost/filesystem.hpp>
#include "utilVishnu.hpp"
#include <omniORB4/CORBA.h>

namespace bfs = boost::filesystem;
using namespace vishnu;

/**
* \param session The object which encapsulates the session information
* \param machine The object which encapsulates the machine information
* \param outDir The output directory where the files will be stored 
* (default is current directory)
* \brief Constructor
*/
JobOutputProxy::JobOutputProxy( const SessionProxy& session,
                  const std::string& machineId,
                  const std::string& outDir)
:msessionProxy(session), mmachineId(machineId), moutDir(outDir) {
}

/**
* \brief Function to get the job results
* \param jobId The Id of the
* \return The job results data structure
*/
//TMS_Data::JobResult_ptr
TMS_Data::JobResult
JobOutputProxy::getJobOutPut(const std::string& jobId) {

  diet_profile_t* profile = NULL;
  std::string sessionKey;
  char* jobResultToString;
  //char* jobResultInString;
  char* errorInfo = NULL;
  char* IDContainer = NULL;
  diet_container_t content;
  char* outputPath = NULL;
  char* errorPath = NULL;
  TMS_Data::JobResult_ptr outJobResult;

  TMS_Data::JobResult jobResult;

  jobResult.setJobId(jobId);

  std::string serviceName = "jobOutputGetResult_";
  serviceName.append(mmachineId);

  profile = diet_profile_alloc(serviceName.c_str(), 3, 3, 5);
  sessionKey = msessionProxy.getSessionKey();

  std::string msgErrorDiet = "call of function diet_string_set is rejected ";
   //IN Parameters
  if (diet_string_set(diet_parameter(profile,0), strdup(sessionKey.c_str()), DIET_VOLATILE)) {
    msgErrorDiet += "with sessionKey parameter "+sessionKey;
    raiseDietMsgException(msgErrorDiet);
  }

  if (diet_string_set(diet_parameter(profile,1), strdup(mmachineId.c_str()), DIET_VOLATILE)) {
    msgErrorDiet += "with machineId parameter "+mmachineId;
    raiseDietMsgException(msgErrorDiet);
  }

   const char* name = "getJobOutPut";
  ::ecorecpp::serializer::serializer _ser(name);
  //To serialize the options object in to optionsInString
  jobResultToString =  strdup(_ser.serialize(const_cast<TMS_Data::JobResult_ptr>(&jobResult)).c_str());

  if (diet_string_set(diet_parameter(profile,2), jobResultToString, DIET_VOLATILE)) {
    msgErrorDiet += "with the job result parameter "+std::string(jobResultToString);
    raiseDietMsgException(msgErrorDiet);
  }

  if (diet_string_set(diet_parameter(profile,3), strdup((moutDir.c_str())), DIET_VOLATILE)) {
    msgErrorDiet += "with outDir parameter "+moutDir;
    raiseDietMsgException(msgErrorDiet);
  }

   //OUT Parameters
  diet_string_set(diet_parameter(profile,4), NULL, DIET_VOLATILE);
  diet_container_set(diet_parameter(profile,5), DIET_PERSISTENT_RETURN);

  if(!diet_call(profile)) {
    if(diet_string_get(diet_parameter(profile,4), &errorInfo, NULL)){
      msgErrorDiet += " by receiving errorInfo message";
      raiseDietMsgException(msgErrorDiet);
    }

     /*To raise a vishnu exception if the receiving message is not empty*/
     raiseExceptionIfNotEmptyMsg(errorInfo);


    try {
      IDContainer = (profile->parameters[5]).desc.id;
      dagda_get_container(IDContainer);
      dagda_get_container_elements(IDContainer, &content);

      if (content.size == 2) {
        //To get all files from the container
        dagda_get_file(content.elt_ids[0],&outputPath);
        dagda_get_file(content.elt_ids[1],&errorPath);

         std::string outFileName = "outputOfJob_"+jobResult.getJobId();
         std::string errFileName = "errorsOfJob_"+jobResult.getJobId();

        if(moutDir.size()==0) {
          moutDir = (bfs::path(bfs::current_path().string())).string();
        }

        vishnu::boostMoveFile(std::string(outputPath), moutDir, outFileName);
        vishnu::boostMoveFile(std::string(errorPath), moutDir, errFileName);

        jobResult.setOutputPath(moutDir+"/"+outFileName);
        jobResult.setErrorPath(moutDir+"/"+errFileName);
      }
    } catch (CORBA::Exception & e) {//To catch CORBA exception sent by DAGDA
      if (content.size == 2) {
        for(unsigned int i = 0; i < content.size; i++) {
          dagda_delete_data(content.elt_ids[i]);
        }
      }
      dagda_delete_data(IDContainer);
      diet_profile_free(profile);
      throw UserException(ERRCODE_INVALID_PARAM,"CORBA Exception: "+ std::string(e._name())+
            " ("+std::string(IDContainer) + ")");
      }
    }
  else {
    raiseDietMsgException("DIET call failure");
  }
  if (content.size == 2) {
    //To clean the container
    for (unsigned int i = 0; i < content.size; i++) {
      dagda_delete_data(content.elt_ids[i]);
    }
  }
  //To clean the container Id
  dagda_delete_data(IDContainer);
  diet_profile_free(profile);
  return jobResult;
}

/**
* \brief Function to get the results of all job submitted
* \return The list of the job results
*/
//TODO: faire un try catch Vishnu exception pour nettoyer l'id Dadga dans le catch puis throw e;
TMS_Data::ListJobResults_ptr
JobOutputProxy::getAllJobsOutPut() {
  diet_profile_t* profile = NULL;
  std::string sessionKey;
  char* listJobResultInString = NULL;
  char* errorInfo = NULL;
  TMS_Data::ListJobResults_ptr listJobResults_ptr = NULL;

  char* IDContainer = NULL;
  diet_container_t content;

  std::string serviceName = "jobOutputGetCompletedJobs_";
  serviceName.append(mmachineId);

  profile = diet_profile_alloc(serviceName.c_str(), 2, 2, 5);
  sessionKey = msessionProxy.getSessionKey();

  std::string msgErrorDiet = "call of function diet_string_set is rejected ";
   //IN Parameters
  if (diet_string_set(diet_parameter(profile,0), strdup(sessionKey.c_str()), DIET_VOLATILE)) {
    msgErrorDiet += "with sessionKey parameter "+sessionKey;
    raiseDietMsgException(msgErrorDiet);
  }

  if (diet_string_set(diet_parameter(profile,1), strdup(mmachineId.c_str()), DIET_VOLATILE)) {
    msgErrorDiet += "with machineId parameter "+mmachineId;
    raiseDietMsgException(msgErrorDiet);
  }

  if (diet_string_set(diet_parameter(profile,2), strdup((moutDir.c_str())), DIET_VOLATILE)) {
    msgErrorDiet += "with outDir parameter "+moutDir;
    raiseDietMsgException(msgErrorDiet);
  }
 

   //OUT Parameters
  diet_string_set(diet_parameter(profile,3), NULL, DIET_VOLATILE);
  diet_string_set(diet_parameter(profile,4), NULL, DIET_VOLATILE);
  diet_container_set(diet_parameter(profile,5), DIET_PERSISTENT_RETURN);

  if(!diet_call(profile)) {

    if(diet_string_get(diet_parameter(profile,3), &listJobResultInString, NULL)){
      msgErrorDiet += " by receiving User serialized  message";
      raiseDietMsgException(msgErrorDiet);
    }

    if(diet_string_get(diet_parameter(profile,4), &errorInfo, NULL)){
      msgErrorDiet += " by receiving errorInfo message";
      raiseDietMsgException(msgErrorDiet);
    }

    /*To raise a vishnu exception if the receiving message is not empty*/
    raiseExceptionIfNotEmptyMsg(errorInfo);

    //To build the listJobResults_ptr
    parseEmfObject(std::string(listJobResultInString), listJobResults_ptr);

    if(moutDir.size()==0) {
      moutDir = (bfs::path(bfs::current_path().string())).string();
    }

    try {
      IDContainer = (profile->parameters[5]).desc.id;
      dagda_get_container(IDContainer);
      dagda_get_container_elements(IDContainer, &content);

      if (content.size > 1) {
        //To get all files from the container
        for(unsigned int i = 0; i < content.size; i++) {
          char* path = NULL;
          dagda_get_file(content.elt_ids[i],&path);
          
          std::string outFileName = "outputOfJob_"+listJobResults_ptr->getResults().get((i/2))->getJobId();
          vishnu::boostMoveFile(std::string(path), moutDir, outFileName); 
          listJobResults_ptr->getResults().get((i/2))->setOutputPath(moutDir+"/"+outFileName) ;
         
          if(i < content.size-1) {
            i++;
            char* errorPath = NULL;
            dagda_get_file(content.elt_ids[i],&errorPath);
            
            std::string errFileName =  "errorsOfJob_"+listJobResults_ptr->getResults().get((i/2))->getJobId();
            vishnu::boostMoveFile(std::string(errorPath), moutDir, errFileName);
            listJobResults_ptr->getResults().get((i/2))->setErrorPath(moutDir+"/"+errFileName);
          }
        }
        //To clean the container
        for(unsigned int i = 0; i < content.size; i++) {
          dagda_delete_data(content.elt_ids[i]);
        }
      }
    } catch (CORBA::Exception & e) {//To catch CORBA exception sent by DAGDA
        if (content.size > 1) {
          for(unsigned int i = 0; i < content.size; i++) {
            dagda_delete_data(content.elt_ids[i]);
          }
        }
        dagda_delete_data(IDContainer);
        diet_profile_free(profile);
        throw UserException(ERRCODE_INVALID_PARAM, "CORBA Exception: "+std::string(e._name())+
        " ("+std::string(IDContainer) + ")");
      }
  }
  else {
    raiseDietMsgException("DIET call failure");
  }
  //To clean the container Id
  dagda_delete_data(IDContainer);
  diet_profile_free(profile);
  return listJobResults_ptr;
}

/**
* \brief Destructor
*/

JobOutputProxy::~JobOutputProxy() {
}
