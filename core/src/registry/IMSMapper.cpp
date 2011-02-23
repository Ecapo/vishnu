/**
 * \file IMSMapper.ccp
 * \brief Implementation of the IMS mapper
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date 10/02/11
 */

#include "IMSMapper.hpp"

IMSMapper::IMSMapper(){
};

IMSMapper::IMSMapper(MapperRegistry* reg, string na):Mapper(reg){
  mname = na;
  mmap.insert (pair<string, string>("1", "exportCommands"));
  mmap.insert (pair<string, string>("2", "getMetricCurrentValue"));
  mmap.insert (pair<string, string>("3", "getMetricHistory"));
  mmap.insert (pair<string, string>("4", "getProcesses"));
  mmap.insert (pair<string, string>("5", "setSystemInfo"));
  mmap.insert (pair<string, string>("6", "setSystemThreshold"));
  mmap.insert (pair<string, string>("7", "getSystemThreshold"));
  mmap.insert (pair<string, string>("8", "defineUserIdentifier"));
  mmap.insert (pair<string, string>("9", "defineMachineIdentifier"));
  mmap.insert (pair<string, string>("10", "defineJobIdentifier"));
  mmap.insert (pair<string, string>("11", "defineTransferIdentifier"));
  mmap.insert (pair<string, string>("12", "loadShed"));
  mmap.insert (pair<string, string>("13", "setUpdateFrequency"));
  mmap.insert (pair<string, string>("14", "getUpdateFrequency"));
};

int
IMSMapper::registerMapper(){
  mreg->addMapper(mname, this);
  return 0;
}

int
IMSMapper::unregisterMapper(){
  return mreg->removeMapper(mname);
}

int
IMSMapper::getCommand(const string& key,string& command){
  map<string, string>::const_iterator it;
  for (it = mmap.begin() ; it != mmap.end() ; it++){
    if (key.compare(it->first)==0){
      command = it->second;
      return 0;
    }
  }
}

int
IMSMapper::getKey(const string& command, string& key){
  map<string, string>::const_iterator it;
  for (it = mmap.begin() ; it != mmap.end() ; it++){
    if (command.compare(it->second)==0){
      key = it->first;
      return 0;
    }
  }
}
