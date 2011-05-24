#include <string>

#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

#include "SessionProxy.hpp"

#include "FileProxy.hh"
#include "LocalFileProxy.hh"
#include "RemoteFileProxy.hh"
#include "FileProxyFactory.hh"

using namespace std;


FileProxy* FileProxyFactory::getFileProxy(const SessionProxy& sessionProxy,const string& path,
                                 const string& user) {
  string distUser(user);
  if (FileProxy::extHost(path)=="localhost")
    return new LocalFileProxy(sessionProxy,path);

  if (user=="") {
    uid_t uid = getuid();
    struct passwd*  pw = getpwuid(uid);
    distUser = pw->pw_name;
  }
  return new RemoteFileProxy(sessionProxy,path, distUser);
}

