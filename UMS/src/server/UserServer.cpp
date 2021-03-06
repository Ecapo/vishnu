/**
* \file UserServer.cpp
* \brief This file implements the Class which manipulates VISHNU user data on server side.
* \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
* \date 31/01/2011
*/

#include <boost/scoped_ptr.hpp>
#include "UserServer.hpp"
#include "DbFactory.hpp"
#include "DatabaseResult.hpp"
#include "LocalAccountServer.hpp"
#include "utilVishnu.hpp"
#include "utilServer.hpp"
#include "AuthenticatorFactory.hpp"

/**
* \brief Constructor
* \fn UserServer(std::string userId, std::string password)
* \param userId The userId of the user
* \param password The password of the user
*/
UserServer::UserServer(std::string userId, std::string password) {
  DbFactory factory;
  muser.setUserId(userId);
  muser.setPassword(password);
  mdatabaseVishnu = factory.getDatabaseInstance();
  msessionServer = NULL;
}

/**
* \brief Constructor
* \fn UserServer(const UMS_Data::User& user)
* \param user The user data structure
*/
UserServer::UserServer(const UMS_Data::User& user):muser(user) {
  DbFactory factory;
  mdatabaseVishnu = factory.getDatabaseInstance();
  msessionServer = NULL;
}

/**
* \brief Constructor
* \fn UserServer(SessionServer sessionServer)
* \param sessionServer The object to manipulate session
*/
UserServer::UserServer(SessionServer sessionServer): msessionServer(&sessionServer) {
  DbFactory factory;
  mdatabaseVishnu = factory.getDatabaseInstance();
}

/**
* \brief Function to add a new VISHNU user
* \fn int add(UMS_Data::User*& user, int vishnuId)
* \param user The user data structure
* \param vishnuId The identifier of the vishnu instance
* \param sendmailScriptPath The path to the script for sending emails
* \return raises an exception on error
*/
int
UserServer::add(UMS_Data::User*& user, int vishnuId, std::string sendmailScriptPath) {
  std::string pwd;
  std::string sqlUpdate = "update users set ";

  std::string idUserGenerated;
  std::string passwordCrypted;
  std::string formatiduser;

  if (exist()) {
    if (isAdmin()) {

      //Generation of password
      pwd = generatePassword(user->getLastname(), user->getFirstname());
      user->setPassword(pwd.substr(0,PASSWORD_MAX_SIZE));

      //Generation of userid
      idUserGenerated = vishnu::getObjectId(vishnuId,
                                            "formatiduser",
                                            USER,
                                            user->getLastname());
      user->setUserId(idUserGenerated);
      //To get the password encrypted
      passwordCrypted = vishnu::cryptPassword(user->getUserId(), user->getPassword());

      // If there only one field reserved by getObjectId
      if (getAttribut("where userid='"+user->getUserId()+"'","count(numuserid)") == "1") {

        //To active the user status
        user->setStatus(ACTIVE_STATUS);

        sqlUpdate+="vishnu_vishnuid="+convertToString(vishnuId)+", ";
        sqlUpdate+="pwd='"+passwordCrypted+"', ";
        sqlUpdate+="firstname='"+user->getFirstname()+"', ";
        sqlUpdate+="lastname='"+user->getLastname()+"', ";
        sqlUpdate+="privilege="+convertToString(user->getPrivilege())+", ";
        sqlUpdate+="email='"+user->getEmail()+"', ";
        sqlUpdate+="passwordstate=0, ";
        sqlUpdate+="status="+convertToString(user->getStatus())+" ";
        sqlUpdate+="where userid='"+user->getUserId()+"';";
        mdatabaseVishnu->process(sqlUpdate);


        //Send email
        std::string emailBody = getMailContent(*user, true);
        sendMailToUser(*user, emailBody, "Vishnu message: user created", sendmailScriptPath);

      }// END If the user to add exists
      else {
        UMSVishnuException e (ERRCODE_USERID_EXISTING);
        throw e;
      }
    } //END if the user is an admin
    else {
      UMSVishnuException e (ERRCODE_NO_ADMIN);
      throw e;
    }
  } //END if the user exists
  else {
    UMSVishnuException e (ERRCODE_UNKNOWN_USER);
    throw e;
  }
  return 0;
}//END: add(UMS_Data::User*& user)

