#include <boost/assign/list_inserter.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/assign/std/vector.hpp>
#include <boost/filesystem.hpp>
#include <boost/process/all.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/thread.hpp>
#include "tmsTestUtils.hpp"
using namespace std;
namespace bp = boost::process;

namespace ba = boost::assign;
namespace bf = boost::filesystem;
namespace bs = boost::system;

VishnuConnexion::VishnuConnexion(const string& uid, const string& upwd, const UMS_Data::ConnectOptions& co):muid(uid),mupwd(upwd),mco(co),open(false){

}

VishnuConnexion::~VishnuConnexion(){
  if(true==open){
  BOOST_REQUIRE_EQUAL(vishnu::close(msession.getSessionKey()),0);
  BOOST_TEST_MESSAGE("The session is closed");
  }
}

string VishnuConnexion::getConnexion(){

  BOOST_REQUIRE(vishnu::connect(muid,mupwd,msession,mco)==0);
  open=true;
   BOOST_TEST_MESSAGE("The session is open");
   return msession.getSessionKey();
}


bool operator== (const Job& lJob,const Job& rJob ){

  return ( (lJob.getJobId() == rJob.getJobId())
        && (lJob.getSubmitMachineId() == rJob.getSubmitMachineId())  
        && (lJob.getJobPath() == rJob.getJobPath()) 
        && (lJob.getJobName() == rJob.getJobName()) 
        && (lJob.getJobPrio() == rJob.getJobPrio())
        && (lJob.getOwner() == rJob.getOwner() ) 
        && (lJob.getJobQueue() == rJob.getJobQueue() ) 
        && (lJob.getWallClockLimit() == rJob.getWallClockLimit() ) 
     
     
      );



}




  
