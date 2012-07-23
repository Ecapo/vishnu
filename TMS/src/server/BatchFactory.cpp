/**
 * \file BatchFactory.cpp
 * \brief This file implements the batch factory
 * \author Daouda Traore (daouda.traore@sysfera.com)
 * \date April
 */

#include "BatchFactory.hpp"
#include "SharedLibrary.hh"

#include <boost/format.hpp>
#include <iostream>

static int created=0;

/**
 * \brief Constructor
 */
BatchFactory::BatchFactory() {
  mbatchServer = NULL;
}


BatchServer*
loadPluginBatch(const char *name) {
  dadi::SharedLibrary *plugin(NULL);
  void *factory(NULL);
  BatchServer *instance(NULL);

  std::string libname = boost::str(boost::format("%1%%2%%3%")
                                   % dadi::SharedLibrary::prefix()
                                   % name
                                   % dadi::SharedLibrary::suffix());

  plugin = new dadi::SharedLibrary(libname);
  if (plugin->isLoaded()) {
    factory = plugin->symbol("create_plugin_instance");
    ((int (*)(void **))(factory))((void**) &instance);
  }

}



/**
 * \brief Function to create a batchServer.
 * \param batchType The type of batchServer to create
 * \return an instance of BatchServer
 */
BatchServer*
BatchFactory::getBatchServerInstance(BatchType batchType) {
  BatchServer *instance(NULL);

  switch (batchType) {
    case TORQUE:
      instance = loadPluginBatch("vishnu-tms-torque");
      break;
    case LOADLEVELER:
      instance = loadPluginBatch("vishnu-tms-loadleveler");
      break;
    case SLURM:
      instance = loadPluginBatch("vishnu-tms-slurm");
      break;
    case LSF:
      instance = loadPluginBatch("vishnu-tms-lsf");
      break;
    case SGE:
      instance = loadPluginBatch("vishnu-tms-sge");
      break;
    default:
      break;
  }

  return static_cast<BatchServer *>(instance);
}

/**
 * \brief Function to delete a batchServer.
 */
void BatchFactory::deleteBatchServerInstance() {
   delete mbatchServer;
}
/**
 * \brief Destructor
 */
BatchFactory::~BatchFactory() {
}
