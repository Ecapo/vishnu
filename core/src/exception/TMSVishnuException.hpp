/**
 * \file TMSVishnuException.hh
 * \brief This file defines the TMS sub class for the Vishnu exceptions.
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 */
#ifndef __TMSVISHNUEXCEPTION__
#define __TMSVISHNUEXCEPTION__

#include "UserException.hpp"

// Definitions of TMS specific error codes
static const int ERRCODE_UNKNOWN_BATCH_SCHEDULER = 101;
static const int ERRCODE_BATCH_SCHEDULER_ERROR = 102;
static const int ERRCODE_UNKNOWN_JOBID = 103;
static const int ERRCODE_PERMISSION_DENIED = 104;
static const int ERRCODE_ALREADY_TERMINATED = 105;
static const int ERRCODE_ALREADY_CANCELED = 106;
static const int ERRCODE_JOB_IS_NOT_TERMINATED = 107;


class TMSVishnuException: public UserException{

  public:
  /**
   * \brief Default constructor
   * \fn TMSVishnuException()
   */
  TMSVishnuException();
  /**
   * \brief Copy constructor
   * \fn UMSVishnuException()
   */
  TMSVishnuException(const TMSVishnuException& e);
  /**
   * \brief Constructor
   * \fn UMSVishnuException()
   */
  TMSVishnuException(int msg, std::string msgComp= "");
  /**
   * \brief Default constructor
   * \fn ~TMSVishnuException()
   */
  ~TMSVishnuException() throw (){};

  /**
   * \brief Function to initialize all UMS generic messages
   * \fn void initMsg()
   */
  void
  initMsg();


};

#endif // TMSVishnuException
