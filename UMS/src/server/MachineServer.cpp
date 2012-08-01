/* This file is a part of VISHNU.

* Copyright SysFera SAS (2011) 

* contact@sysfera.com

* This software is a computer program whose purpose is to provide 
* access to distributed computing resources.
*
* This software is governed by the CeCILL  license under French law and
* abiding by the rules of distribution of free software.  You can  use, 
* modify and/ or redistribute the software under the terms of the CeCILL
* license as circulated by CEA, CNRS and INRIA at the following URL
* "http://www.cecill.info". 

* As a counterpart to the access to the source code and  rights to copy,
* modify and redistribute granted by the license, users are provided only
* with a limited warranty  and the software's author,  the holder of the
* economic rights,  and the successive licensors  have only  limited
* liability. 
*
* In this respect, the user's attention is drawn to the risks associated
* with loading,  using,  modifying and/or developing or reproducing the
* software by the user in light of its specific status of free software,
* that may mean  that it is complicated to manipulate,  and  that  also
* therefore means  that it is reserved for developers  and  experienced
* professionals having in-depth computer knowledge. Users are therefore
* encouraged to load and test the software's suitability as regards their
* requirements in conditions enabling the security of their systems and/or 
* data to be ensured and,  more generally, to use and operate it in the 
* same conditions as regards security. 
*
* The fact that you are presently reading this means that you have had
* knowledge of the CeCILL license and that you accept its terms.
*/

/**
* \file MachineServer.cpp
* \brief This file implements the Class which manipulates VISHNU machine data on server side.
* \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
* \date 31/01/2011
*/

#include "MachineServer.hpp"
#include "DbFactory.hpp"
#include "utilVishnu.hpp"
#include "utilServer.hpp"

/**
* \brief Constructor
* \fn MachineServer(UMS_Data::Machine*& machine)
* \param machine The machine data structure
*/
MachineServer::MachineServer(UMS_Data::Machine*& machine):
mmachine(machine)
{
  DbFactory factory;
  mdatabaseVishnu = factory.getDatabaseInstance();
}

/**
* \brief Constructor
* \fn MachineServer(UMS_Data::Machine*& machine, SessionServer& session)
* \param machine The machine data structure
* \param session The object which encapsulates session data
*/
MachineServer::MachineServer(UMS_Data::Machine*& machine, SessionServer& session):
mmachine(machine), msessionServer(session)
{
  DbFactory factory;
  mdatabaseVishnu = factory.getDatabaseInstance();
}

