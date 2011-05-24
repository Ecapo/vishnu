#include "ShellExporter.hpp"
#include "utilServer.hpp"
#include "IMSVishnuException.hpp"
#include "Mapper.hpp"
#include "UMSShellMapper.hpp"
//#include "FMSShellMapper.hpp"
#include "TMSShellMapper.hpp"
#include "IMSShellMapper.hpp"

ShellExporter::ShellExporter(UserServer u, IMS_Data::ExportOp_ptr op):ExportServer(u, *op) {
  UMSShellMapper *mmapperUMS;
  TMSShellMapper *mmapperTMS;
  //  FMSShellMapper *mmapperFMS;
  IMSShellMapper *mmapperIMS;
  /*Mapper registration*/
  mmapperTMS = new TMSShellMapper(MapperRegistry::getInstance(), TMSSHELLMAPPERNAME);
  mmapperTMS->registerMapper();
  mmapperIMS = new IMSShellMapper(MapperRegistry::getInstance(), IMSSHELLMAPPERNAME);
  mmapperIMS->registerMapper();
  //  mmapperFMS = new FMSShellMapper(MapperRegistry::getInstance(), FMSSHELLMAPPERNAME);
  //  mmapperFMS->registerMapper();
  mmapperUMS = new UMSShellMapper(MapperRegistry::getInstance(), UMSSHELLMAPPERNAME);
  mmapperUMS->registerMapper();
}

ShellExporter::~ShellExporter() {
}

int
ShellExporter::exporte(string oldSession, string &content){
  bool authorized = true;
  vector<string>::iterator iter;
  vector<string> line;
  string desc;

  // Init the script
  content = "#!/bin/sh";
  content += " \n";

  // Check the user is alloed to export
  if (!muser.isAdmin() && !isAllowed(oldSession, muser)){
    throw UserException(ERRCODE_INVEXPORT, "The user is not allowed to export this session");
  }

  // Check the session is closed
  if (!isClosed(oldSession)) {
    throw IMSVishnuException(ERRCODE_INVEXPORT, "The session id is invalid");
  }

  // The request, ordered by starttime (=submission)
  string req = "SELECT command.ctype, command.description, command.starttime from "
    " command, vsession where vsession.numsessionid=command.vsession_numsessionid and "
    " vsession.vsessionid='"+oldSession+"' order by starttime asc";

  boost::scoped_ptr<DatabaseResult> result (mdatabase->getResult(req.c_str()));

  // Adding all the results to the content
  for (size_t i = 0 ; i<result->getNbTuples(); i++) {
    line.clear();
    line = result->get(i);
    iter = line.begin();
    //MAPPER CREATION
    try {
      CmdType type = static_cast<CmdType>(convertToInt(*iter));
      Mapper* mapper = MapperRegistry::getInstance()->getMapper(getMapperName(type));
    content += mapper->decode(*(++iter));
    } catch (SystemException &e) {
      cout << "iter: " << *iter << endl;
      throw (e);
    }
    content += " \n";
  }

  return 0;
}

bool
ShellExporter::isClosed(string sid) {
  bool res = false;
  if (sid.size() < 1)
    return res;
  string req = "select * from vsession where vsessionid ='"+sid+"' and state='0'";
  try {
    boost::scoped_ptr<DatabaseResult> result(mdatabase->getResult(req.c_str()));
    res = (result->getNbTuples()>0);
  } catch(SystemException &e) {
    return res;
  }
  return res;
}

bool
ShellExporter::isAllowed(string oldSession, UserServer muser) {
  bool res = false;
  string req = "select * from vsession, users where vsession.vsessionid='"+oldSession+"' and vsession.users_numuserid=users.numuserid and users.userid='"+muser.getData().getUserId()+"'";
  try {
    boost::scoped_ptr<DatabaseResult> result(mdatabase->getResult(req.c_str()));
    res = (result->getNbTuples()>0);
  } catch(SystemException &e) {
    return res;
  }
  return res;
}

string
ShellExporter::getMapperName(int type) {
  switch (type) {
  case UMS:
    return UMSSHELLMAPPERNAME;
    break;
  case TMS:
    return TMSSHELLMAPPERNAME;
    break;
  case FMS:
    return FMSSHELLMAPPERNAME;
    break;
  case IMS:
    return IMSSHELLMAPPERNAME;
    break;
  default:
    throw IMSVishnuException(ERRCODE_INVEXPORT, "Cannot find mapper");
    break;
  }
}
