
/**
 * \file ProcessCtl.hpp
 * \brief This file defines the class to control the processes
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date 10/05/11
 */

#ifndef __PROCESSCTL__HH__
#define __PROCESSCTL__HH__

#include "data/ProcessServer.hpp"


/**
 * \class ProcessCtl
 * \brief Class dealing wwith the control of the processes
 */
class ProcessCtl{
public:
  /**
   * \brief Constructor
   */
  ProcessCtl(string mid, UserServer user);
  /**
   * \brief Destructor
   */
  ~ProcessCtl();
  /**
   * \brief Return true if the process with a diet name Pname is of type IMS
   * \param Pname: DIET name of the process
   * \return True if Pname represents an IMS SeD
   */
  bool
  isIMSSeD(string Pname);
  /**
   * \brief To relaunch a process
   * \param op: Options for restarting
   * \param machineTo: the host of the process
   * \param isAPI: If called with the external API or not
   */
  void
  restart(IMS_Data::RestartOp_ptr op, string machineTo, bool isAPI = true);
  /**
   * \brief To stop a process
   */
  void
  stop(IMS_Data::Process_ptr p);
  /**
   * To perform load schedding operations
   * \param type: The type of loadShedding
   */
  void
  loadShed(int type);
protected:
private:
  /**
   * \brief To stop all the vishnu processes on a machine
   */
  void
  stopAll();
  /**
   * \brief The process server
   */
  ProcessServer mp;
  /**
   * \brief The option to restart
   */
  IMS_Data::RestartOp mop;
  /**
   * \brief The machine id
   */
  string mmid;
  /**
   * \brief A user server
   */
  UserServer muser;
};

#endif

