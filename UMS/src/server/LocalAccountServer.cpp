/**
* \file LocalAccountServer.cpp
* \brief This file implements the Class which manipulates VISHNU localAccount data on server side.
* \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
* \date 31/01/2011
*/

#include "LocalAccountServer.hpp"
#include "DbFactory.hpp"

/**
* \brief Constructor
* \fn LocalAccountServer(LocalAccount account, SessionServer session)
* \param account The LocalAccount data structure
* \param session The object which encapsulates session data
*/
LocalAccountServer::LocalAccountServer(UMS_Data::LocalAccount*& account, SessionServer& session):
mlocalAccount(account), msessionServer(session) {
  DbFactory factory;
  mdatabaseVishnu = factory.getDatabaseInstance();
}

/**
* \brief Function to add a new VISHNU localAccount
* \fn int add()
* \return raises an exception on error
*/
int
LocalAccountServer::add() {

  std::string numMachine;
  std::string numUser;
  std::string sqlInsert = "insert into account (machine_nummachineid, users_numuserid, "
  "aclogin, sshpathkey, home) values ";

  //Creation of the object user
  UserServer userServer = UserServer(msessionServer);
  userServer.init();

  //Creation of the object machine
  UMS_Data::Machine *machine = new UMS_Data::Machine();
  machine->setMachineId(mlocalAccount->getMachineId());
  MachineServer machineServer = MachineServer(machine);

  //if the user exists
  if (userServer.exist()) {
    //if the session key is for the owner of the local account or the user is an admin
    if (userServer.getData().getUserId().compare(mlocalAccount->getUserId()) == 0 ||
      userServer.isAdmin()){
      //if the machine exists and it is not locked
      if (machineServer.getAttribut("where machineid='"+mlocalAccount->getMachineId()+"'"
        " and status=1").size() != 0) {
        //To get the database number id of the machine
        numMachine = machineServer.getAttribut("where machineid='"+mlocalAccount->getMachineId()+"'");
        //To get the database number id of the user
        numUser = userServer.getAttribut("where userid='"+mlocalAccount->getUserId()+"'");

        //if the local account does not exist
        if (!exist(numMachine, numUser)) {

          if (!isLoginUsed(numMachine, mlocalAccount->getAcLogin())) {

            //The sql code to insert the localAccount on the database
            mdatabaseVishnu->process(sqlInsert + "('"+numMachine+"', '"+numUser+"', '"+mlocalAccount->getAcLogin()+"', "
            "'"+mlocalAccount->getSshKeyPath()+"', '"+mlocalAccount->getHomeDirectory()+"')");

            msshpublickey = machineServer.getAttribut("where "
            "machineid='"+mlocalAccount->getMachineId()+"'", "sshpublickey");
          }
          else {
            throw UMSVishnuException(ERRCODE_LOGIN_ALREADY_USED);
          }

        }//END if the local account does not exist
        else {
          UMSVishnuException e (ERRCODE_LOCAL_ACCOUNT_EXIST);
          throw e;
        }
      } //End if the machine exists and it is not locked
      else {
        UMSVishnuException e (ERRCODE_UNUSABLE_MACHINE);
        throw e;
      }
    }//End if the session key is for the owner of the local account or the user is an admin
    else {
      UMSVishnuException e (ERRCODE_NO_ADMIN);
      throw e;
    }
  }//End if the user exists
  else {
    UMSVishnuException e (ERRCODE_UNKNOWN_USER);
    throw e;
  }
  return 0;
}

/**
* \brief Function to update a VISHNU localAccount
* \fn int update()
* \return raises an exception on error
*/
int
LocalAccountServer::update() {
  std::string numMachine;
  std::string numUser;
  std::string sqlCommand = "";

  //Creation of the object use
  UserServer userServer = UserServer(msessionServer);
  userServer.init();

  //Creation of the object machine
  UMS_Data::Machine *machine = new UMS_Data::Machine();
  machine->setMachineId(mlocalAccount->getMachineId());
  MachineServer machineServer = MachineServer(machine);

  //if the user exists
  if (userServer.exist()) {
    //if the session key is for the owner of the local account or the user is an admin
    if (userServer.getData().getUserId().compare(mlocalAccount->getUserId()) == 0 ||
      userServer.isAdmin()){

      //if the machine exists and it is not locked
      if (machineServer.getAttribut("where machineid='"+mlocalAccount->getMachineId()+"'"
        " and status=1").size() != 0) {

        //To get the database number id of the machine
        numMachine = machineServer.getAttribut("where machineid='"+mlocalAccount->getMachineId()+"'");
        //To get the database number id of the user
        numUser = userServer.getAttribut("where userid='"+mlocalAccount->getUserId()+"'");

        //if the local account exists
        if (exist(numMachine, numUser)) {

          //check if the acLogin is not already used
          if (isLoginUsed(numMachine, mlocalAccount->getAcLogin())) {
            throw UMSVishnuException(ERRCODE_LOGIN_ALREADY_USED);
          }

          //if a new acLogin has been defined
          if (mlocalAccount->getAcLogin().size() != 0) {
          sqlCommand.append("UPDATE account SET aclogin='"+mlocalAccount->getAcLogin()+"'"
          " where machine_nummachineid="+numMachine+" and users_numuserid="+numUser+";");
          }

          //if a new sshpathkey has been defined
          if (mlocalAccount->getSshKeyPath().size() != 0) {
          sqlCommand.append("UPDATE account SET sshpathkey='"+mlocalAccount->getSshKeyPath()+"'"
          " where machine_nummachineid="+numMachine+" and users_numuserid="+numUser+";");
          }

          //if a new home directory has been defined
          if (mlocalAccount->getHomeDirectory().size() != 0) {
          sqlCommand.append("UPDATE account SET home='"+mlocalAccount->getHomeDirectory()+"'"
          " where machine_nummachineid="+numMachine+" and users_numuserid="+numUser+";");
          }

          //If there is a change
          if (!sqlCommand.empty()) {
            mdatabaseVishnu->process(sqlCommand.c_str());
          }

        }//END if the local account exists
        else {
          UMSVishnuException e (ERRCODE_UNKNOWN_LOCAL_ACCOUNT);
          throw e;
        }
      } //End if the machine exists and it is not locked
      else {
        UMSVishnuException e (ERRCODE_UNUSABLE_MACHINE);
        throw e;
      }
    }//if the session key is for the owner of the local account or the user is an admin
    else {
      UMSVishnuException e (ERRCODE_SESSIONKEY_NOT_FOUND);
      throw e;
    }
  }//End if the user exists
  else {
    UMSVishnuException e (ERRCODE_UNKNOWN_USER);
    throw e;
  }
  return 0;
}