/**
* \brief Function to update user information
* \fn int update(UMS_Data::User*& user)
* \param user The user data structure
* \return raises an exception on error
*/
int
UserServer::update(UMS_Data::User *user) {
  std::string sqlCommand = "";

  if (exist()) {
    if (isAdmin()) {
      //if the user whose information will be updated exists
      if (getAttribut("where userid='"+user->getUserId()+"'").size() != 0) {

        //if a new fisrtname has been defined
        if (user->getFirstname().size() != 0) {
          sqlCommand.append("UPDATE users SET firstname='"+user->getFirstname()+"'"
          " where userid='"+user->getUserId()+"';");
        }

        //if a new lastname has been defined
        if (user->getLastname().size() != 0) {
          sqlCommand.append("UPDATE users SET lastname='"+user->getLastname()+"'"
          " where userid='"+user->getUserId()+"';");
        }

        //if a new email has been defined
        if (user->getEmail().size() != 0) {
          sqlCommand.append("UPDATE users SET email='"+user->getEmail()+"'"
          " where userid='"+user->getUserId()+"';");
        }

        //If a new status has been defined
        if (user->getStatus() != UNDEFINED_VALUE) {
          //if the user will be locked
          if (user->getStatus() == 0) {
            //if the user is not already locked
            if (convertToInt(getAttribut("where userid='"+user->getUserId()+"'", "status")) != 0) {
              sqlCommand.append("UPDATE users SET status="+convertToString(user->getStatus())+""
              " where userid='"+user->getUserId()+"';");
            } //End if the user is not already locked
            else {
              UMSVishnuException e (ERRCODE_USER_ALREADY_LOCKED);
              throw e;
            }
          } //End if the user will be locked
          else {
            sqlCommand.append("UPDATE users SET status="+convertToString(user->getStatus())+""
            " where userid='"+user->getUserId()+"';");
          }
        }
        // if the user whose privilege will be updated is not an admin
        if (convertToInt(getAttribut("where userid='"+user->getUserId()+"'", "privilege")) != 1) {
          sqlCommand.append("UPDATE users SET privilege="+convertToString(user->getPrivilege())+""
          " where userid='"+user->getUserId()+"';");
        }

        //If there is a change
        if (!sqlCommand.empty()) {
          mdatabaseVishnu->process(sqlCommand.c_str());
        }
      } // End if the user whose information will be updated exists
      else {
        UMSVishnuException e (ERRCODE_UNKNOWN_USERID);
        throw e;
      }
    } //END if the user is an admin
    else {
      UMSVishnuException e (ERRCODE_NO_ADMIN);
      throw e;
    }
  } //END if the user exists
  else {
    UMSVishnuException e (ERRCODE_UNKNOWN_USER);
    throw e;
  }
  return 0;
} //END: update(UMS_Data::User *user)

/**
* \brief Function to delete VISHNU user
* \fn int deleteUser(UMS_Data::User user)
* \param user The user data structure
* \return raises an exception on error
*/
int
UserServer::deleteUser(UMS_Data::User user) {

  //If the user to delete is not the super VISHNU admin
  if (user.getUserId().compare(ROOTUSERNAME) != 0) {
    //If the user exists
    if (exist()) {
      if (isAdmin()) {
        //if the user who will be deleted exist
        if (getAttribut("where userid='"+user.getUserId()+"'").size() != 0) {
          //Execution of the sql code to delete the user
          mdatabaseVishnu->process("DELETE FROM users where userid='"+user.getUserId()+"'");
        } // End if the user who will be deleted exist
        else {
          UMSVishnuException e (ERRCODE_UNKNOWN_USERID);
          throw e;
        }
      } //END if the user is an admin
      else {
        UMSVishnuException e (ERRCODE_NO_ADMIN);
        throw e;
      }
    } //END if the user exists
    else {
      UMSVishnuException e (ERRCODE_UNKNOWN_USER);
      throw e;
    }
  }//END If the user to delete is not the super VISHNU admin
  else {
    UserException e (ERRCODE_INVALID_PARAM, "It is not possible to delete this user. It is the VISHNU root user");
    throw e;
  }
  return 0;
}//END: deleteUser(UMS_Data::User user)

