
#ifndef _SLURM_PROTOCOL_SOCKET_COMMON_H
#define _SLURM_PROTOCOL_SOCKET_COMMON_H

#if HAVE_CONFIG_H
#  include "config.h"
#  if HAVE_INTTYPES_H
#    include <inttypes.h>
#  else
#    if HAVE_STDINT_H
#      include <stdint.h>
#    endif
#  endif  /* HAVE_INTTYPES_H */
#else   /* !HAVE_CONFIG_H */
#  include <inttypes.h>
#endif  /*  HAVE_CONFIG_H */

#include <netinet/in.h>

#define AF_SLURM AF_INET
#define SLURM_INADDR_ANY 0x00000000

/* LINUX SPECIFIC */
/* this is the slurm equivalent of the operating system file descriptor,
 * which in linux is just an int */
typedef int32_t slurm_fd_t ;

/* this is the slurm equivalent of the BSD sockets sockaddr
 * also defined in slurm/slurm.h for users */
#ifndef __slurm_addr_t_defined
#  define  __slurm_addr_t_defined
   typedef struct sockaddr_in slurm_addr_t ;
#endif

/* this is the slurm equivalent of the BSD sockets fd_set */
typedef fd_set slurm_fd_set ;
typedef fd_set _slurm_fd_set ;

#endif
