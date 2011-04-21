/**
 * \file api_tms.cpp
 * \brief This file contains the VISHNU api functions for TMS package.
 * \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
 * \date March 2011
 */

#include "api_tms.hpp"
#include "utilVishnu.hpp"
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

/**
  * \brief The submitJob function submits job on a machine through the use of a script (scriptFilePath).
  * \fn int submitJob(const std::string& sessionKey, const std::string& machineId, const std::string& scriptFilePath, Job& jobInfo, const SubmitOptions& options)
  * \param sessionKey : The session key
  * \param machineId : Is the id of the machine on which the job must be submitted
  * \param scriptFilePath : The path to the file containing the characteristics (job command, and batch scheduler directive required or optional) of the job to submit.
  * \param jobInfo : The  Job object containing the output information (ex: jobId and jobPath) of the job to submit
  * \param options : Is an instance of the class SubmitOptions. Each optionnal value is associated to a set operation (e.g: setNbCpu(...)) in the class SubmitOptions.  If no set operation is not called on the instance object  options, the job is submitted with the options defined in the scriptFilePath. Otherewise the job is submitted with the optionnal values set by the options object and optionnal values defined in the scriptFilePath, but optionnal values set by SubmitOptions object take precedence over those in scriptFilePath. With in the object options or within the scriptFilePath, the last occurance of an optionnal value takes precedence over earlier occurance.
  * \return int : an error code
  */
int
vishnu::submitJob(const std::string& sessionKey,
          const std::string& machineId,
          const std::string& scriptFilePath,
          Job& jobInfo,
          const SubmitOptions& options)
throw (UMSVishnuException, TMSVishnuException, UserException, SystemException) {

  SessionProxy sessionProxy(sessionKey);

  jobInfo.setJobPath(scriptFilePath);
  std::string scriptContent = vishnu::get_file_content(scriptFilePath);

  JobProxy jobProxy(sessionProxy,
                    machineId,
                    jobInfo);

  int ret = jobProxy.submitJob(scriptContent, options);
  jobInfo = jobProxy.getData();

  return ret;
}

/**
* \brief The cancelJob function cancels a job from its id
* \fn int cancelJob(const std::string& sessionKey, const std::string& machineId, const std::string& jobId, std::string& infoMsg)
* \param sessionKey : The session key
* \param machineId : The id of the machine
* \param jobId : The Id of the job
* \param infoMsg : The information message
* \return int : an error code
*/
int
vishnu::cancelJob(const std::string& sessionKey,
          const std::string& machineId,
          const std::string& jobId)
throw (UMSVishnuException, TMSVishnuException, UserException, SystemException) {

  SessionProxy sessionProxy(sessionKey);

  TMS_Data::Job job;
  job.setJobId(jobId);

  return JobProxy(sessionProxy,
                  machineId,
                  job).cancelJob();

}


/**
* \brief The getJobInfo function gets information on a job from its id
* \fn int getJobInfo(const std::string& sessionKey, const std::string& machineId, const std::string& jobId, Job& jobInfos)
* \param sessionKey : The session key
* \param machineId : The id of the machine
* \param jobId : The id of the job
* \param jobInfos : The resulting information on the job
* \return int : an error code
*/

//TODO: fonction api externe à enlever car identique à liste job
int
vishnu::getJobInfo(const std::string& sessionKey,
            const std::string& machineId,
            const std::string& jobId,
            Job_ptr& jobInfos)
throw (UMSVishnuException, TMSVishnuException, UserException, SystemException) {

  std::string serviceName = "getListOfJobs_";
  serviceName.append(machineId);

  SessionProxy sessionProxy(sessionKey);

  TMS_Data::ListJobsOptions listJobsOptions;
  listJobsOptions.setJobId(jobId);

  QueryProxy<TMS_Data::ListJobsOptions, TMS_Data::ListJobs>
  query(listJobsOptions, sessionProxy, serviceName, machineId);

  TMS_Data::ListJobs* listJobs_ptr = query.list();

  if(listJobs_ptr != NULL) {
    jobInfos = listJobs_ptr->getJobs().get(0);
  }
  return 0;
}