/**
* \brief Function to change VISHNU user password
* \fn int changePassword(std::string newPassword)
* \param newPassword The new password of the user
* \return raises an exception on error
*/
int
UserServer::changePassword(std::string newPassword) {
  std::string sqlChangePwd;
  std::string sqlUpdatePwdState;
  //the flagForChangePwd is set to true to avoid the password state checking

  //If the user exist
  if (isAuthenticate(true)) {
    //If the identifiers used for the connection are a global VISHNU identifiers registered on UMS database
    if (getAttribut("where userid='"+muser.getUserId()+"'").size() != 0) {
      //To encrypt password with the global userId as a salt
      newPassword = vishnu::cryptPassword(muser.getUserId(), newPassword);

      //sql code to change the user password
      sqlChangePwd = "UPDATE users SET pwd='"+newPassword+"'where "
      "userid='"+muser.getUserId()+"' and pwd='"+muser.getPassword()+"';";

      //sql code to update the passwordstate
      sqlUpdatePwdState = "UPDATE users SET passwordstate=1 "
      "where userid='"+muser.getUserId()+"' and pwd='"+newPassword+"';";

      sqlChangePwd.append(sqlUpdatePwdState);
      mdatabaseVishnu->process(sqlChangePwd.c_str());

      //Put the new user's password
      muser.setPassword(newPassword);
    }//End //If the identifiers used for the connection are a global VISHNU identifiers
    else {
      UMSVishnuException e (ERRCODE_READONLY_ACCOUNT);
      throw e;
    }
  } //End If the user exist with the flagForChangePwd to true ti avoid the passwordstate checking
  else {
    UMSVishnuException e (ERRCODE_UNKNOWN_USER);
    throw e;
  }
  return 0;
}//END: changePassword(std::string newPassword)

/**
* \brief Function to change VISHNU user password
* \fn int resetPassword(UMS_Data::User user, std::string sendmailScriptPath)
* \param user The user data structure
* \param sendmailScriptPath The path to the script for sending emails
* \return raises an exception on error
*/
int
UserServer::resetPassword(UMS_Data::User& user, std::string sendmailScriptPath) {
  std::string sqlResetPwd;
  std::string sqlUpdatePwdState;
  std::string passwordCrypted;
  std::string pwd;


  //If the user exists
  if (exist()) {
    //if the user is an admin
    if (isAdmin()) {
      //if the user whose password will be reset exists
      if (getAttribut("where userid='"+user.getUserId()+"'").size() != 0) {
        //generation of a new password
        pwd = generatePassword(user.getUserId(), user.getUserId());
        user.setPassword(pwd.substr(0,PASSWORD_MAX_SIZE));

        //to get the password encryptes
        passwordCrypted = vishnu::cryptPassword(user.getUserId(), user.getPassword());

        //The sql code to reset the password
        sqlResetPwd = "UPDATE users SET pwd='"+passwordCrypted+"' where "
        "userid='"+user.getUserId()+"';";
        //sql code to update the passwordstate
        sqlUpdatePwdState = "UPDATE users SET passwordstate=0 "
        "where userid='"+user.getUserId()+"' and pwd='"+passwordCrypted+"';";
        //To append the previous sql codes
        sqlResetPwd.append(sqlUpdatePwdState);
        //Execution of the sql code on the database
        mdatabaseVishnu->process( sqlResetPwd.c_str());
        //to get the email adress of the user
        std::string email = getAttribut("where userid='"+user.getUserId()+"'", "email");
        user.setEmail(email);
        //Send email
        std::string emailBody = getMailContent(user, false);
        sendMailToUser(user, emailBody, "Vishnu message: password reset", sendmailScriptPath);
      } // End if the user whose password will be reset exists
      else {
        UMSVishnuException e (ERRCODE_UNKNOWN_USERID, "You must use a global VISHNU identifier");
        throw e;
      }
    } //END if the user is an admin
    else {
      UMSVishnuException e (ERRCODE_NO_ADMIN);
      throw e;
    }
  } //END if the user exists
  else {
    UMSVishnuException e (ERRCODE_UNKNOWN_USER);
    throw e;
  }
  return 0;
}//END: resetPassword(UMS_Data::User user)

