
###############################################################################
# core_test_setup: macro that setup our test environment
# it add a test-xml target
###############################################################################

macro( core_test_setup )
  enable_testing()
  # add a test-xml targets that allows us to generate Boost.Test xml reports
  add_custom_target( core_test-xml )
  include(CTest)

  file( MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/reports )
  set( REPORT_OUTPUT_PATH ${PROJECT_BINARY_DIR}/reports )
endmacro()

###############################################################################
# core_test: macro that setup a test
# @param[in] NAME  test filename stripped from extension
# @param[in] DISABLED (optional) when defined, remove test from master suite
#                     and display a message to the user.
###############################################################################

macro( core_test NAME )
  if( ${ARGC} MATCHES 2 )
    if( ${ARGV1} MATCHES "DISABLED" )
      message( STATUS "${ARGV0}: explicitly disabled by developer" )
      set( ${ARGV0}-DISABLED ON )
    endif()
  endif()

  if( NOT DEFINED ${ARGV0}-DISABLED )
    # create unit tests executable
    add_executable( ${NAME}
      "${NAME}.cpp"
      #entry point
    TestRunner.cpp )
    include_directories( ${Boost_INCLUDE_DIRS}
      ${DATA_BASE_INCLUDE_DIR}
      ${CONFIG_SOURCE_DIR}
      ${VISHNU_EXCEPTION_INCLUDE_DIR}
      ${DATABASE_INCLUDE_DIR}
      ${CORE_DATA_DIR}
      ${Boost_INCLUDE_DIRS}
      ${PROJECT_BINARY_DIR}/include
      ${CORE_TEST_DIR}
      )

    # link libraries
    target_link_libraries( ${NAME}
      ${LIBPQ_LIB}
      ${Boost_LIBRARIES}
      vishnu-core
      vishnu-core-server
      crypt
      vishnu-test
      ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

    # test executable installation has not been tested yet -sic-
    # install( TARGETS ${NAME} DESTINATION bin )
    add_test( ${NAME} ${BIN_DIR}/${NAME} )

    # prevent Boost.Test to catch unrelated exceptions
    set_property( TEST ${NAME}
      PROPERTY ENVIRONMENT "BOOST_TEST_CATCH_SYSTEM_ERRORS=no;" )
    # just make sure that our test are run in a serial fashion
    set_property( TEST ${NAME} PROPERTY RUN_SERIAL ON )

    #
    add_custom_target( ${NAME}-xml
        COMMAND ${CMAKE_COMMAND}
        -DTEST_PROG=${NAME}
        -DBIN_PATH=${BIN_DIR}
        -DREPORT_PATH=${REPORT_OUTPUT_PATH}
        -P ${PROJECT_SOURCE_DIR}/Cmake/runtest.cmake )
      add_dependencies( core_test-xml ${NAME}-xml )
    endif()
endmacro()

###############################################################################