/**
* \brief Function to delete a VISHNU localAccount
* \fn int deleteLocalAccount()
* \return raises an exception on error
*/
int
LocalAccountServer::deleteLocalAccount() {

  std::string numMachine;
  std::string numUser;


  //Creation of the object user
  UserServer userServer = UserServer(msessionServer);
  userServer.init();

  //Creation of the object machine
  UMS_Data::Machine *machine = new UMS_Data::Machine();
  machine->setMachineId(mlocalAccount->getMachineId());
  MachineServer machineServer = MachineServer(machine);

  //if the user exists
  if (userServer.exist()) {
    //if the session key is for the owner of the local account or the user is an admin
    if (userServer.getData().getUserId().compare(mlocalAccount->getUserId()) == 0 ||
      userServer.isAdmin()){

      //if the machine exists and it is not locked
      if (machineServer.getAttribut("where machineid='"+mlocalAccount->getMachineId()+"'"
        " and status=1").size() != 0) {

        //To get the database number id of the machine
        numMachine = machineServer.getAttribut("where machineid='"+mlocalAccount->getMachineId()+"'");
        //To get the database number id of the user
        numUser = userServer.getAttribut("where userid='"+mlocalAccount->getUserId()+"'");

        //if the local account exists
        if (exist(numMachine, numUser)) {

          //To remove the local account from the database
          mdatabaseVishnu->process("DELETE FROM account "
          "where machine_nummachineid="+numMachine+" and users_numuserid="+numUser);

        }//END if the local account exists
        else {
          UMSVishnuException e (ERRCODE_UNKNOWN_LOCAL_ACCOUNT);
          throw e;
        }
      } //END if the machine exists and it is not locked
      else {
        UMSVishnuException e (ERRCODE_UNUSABLE_MACHINE);
        throw e;
      }
    }//END if the session key is for the owner of the local account or the user is an admin
    else {
      UMSVishnuException e (ERRCODE_NO_ADMIN);
      throw e;
    }
  }//End if the user exists
  else {
    UMSVishnuException e (ERRCODE_UNKNOWN_USER);
    throw e;
  }
  return 0;
}
/**
* \fn ~LocalAccountServer
* \brief Destructor
*/
LocalAccountServer::~LocalAccountServer() {
  //delete mlocalAccount;
}
/**
* \brief Function to get localAccount information
* \fn UMS_Data::LocalAccount getData()
* \return  The LocalAccount data structure
*/
UMS_Data::LocalAccount *
LocalAccountServer::getData() {
  return mlocalAccount;
}
/**
* \brief Function to get localAccount information from the database vishnu
* \fn getAttribut(std::string condition, std::string attrname);
* \param condition The condition of the select request
* \param attrname the name of the attribut to get
* \return the value of the attribut or empty string if no results
*/
std::string
LocalAccountServer::getAttribut(std::string condition, std::string attrname) {

  std::string sqlCommand("SELECT "+attrname+" FROM account "+condition);
  boost::scoped_ptr<DatabaseResult> result(mdatabaseVishnu->getResult(sqlCommand.c_str()));
  return result->getFirstElement();
}

/**
* \brief Function to check localAccount on database
* \fn bool exist()
* \return true if the localAccount exists else false
*/
bool
LocalAccountServer::exist(std::string idmachine, std::string iduser) {

  if ((idmachine.size() > 0) && (iduser.size() > 0)) {
    return (getAttribut("where machine_nummachineid="+idmachine+" and users_numuserid="+iduser).size() != 0);
  }
  else {
    return false;
  }
}

/**
 * \brief Function to check if a given login is used on a machine
 * \param numMachine the internal id of the machine
 * \param acLogin the account login
 * \return true if the login is already used
 */
bool
LocalAccountServer::isLoginUsed(std::string numMachine, std::string acLogin) {
  if (numMachine.empty() || acLogin.empty()) {
    return false;
  }
  std::string numUser = getAttribut("where machine_nummachineid="+numMachine+" and aclogin='"+acLogin+"'", "users_numuserid");
  return !numUser.empty();
}

/**
* \brief Function to get the content of the public ssh generated
* \fn std::string getPublicKey()
* \return The content of the ssh public key generated
*/
std::string
LocalAccountServer::getPublicKey() {
  return msshpublickey;
}