/**
* \brief Function to add a new VISHNU machine
* \fn int add(int vishnuId)
* \param vishnuId The identifier of the vishnu instance
* \return raises an exception on error
*/
int
MachineServer::add(int vishnuId) {
  std::string sqlInsert = "insert into machine (vishnu_vishnuid, name, site, machineid, status, sshpublickey) values ";
  std::string idMachineGenerated;
  int machineCpt;
  std::string formatidmachine;

  UserServer userServer = UserServer(msessionServer);
  userServer.init();

  std::string vishnuid;

  //if the user exists
  if (userServer.exist()) {
    //if the user is an admin
    if (userServer.isAdmin()) {

      vishnuid = convertToString(vishnuId);
      machineCpt = convertToInt(getAttrVishnu("machinecpt", vishnuid));

      //To get the formatidmachine
      formatidmachine = getAttrVishnu("formatidmachine", vishnuid);
      //if the formatidmachine is defined
      if (formatidmachine.size() != 0) {
        //Generation of userid
        idMachineGenerated =
        getGeneratedName(formatidmachine.c_str(),
                          machineCpt,
                          MACHINE,
                          mmachine->getName());

        //if the machine id is generated
        if (idMachineGenerated.size() != 0) {
          incrementCpt("machinecpt", machineCpt);
          mmachine->setMachineId(idMachineGenerated);

          //if the machineId does not exist
          if (getAttribut("where machineid='"+mmachine->getMachineId()+"'").size() == 0) {

            mdatabaseVishnu->process(sqlInsert + "("+vishnuid+",'"+mmachine->getName()+"'\
            ,'"+ mmachine->getSite()+"','"+mmachine->getMachineId()+"',"+convertToString(mmachine->getStatus())+", \
            '"+mmachine->getSshPublicKey()+"')");

            //To insert the description of the machine
            mdatabaseVishnu->process("insert into description (machine_nummachineid, lang, \
            description) values \
            ("+getAttribut("where machineid='"+mmachine->getMachineId()+"'")+",\
            '"+ mmachine->getLanguage()+"','"+mmachine->getMachineDescription()+"')");

          } //if the machineId does not exist
          else {
            UMSVishnuException e (ERRCODE_MACHINE_EXISTING);
            throw e;
          }
        }//if the machine id is generated
        else {
          SystemException e (ERRCODE_SYSTEM, "There is a problem to parse the formatidmachine");
          throw e;
        }
      }//END if the formatidmachine is defined
      else {
        SystemException e (ERRCODE_SYSTEM, "The formatidmachine is not defined");
        throw e;
      }
    } //End if the user is an admin
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
} //END: add()

/**
* \brief Function to update a VISHNU machine
* \fn int update()
* \return raises an exception on error
*/
int
MachineServer::update() {

  std::string sqlCommand = "";



  UserServer userServer = UserServer(msessionServer);
  userServer.init();

  //if the user exists
  if (userServer.exist()) {
    //if the user is an admin
    if (userServer.isAdmin()) {

      //if the machine to update exists
      if (getAttribut("where machineid='"+mmachine->getMachineId()+"'").size() != 0) {

        //if a new machine name has been defined
        if (mmachine->getName().size() != 0) {
          sqlCommand.append("UPDATE machine SET name='"+mmachine->getName()+"'\
          where machineId='"+mmachine->getMachineId()+"';");
        }

        //if a new site has been defined
        if (mmachine->getSite().size() != 0) {
          sqlCommand.append("UPDATE machine SET site='"+mmachine->getSite()+"'\
          where machineId='"+mmachine->getMachineId()+"';");
        }

        //Set the status of the machine
        sqlCommand.append("UPDATE machine SET status="+convertToString(mmachine->getStatus())+
        " where machineId='"+mmachine->getMachineId()+"';");

        //if a new ssh public key has been defined
        if (mmachine->getSshPublicKey().size() != 0) {
          sqlCommand.append("UPDATE machine SET sshpublickey='"+mmachine->getSshPublicKey()+"'"
          " where machineId='"+mmachine->getMachineId()+"';");
        }

        //if a new language has been defined
        if (mmachine->getLanguage().size() != 0) {
          sqlCommand.append("UPDATE description SET lang='"+mmachine->getLanguage()+"'"
          " where machine_nummachineid='"+getAttribut("where machineid='"+mmachine->getMachineId()+"'")+"';");
        }

        //if a new machine description has been defined
        if (mmachine->getMachineDescription().size() != 0) {
          sqlCommand.append("UPDATE description SET description='"+mmachine->getMachineDescription()+"'"
          " where machine_nummachineid='"+getAttribut("where machineid='"+mmachine->getMachineId()+"'")+"';");
        }

        mdatabaseVishnu->process(sqlCommand.c_str());

      } //End if the machine to update exists
      else {
        UMSVishnuException e (ERRCODE_UNKNOWN_MACHINE);
        throw e;
      }

    } //End if the user is an admin
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
} //END: update()

/**
* \brief Function to delete a VISHNU machine
* \fn int deleteMachine()
* \return raises an exception on error
*/
int
MachineServer::deleteMachine() {

  UserServer userServer = UserServer(msessionServer);
  userServer.init();
  //if the user exists
  if (userServer.exist()) {
    //if the user is an admin
    if (userServer.isAdmin()) {
      //if the machine to update exists
      if (getAttribut("where machineid='"+mmachine->getMachineId()+"'").size() != 0) {
        mdatabaseVishnu->process("DELETE FROM machine where machineid='"+mmachine->getMachineId()+"'");
      } //End if the machine to update exists
      else {
        UMSVishnuException e (ERRCODE_UNKNOWN_MACHINE);
        throw e;
      }
    } //End if the user is an admin
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
} //END: deleteMachine()

/**
* \fn ~MachineServer()
* \brief Destructor
*/
MachineServer::~MachineServer() {
}

/**
* \brief Function to get machine information
* \fn UMS_Data::Machine* getData()
* \return  The user data structure
*/
UMS_Data::Machine*
MachineServer::getData() {
  return mmachine;
}

/**
* \brief Function to get machine information from the database vishnu
* \fn getAttribut(std::string condition, std::string attrname);
* \param condition The condition of the select request
* \param attrname the name of the attribut to get
* \return the value of the attribut or empty string if no results
*/

std::string
MachineServer::getAttribut(std::string condition, std::string attrname) {

  std::string sqlCommand("SELECT "+attrname+" FROM machine "+condition);
  boost::scoped_ptr<DatabaseResult> result(mdatabaseVishnu->getResult(sqlCommand.c_str()));
  return result->getFirstElement();
}

/**
* \brief Function to get the content of the public ssh key
* \fn std::string getPublicKey()
* \return The content of the ssh public key
*/
std::string
MachineServer::getPublicKey() {
  return msshpublickey;
}

/**
* \brief Function to get the machine 
* \return The name of the machine 
*/
std::string 
MachineServer::getMachineName() {

  std::string  machineName = getAttribut("where machineid='"+getData()->getMachineId()+"'", "name");

  return machineName;
}

/**
* \brief Function to check the machineId
* \return raises an exception 
*/
void MachineServer::checkMachine() {

  if(getAttribut("where machineid='"+mmachine->getMachineId()+"'").size()==0){
    throw UMSVishnuException(ERRCODE_UNKNOWN_MACHINE, mmachine->getMachineId()+" does not exist among the defined" 
                                                                 " machines by VISHNU System");
  }
  if(getAttribut("where status=1 and  machineid='"+mmachine->getMachineId()+"'").size() == 0) {
    throw UMSVishnuException(ERRCODE_MACHINE_LOCKED);
  }
}

