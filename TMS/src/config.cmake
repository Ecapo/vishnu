set(TORQUESERVER "")
set(TORQUE_ALL_INCLUDE_DIR "")
set(TORQUE_ALL_LIB_DIR "")

set(LOADLEVELERSERVER "")
set(LOADLEVELER_ALL_INCLUDE_DIR "")
set(LOADLEVELER_ALL_LIB_DIR "")

set(SLURMSERVER "")
set(SLURM_ALL_INCLUDE_DIR "")
set(SLURM_ALL_LIB_DIR "")

set(LSFSERVER "")
set(LSF_ALL_INCLUDE_DIR "")
set(LSF_ALL_LIB_DIR "")

if (VISHNU_USE_TORQUE)
  set(TORQUESERVER server/TorqueServer.cpp utils_torque/pbs_sub.c)
  set(TORQUE_ALL_INCLUDE_DIR ${TORQUE_INCLUDE_DIR}  ${UTILS_TORQUE_DIR})
  set(TORQUE_ALL_LIB_DIR ${TORQUE_LIB})
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/utils_torque/pbs_server_config.h.in
    ${CMAKE_CURRENT_SOURCE_DIR}/utils_torque/pbs_server_config.h)
endif(VISHNU_USE_TORQUE)

if(VISHNU_USE_LOADLEVELER)
  set(LOADLEVELERSERVER server/LLServer.cpp)
  set(LOADLEVELER_ALL_INCLUDE_DIR ${LOADLEVELER_INCLUDE_DIR})
  set(LOADLEVELER_ALL_LIB_DIR ${LOADLEVELER_LIB})
endif(VISHNU_USE_LOADLEVELER) 

if (VISHNU_USE_SLURM)
  set(SLURMSERVER server/SlurmServer.cpp
    slurm_parser/opt.c
    slurm_parser/slurm_internal_api.c
    slurm_parser/uid.c
    slurm_parser/slurm_parser.c)
  set(SLURM_ALL_INCLUDE_DIR ${SLURM_INCLUDE_DIR}  ${SLURM_PARSER_DIR})
  set(SLURM_ALL_LIB_DIR ${SLURM_LIB})
endif(VISHNU_USE_SLURM)

if(VISHNU_USE_LSF)
  set(LSFSERVER server/LSFServer.cpp)
  set(LSF_ALL_INCLUDE_DIR ${LSF_INCLUDE_DIR})
  set(LSF_ALL_LIB_DIR ${LSF_LIB})
endif(VISHNU_USE_LSF) 

set(UsedBatchServer ${TORQUESERVER}  ${LOADLEVELERSERVER}  ${SLURMSERVER} ${LSFSERVER})
set(USED_BATCH_INCLUDE_DIR  ${TORQUE_ALL_INCLUDE_DIR}  ${LOADLEVELER_ALL_INCLUDE_DIR}  ${SLURM_ALL_INCLUDE_DIR} ${LSF_ALL_INCLUDE_DIR})
set(USED_BATCH_LIB ${TORQUE_ALL_LIB_DIR}  ${LOADLEVELER_ALL_LIB_DIR}  ${SLURM_ALL_LIB_DIR} ${LSF_ALL_LIB_DIR})