/**
* \fn ~UserServer()
* \brief Destructor
*/
UserServer::~UserServer() {
}
/**
* \brief Function to get user information
* \fn UMS_Data::User getData()
* \return  The user data structure
*/
UMS_Data::User
UserServer::getData() {
  return muser;
}

/**
* \brief Function to initialize user data for constructor with sessionServer
* \fn init()
*/
void
UserServer::init(){
  std::string numUser;
  std::string sessionState;

  //if userId and password have not been defined
  if ((muser.getUserId().size() == 0) && (muser.getPassword().size() == 0)) {
    //To get the users_numuserid by using the sessionServer
    numUser =
    msessionServer->getAttribut("where"
    " sessionkey='"+msessionServer->getData().getSessionKey()+"'", "users_numuserid");

    //if the session key is found
    if (numUser.size() != 0) {
      //To get the session state
      sessionState =
      msessionServer->getAttribut("where"
      " sessionkey='"+msessionServer->getData().getSessionKey()+"'", "state");

      //if the session is active
      if (convertToInt(sessionState) == 1) {
        muser.setUserId(getAttribut("where numuserid='"+numUser+"'", "userid"));
        muser.setPassword(getAttribut("where numuserid='"+numUser+"'", "pwd"));
      } //End if the session is active
      else {
        UMSVishnuException e (ERRCODE_SESSIONKEY_EXPIRED);
        throw e;
      }

    } //END If the session key is found
    else {
      UMSVishnuException e (ERRCODE_SESSIONKEY_NOT_FOUND);
      throw e;
    }
  }//END If the userId and password have not been defined
} //END: void init()
/**
* \brief Function to check user on database
* \fn bool exist(bool flagForChangePwd)
* \param flagForChangePwd A flag to check the password state
* \return true if the password state has not to be checked else false
*/
bool
UserServer::exist(bool flagForChangePwd) {
  //if the user is on the database
  if (getAttribut("where userid='"+muser.getUserId()+"'and pwd='"+muser.getPassword()+"'").size() != 0) {
    CheckUserState(flagForChangePwd);
    return true;
  }//END if the user is on the database
  else {
    return false;
  }
} //END: exist(bool flagForChangePwd)

/**
* \brief Function to check user on database
* \return true if the user is authenticated else false
*/
bool
UserServer::isAuthenticate(bool flagForChangePwd) {

  bool existUser = false;
  AuthenticatorFactory authFactory;
  Authenticator *authenticatorInstance = authFactory.getAuthenticatorInstance();

  existUser = authenticatorInstance->authenticate(muser);
  if (existUser) {
    CheckUserState(flagForChangePwd);
  }
  return existUser;
}


/**
* \brief Function to check the VISHNU user privilege
* \fn bool isAdmin()
* \return true if the user is an admin else false
*/
bool
UserServer::isAdmin() {

    return (convertToInt (getAttribut("where userid='"+muser.getUserId()+"'and "
    "pwd='"+muser.getPassword()+"'", " privilege")) != 0);
}

/**
* \brief Function to check the user attribut value
* \fn bool isAttributOk(std::string attributName, int valueOk)
* \param attributName The name of the attribut to check
* \param valueOk the value which will be compare to attribut name value
* \return true if the attributName value is valueOk
*/
bool
UserServer::isAttributOk(std::string attributName, int valueOk) {
    return (convertToInt(getAttribut("where userid='"+muser.getUserId()+"'and "
    "pwd='"+muser.getPassword()+"'", attributName)) == valueOk);
}

/**
* \brief Function to get user information from the database vishnu
* \fn getAttribut(std::string condition, std::string attrname);
* \param condition The condition of the select request
* \param attrname the name of the attribut to get
* \return the value of the attribut or empty string if no results
*/
std::string UserServer::getAttribut(std::string condition, std::string attrname) {
  std::string sqlCommand("SELECT "+attrname+" FROM users "+condition);
  boost::scoped_ptr<DatabaseResult> result(mdatabaseVishnu->getResult(sqlCommand.c_str()));
  return result->getFirstElement();
}

