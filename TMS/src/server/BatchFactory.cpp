/**
 * \file BatchFactory.cpp
 * \brief This file implements the batch factory
 * \author Daouda Traore (daouda.traore@sysfera.com)
 * \date April
 */

#include "BatchFactory.hpp"
#include "SharedLibrary.hh"

extern "C" {
#include "drmaa.h"
}
#include <iostream>

static int created=0;

/**
 * \brief Constructor
 */
BatchFactory::BatchFactory() {
  mbatchServer = NULL;
}




/**
 * \brief Function to create a batchServer.
 * \param batchType The type of batchServer to create
 * \return an instance of BatchServer
 */
BatchServer*
BatchFactory::getBatchServerInstance(BatchType batchType) {
  void *factory(NULL);
  BatchServer *instance(NULL);
  dadi::SharedLibrary *plugin(NULL);

  switch (batchType) {
    case TORQUE:
      plugin = new dadi::SharedLibrary("tms-server-torque");
      if (plugin->isLoaded()) {
        factory = plugin->symbol("create_plugin_instance");
        ((int (*)(void **))(factory))((void**) &instance);
      }
      break;
    case LOADLEVELER:
      plugin = new dadi::SharedLibrary("tms-server-loadleveler");
      if (plugin->isLoaded()) {
        factory = plugin->symbol("create_plugin_instance");
        ((int (*)(void **))(factory))((void**) &instance);
      }
      break;
    case SLURM:
      plugin = new dadi::SharedLibrary("tms-server-slurm");
      if (plugin->isLoaded()) {
        factory = plugin->symbol("create_plugin_instance");
        ((int (*)(void **))(factory))((void**) &instance);
      }
      break;
    case LSF:
      plugin = new dadi::SharedLibrary("tms-server-lsf");
      if (plugin->isLoaded()) {
        factory = plugin->symbol("create_plugin_instance");
        ((int (*)(void **))(factory))((void**) &instance);
      }
      break;
    case SGE:
      plugin = new dadi::SharedLibrary("tms-server-sge");
      if (plugin->isLoaded()) {
        factory = plugin->symbol("create_plugin_instance");
        ((int (*)(void **))(factory))((void**) &instance);
      }
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
