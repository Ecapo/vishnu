# Database initialization for UMS tests
configure_file( UMSinitTest.sql.in "${TEST_SQL_PATH}/UMSinitTest.sql")
configure_file( UMSinitAuthSystem.sql.in "${TEST_SQL_PATH}/UMSinitAuthSystem.sql")
configure_file( UMSinitWebboardData.sql.in "${TEST_SQL_PATH}/UMSinitWebboardData.sql")
configure_file( cleanall.sql "${TEST_SQL_PATH}/cleanall.sql" COPYONLY)
configure_file( clean_session.sql "${TEST_SQL_PATH}/clean_session.sql" COPYONLY)
configure_file( cleanModelWithWebboard.sql "${TEST_SQL_PATH}/cleanModelWithWebboard.sql" COPYONLY)


# TODO move to another directory
configure_file(id_rsa.pub "${TEST_SQL_PATH}/id_rsa.pub" COPYONLY)