/**
* \brief The listJobs function gets a list of all submitted jobs
* \fn int listJobs(const std::string& sessionKey, const std::string& machineId, ListJobs& listOfJobs, const ListJobsOptions& options)
* \param sessionKey : The session key
* \param machineId : The id of the machine
* \param listOfJobs : The constructed object list of jobs
* \param options : Additional options for jobs listing
* \return int : an error code
*/
int
vishnu::listJobs(const std::string& sessionKey,
          const std::string& machineId,
          ListJobs& listOfJobs,
          const ListJobsOptions& options)
throw (UMSVishnuException, TMSVishnuException, UserException, SystemException) {

  std::string serviceName = "getListOfJobs_";
  serviceName.append(machineId);

  SessionProxy sessionProxy(sessionKey);

  //To check the job status options
  checkJobStatus(options.getStatus());
  //To check the job priority options
  checkJobPriority(options.getPriority());

  QueryProxy<TMS_Data::ListJobsOptions, TMS_Data::ListJobs>
  query(options, sessionProxy, serviceName, machineId);

  TMS_Data::ListJobs* listJobs_ptr = query.list();

  if(listJobs_ptr != NULL) {
    TMS_Data::Job_ptr job;
    for(unsigned int i = 0; i < listJobs_ptr->getJobs().size(); i++) {
      job = listJobs_ptr->getJobs().get(i);
      listOfJobs.getJobs().push_back(job);
    }
  }
  return 0;
}

/**
* \brief The getJobProgress function gets the progression status of jobs
* \fn int getJobProgress(const std::string& sessionKey, const std::string& machineId, Progression& progress, const ProgressOptions& options)
* \param sessionKey : The session key
* \param machineId : Is the id of the machine to get the jobs progression.
* \param listOfProgress : Is the object containing jobs progression information
* \param options : Is an object containing the available options jobs for progression .
* \return int : an error code
*/
int
vishnu::getJobProgress(const std::string& sessionKey,
              const std::string& machineId,
              ListProgression& listOfProgress,
              const ProgressOptions& options)
throw (UMSVishnuException, TMSVishnuException, UserException, SystemException) {

  std::string serviceName = "getJobsProgression_";
  serviceName.append(machineId);

  SessionProxy sessionProxy(sessionKey);

  QueryProxy<TMS_Data::ProgressOptions, TMS_Data::ListProgression>
  query(options, sessionProxy, serviceName, machineId);

  TMS_Data::ListProgression* listProgression_ptr = query.list();

  if(listProgression_ptr != NULL) {
    TMS_Data::Progression_ptr progression;
    for(unsigned int i = 0; i < listProgression_ptr->getProgress().size(); i++) {
      progression = listProgression_ptr->getProgress().get(i);
      listOfProgress.getProgress().push_back(progression);
    }
  }
  return 0;
}

/**
* \brief The listQueues function gets queues information
* \fn int listQueues(const std::string& sessionKey, const std::string& machineId, ListQueues& listofQueues)
* \param sessionKey : The session key
* \param machineId : The id of the machine
* \param listofQueues : The list of queues
* \return int : an error code
*/
int
vishnu::listQueues(const std::string& sessionKey,
            const std::string& machineId,
            ListQueues& listofQueues)
throw (UMSVishnuException, TMSVishnuException, UserException, SystemException) {

  SessionProxy sessionProxy(sessionKey);
  TMSMachineProxy tmsMachineProxy (sessionProxy, machineId);

  TMS_Data::ListQueues_ptr listQueues_ptr = tmsMachineProxy.getMachineQueues();

  if(listQueues_ptr != NULL) {
    TMS_Data::Queue_ptr queue;
    for(unsigned int i = 0; i < listQueues_ptr->getQueues().size(); i++) {
      queue = listQueues_ptr->getQueues().get(i);
      listofQueues.getQueues().push_back(queue);
    }
  }
  return 0;
}


