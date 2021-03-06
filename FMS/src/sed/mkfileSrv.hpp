
/**
 * \file mkfileSrv.hpp
 * This file contains the declaration of the vishnu mkfile server
 */
#ifndef MKFILESRV_HPP
#define MKFILESRV_HPP

#include "DIET_server.h"

/**
 * \brief The mkfile service profile description
 * \return the mkfile service profile
 */
diet_profile_desc_t* getCreateFileProfile();
/**
 * \brief the mkfile solve function
 * \param profile the service profile
 * \return 0 if the service succeds or an error code otherwise
 */
int solveCreateFile(diet_profile_t* profile);

#endif