/**
* \brief Function to check a userId
* \fn bool existuserId(std::string userId)
* \param userId The userId to check
* \return true if the userId exists
*/
bool
UserServer::existuserId(std::string userId) {

  //If the userID exists on the database
  if (getAttribut("where userid='"+userId+"'").size() != 0)	{
    //If the user is not locked
    if ( convertToInt(getAttribut("where userid='"+userId+"'", "status")) == 1) {
      return true;
    } //END if the user is not locked
    else {
      UMSVishnuException e (ERRCODE_USER_LOCKED);
      throw e;
    }
  }//END If the userID exists on the database
  else {
    return false;
  }
}

/**
* \brief Function to generate a password
* \fn generatePassword(std::string value1, std::string value2)
* \param value1 a string used to generate the password
* \param value2 a string used to generate the password
* \return an encrypted message
*/
std::string
UserServer::generatePassword(std::string value1, std::string value2) {

  std::string salt = "$1$"+value1 + convertToString(generateNumbers())+value2+"$";
  std::string clef = value2+convertToString(generateNumbers());

  return (std::string(crypt(clef.c_str(), salt.c_str())+salt.length()));
}
/**
* \brief Function to send an email to a user
* \param user     the user to whom send the email
* \param content  the body of the email
* \param subject  the subject of the email
* \param sendmailScriptPath The path to the script for sending emails
*/
int
UserServer::sendMailToUser(const UMS_Data::User& user,
                           std::string content,
                           std::string subject,
                           std::string sendmailScriptPath) {

  std::vector<std::string> tokens;
  std::ostringstream command;
  pid_t pid;

  std::string address = user.getEmail();
  //If the address is empty
  if (address.empty()) {
    throw UserException(ERRCODE_INVALID_MAIL_ADRESS, "Empty email address");
  }

  //If the script is empty
  if (sendmailScriptPath.empty()) {
    throw SystemException(ERRCODE_SYSTEM, "Invalid server configuration");
  }
  // To build the script command
  command << sendmailScriptPath << " --to " << address << " -s ";

  std::istringstream is(command.str());
  std::copy(std::istream_iterator<std::string>(is),
  std::istream_iterator<std::string>(),
  std::back_inserter<std::vector<std::string> >(tokens));

  char* argv[tokens.size()+6];
  argv[tokens.size()+5]=NULL;
  //Use of tokens
  for (unsigned int i = 0; i < tokens.size(); ++i) {
    argv[i]=strdup(tokens[i].c_str());
  }
  //To avoid mutiple values by using tokens because of spaces
  argv[tokens.size()]=strdup(subject.c_str());
  argv[tokens.size()+1]=strdup(content.c_str());
  //To execute the script on background
  argv[tokens.size()+2]=strdup(" 1>/dev/null ");
  argv[tokens.size()+3]=strdup(" 2>/dev/null ");
  argv[tokens.size()+4]=strdup(" & ");

  pid = fork();
  if (pid == -1) {//if an error occurs during fork
    for (unsigned int i=0; i<tokens.size()+5; ++i) {
      free(argv[i]);
    }
    throw SystemException(ERRCODE_SYSTEM, "Error during the creation of the process for sending mail to "
    +user.getFirstname()+ " with userId:" +user.getUserId());
  }

  if (pid == 0) {//if the child process
    freopen("dev/null", "r", stdin);
    freopen("dev/null", "w", stdout);
    freopen("dev/null", "w", stderr);

    if (execv(argv[0], argv) == -1) {
      for (unsigned int i=0; i<tokens.size()+5; ++i) {
      free(argv[i]);
      }
      exit(1);
    }
  }
  for (unsigned int i=0; i<tokens.size()+5; ++i) {
    free(argv[i]);
  }
  return 0;
}