/**
* \brief The setMachineRefreshPeriod function sets the refresh period of output and error files contents
* \fn int setMachineRefreshPeriod(const std::string& sessionKey, const std::string& machineId, const int& value)
* \param sessionKey : The session key
* \param machineId : The id of the machine
* \param value : Is the refresh interval value (in seconds)
* \return int : an error code
*/
int
vishnu::setMachineRefreshPeriod(const std::string& sessionKey,
                        const std::string& machineId,
                        const int& value)
throw (UMSVishnuException, TMSVishnuException, UserException, SystemException) {

  SessionProxy sessionProxy(sessionKey);
  TMSMachineProxy tmsMachineProxy (sessionProxy, machineId);
  tmsMachineProxy.setMachineRefreshPeriod(value);

  return 0;
}

/**
* \brief The getMachineRefreshPeriod function gets the refresh period of output and error files contents
* \fn int getMachineRefreshPeriod(const std::string& sessionKey, const std::string& machineId)
* \param sessionKey : The session key
* \param machineId : Represents the machine id
* \param value The refresh period value
* \return the value of the refresh period
*/
int
vishnu::getMachineRefreshPeriod(const std::string& sessionKey,
                                const std::string& machineId,
                                int& value)
throw (UMSVishnuException, TMSVishnuException, UserException, SystemException) {

  SessionProxy sessionProxy(sessionKey);
  TMSMachineProxy tmsMachineProxy (sessionProxy, machineId);

   value = tmsMachineProxy.getMachineRefreshPeriod();
   return 0;
}


/**
* \brief The getJobOutput function gets outputPath and errorPath of a job from its id
* \fn int getJobOutput(const std::string& sessionKey, const std::string& machineId, const std::string& jobId, JobResult& outputInfo, std::string& infoMsg)
* \param sessionKey : The session key
* \param machineId : The Id of the machine
* \param jobId : The Id of the job
* \param outputInfo : The  Job object  containing the job output information (ex: outputPath and errorPath) of the job to submit
* \param infoMsg : The information message
* \return int : an error code
*/
int
vishnu::getJobOutput(const std::string& sessionKey,
              const std::string& machineId,
              const std::string& jobId,
              JobResult& outputInfos)
throw (UMSVishnuException, TMSVishnuException, UserException, SystemException) {

 std::string outputPath = outputInfos.getOutputPath();
 std::string errorPath = outputInfos.getErrorPath();
 if((outputPath.size()!=0)&&(!boost::filesystem::exists(outputPath))) {
    throw UMSVishnuException(ERRCODE_INVALID_PARAM, "The derectory "+outputPath+" does not exist");
  }
  if((errorPath.size()!=0)&&(!boost::filesystem::exists(errorPath))) {
    throw UMSVishnuException(ERRCODE_INVALID_PARAM, "The derectory "+errorPath+" does not exist");
  }

  SessionProxy sessionProxy(sessionKey);
  JobOutPutProxy jobOutPutProxy(sessionProxy, machineId, outputInfos);

  outputInfos = jobOutPutProxy.getJobOutPut(jobId);

  return 0;
}

/**
* \brief The getAllJobsOutput function dynamically gets outputPath and errorPath of completed jobs
* \fn int getAllJobsOutput(const std::string& sessionKey, const std::string& machineId, ListJobResults& listOfResults)
* \param sessionKey : The session key
* \param machineId : The id of the machine
* \param listOfResults : Is the list of jobs results
* \return int : an error code
*/
int
vishnu::getAllJobsOutput(const std::string& sessionKey,
                  const std::string& machineId,
                  ListJobResults& listOfResults)
throw (UMSVishnuException, TMSVishnuException, UserException, SystemException) {

  SessionProxy sessionProxy(sessionKey);
  JobResult outputInfos;
  JobOutPutProxy jobOutPutProxy(sessionProxy, machineId, outputInfos);

  TMS_Data::ListJobResults_ptr listJobResults_ptr = jobOutPutProxy.getAllJobsOutPut();

  if(listJobResults_ptr != NULL) {
    TMS_Data::JobResult_ptr jobResult;
    for(unsigned int i = 0; i < listJobResults_ptr->getResults().size(); i++) {
      jobResult = listJobResults_ptr->getResults().get(i);
      listOfResults.getResults().push_back(jobResult);
    }
  }
  return 0;
}