/**
* \brief Function to get the email content
* \param user     the user who will receives the email
* \param flagAdduser a flag which means that it is uses on adduser function
* \return the email content
*/
std::string
UserServer::getMailContent(const UMS_Data::User& user, bool flagAdduser) {
  std::string content;
  std::stringstream newline;
  newline << std::endl;

  if (flagAdduser) {
    content.append("Dear "+user.getFirstname()+" "+user.getLastname()+ ",");
    content.append(newline.str());
    content.append("This is respectively your userId and your password generated by vishnu:");
    content.append(newline.str());
    content.append("userId:"+ user.getUserId()+",");
    content.append(newline.str());
    content.append("password:"+user.getPassword());
  }
  else {
    content.append("Dear "+user.getUserId()+",");
    content.append(newline.str());
    content.append("This is your new password: "+user.getPassword());
  }
  return content;
}

/**
* \brief Function to get the user account login
* \param machineId The machine identifier of machine on which the user have a account
* \return the user account login
*/
std::string
UserServer::getUserAccountLogin(const std::string& machineId) {

  init();

  std::string userId = getData().getUserId();
  UMS_Data::LocalAccount_ptr account = new UMS_Data::LocalAccount();
  account->setMachineId(machineId);
  account->setUserId(userId);
  LocalAccountServer localAccount(account, *msessionServer);

  UMS_Data::Machine_ptr machine = new UMS_Data::Machine();
  machine->setMachineId(machineId);
  MachineServer machineServer(machine);

  //To get the database number id of the machine
  std::string numMachine = machineServer.getAttribut("where machineid='"+localAccount.getData()->getMachineId()+"'");
  //To get the database number id of the user
  std::string numUser = getAttribut("where userid='"+localAccount.getData()->getUserId()+"'");

  std::string acLogin;
  if ((numMachine.size() > 0) && (numUser.size() > 0)) {
    acLogin = localAccount.getAttribut("where machine_nummachineid="+numMachine+" and users_numuserid="+numUser, "aclogin");
  }

  if(acLogin.size()==0) {
    delete account;
    delete machine;
    throw UMSVishnuException(ERRCODE_UNKNOWN_LOCAL_ACCOUNT, "You have not a local account on this machine");
  }

  delete account;
  delete machine;
  return acLogin;
}

/**
* \brief Function to check the user state (locked or not) and to
* verify the password state
*/
void
UserServer::CheckUserState(bool flagForChangePwd) {
  //If the user is not locked
  if (isAttributOk("status", 1)) {
    //if the flag to check the password state is set
    if (!flagForChangePwd) {
      //If the passwordstate is not active
      if (!isAttributOk("passwordstate", 1)) {
        UMSVishnuException e (ERRCODE_TEMPORARY_PASSWORD);
        throw e;
      }//END If the passwordstate is not active
    }//END the flag to check the password state is set
  } //END if the user is not locked
  else {
    UMSVishnuException e (ERRCODE_USER_LOCKED);
    throw e;
  }
}

/**
* \brief Function to get a certain user account property
* \param machineId The machine identifier of machine on which the user have a account
* \param property The property name
* \return the user account login
*/
std::string
UserServer::getUserAccountProperty(const std::string& machineId, const std::string& property) {

  init();

  std::string userId = getData().getUserId();
  UMS_Data::LocalAccount_ptr account = new UMS_Data::LocalAccount();
  account->setMachineId(machineId);
  account->setUserId(userId);
  LocalAccountServer localAccount(account, *msessionServer);

  UMS_Data::Machine_ptr machine = new UMS_Data::Machine();
  machine->setMachineId(machineId);
  MachineServer machineServer(machine);

  //To get the database number id of the machine
  std::string numMachine = machineServer.getAttribut("where machineid='"+localAccount.getData()->getMachineId()+"'");
  //To get the database number id of the user
  std::string numUser = getAttribut("where userid='"+localAccount.getData()->getUserId()+"'");

  std::string value;
  if ((numMachine.size() > 0) && (numUser.size() > 0)) {
	  value = localAccount.getAttribut("where machine_nummachineid="+numMachine+" and users_numuserid="+numUser, property);
  }

  if(value.size()==0) {
    delete account;
    delete machine;
    throw UMSVishnuException(ERRCODE_UNKNOWN_LOCAL_ACCOUNT, "You have not a local account on this machine");
  }

  delete account;
  delete machine;
  return value;
}
