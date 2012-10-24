#include <boost/test/unit_test.hpp>

#include "core_testconfig.h"
#include "SOCIDatabase.hpp"


BOOST_AUTO_TEST_SUITE( soci_database_test )


using namespace std;
using namespace vishnu;

const std::string configFileDir(TESTCORECONFIGPATH);
const std::string configFilePath=configFileDir+"/soci/";

static SOCIDatabase * myDatabase=NULL;

BOOST_AUTO_TEST_CASE( initialisation )
{
	//TODO : this should be in a Fixture class

	BOOST_TEST_MESSAGE("===== SOCIDatabase unit tests (3) : requests ===== ");
	ExecConfiguration exConfig;
	std::string configFile=configFilePath+"soci-test_config.cfg";
	try{
		exConfig.initFromFile(configFile);
		DbConfiguration myConfig(exConfig);
		myConfig.check();
		myDatabase = new SOCIDatabase(myConfig);
	} catch( exception const & e)
	{
		BOOST_TEST_MESSAGE(e.what());
		BOOST_CHECK(false);
	}
	BOOST_REQUIRE(myDatabase!=NULL);
	BOOST_CHECK(myDatabase->connect()==0);
	BOOST_CHECK(myDatabase->disconnect()==0);
	BOOST_TEST_MESSAGE("Normal connection to Database -- ok");

	BOOST_CHECK_THROW(myDatabase->disconnect(),VishnuException);
}

BOOST_AUTO_TEST_CASE( processing_request_without_transaction )
{
	BOOST_REQUIRE(myDatabase != NULL);

	BOOST_TEST_MESSAGE("--- process request on disconnect database ---");
	// processing with disconnected database
	BOOST_CHECK_THROW(myDatabase->process("create table notab(id int)"),VishnuException);
	// connection to database
	BOOST_REQUIRE(myDatabase->connect()==0);
	// empty request
	BOOST_TEST_MESSAGE("--- process empty request ---");
	BOOST_CHECK_THROW(myDatabase->process(""),VishnuException);
	// bad SQL Syntax
	BOOST_TEST_MESSAGE("--- process request with bad SQL syntax --");
	BOOST_CHECK_THROW(myDatabase->process("this is bad syntax"),VishnuException);
	// SQL exception
	BOOST_TEST_MESSAGE("--- process request with SQL error ---");
	BOOST_CHECK_THROW(myDatabase->process("drop table this_table_should_not_exists"),VishnuException);

	// SQL request without semicolon
	BOOST_TEST_MESSAGE("--- process request without semicolon ---");
	BOOST_CHECK(myDatabase->process("drop table if exists paco")==0);
	// SQL request with semicolon
	BOOST_TEST_MESSAGE("--- process request with semicolon ---");
	BOOST_CHECK(myDatabase->process("create table paco(id int,name varchar(255));")==0);
	// multiple SQL request separated by semicolons
	BOOST_TEST_MESSAGE("--- process multiple requests ---");
	std::string requests;
	std::string requests2;
	requests+="insert into paco(id,name) values('1','lloris');";
	requests+="insert into paco(id,name) values('3','cris')";
	BOOST_CHECK(myDatabase->process(requests)==0);
	requests2+="insert into paco(id) values('5')";
	requests2+=";";
	requests2+="insert into paco(name) values('kallstrom');";
	BOOST_CHECK(myDatabase->process(requests2)==0);

	// disconnect from database
	BOOST_CHECK(myDatabase->disconnect()==0);

}

BOOST_AUTO_TEST_CASE( processing_request_with_transaction )
{
	BOOST_REQUIRE(myDatabase != NULL);

	// connection to database
	BOOST_REQUIRE(myDatabase->connect()==0);
	BOOST_CHECK(myDatabase->startTransaction()==0);
	// process request in transaction
	BOOST_TEST_MESSAGE("--- process request in valid transaction ---");
	BOOST_CHECK(myDatabase->process("insert into paco(id,name) values('9','lisandro')",0)==0);
	BOOST_CHECK_NO_THROW(myDatabase->endTransaction(0));
	// process request in invalid transaction (out of connection pool size)
	BOOST_TEST_MESSAGE("--- process request in invalid transaction");
	BOOST_CHECK_THROW(myDatabase->process("insert into paco(id,name) values('7','ederson')",147),
			VishnuException);
	// process request with a required connection pool position
	BOOST_TEST_MESSAGE("--- process request in inactive transaction ---");
	BOOST_CHECK_NO_THROW(myDatabase->process("insert into paco(id,name) values (null,null);",1));

	// disconnect from database
	BOOST_CHECK(myDatabase->disconnect()==0);

}

BOOST_AUTO_TEST_CASE( getting_results_without_transaction )
{
	BOOST_REQUIRE(myDatabase != NULL);
	// getting result with disconnected database
	BOOST_TEST_MESSAGE("--- get result with disconnected database ---");
	BOOST_CHECK_THROW(myDatabase->getResult("select * from paco"),VishnuException);
	// connection to database
	BOOST_REQUIRE(myDatabase->connect()==0);
	// empty request
	BOOST_TEST_MESSAGE("--- get result of empty request");
	BOOST_CHECK_THROW(myDatabase->getResult(""),VishnuException);
	// bad SQL Syntax
	BOOST_TEST_MESSAGE("--- get result of bad SQL syntax");
	BOOST_CHECK_THROW(myDatabase->getResult("this is bad syntax"),VishnuException);
	// SQL exception
	BOOST_TEST_MESSAGE("--- get result of SQL error ---");
	BOOST_CHECK_THROW(myDatabase->getResult("select * from unknowtable"),VishnuException);
	BOOST_CHECK(myDatabase->getResult("select * from paco")->getNbTuples()==6);
	// disconnect from database
	BOOST_CHECK(myDatabase->disconnect()==0);

}

BOOST_AUTO_TEST_CASE( getting_results_with_transaction )
{
	BOOST_REQUIRE(myDatabase != NULL);
	// connection to database
	BOOST_REQUIRE(myDatabase->connect()==0);
	// start transaction
	BOOST_CHECK(myDatabase->startTransaction()==0);
	BOOST_TEST_MESSAGE("--- get result in valid transaction ---");
	BOOST_CHECK(myDatabase->getResult("select id from paco",0)->getNbTuples()==6);
	// transaction 147 out of connection pool size
	BOOST_TEST_MESSAGE("--- get result in invalid transaction ---");
	BOOST_CHECK_THROW(myDatabase->getResult("select * from paco",147),VishnuException);
	BOOST_CHECK_NO_THROW(myDatabase->getResult("select * from paco",0));
	// end transaction
	BOOST_CHECK_NO_THROW(myDatabase->endTransaction(0));
	// with a required connection pool position
	BOOST_TEST_MESSAGE("--- get result in inactive transaction ---");
	BOOST_CHECK_NO_THROW(myDatabase->getResult("select * from paco",1));
	// disconnect from database
	BOOST_CHECK(myDatabase->disconnect()==0);
}

BOOST_AUTO_TEST_CASE( execute_without_transaction )
{
	BOOST_REQUIRE(myDatabase != NULL);
	// getting result with disconnected database
	BOOST_TEST_MESSAGE("--- execute with disconnected database ---");
	BOOST_CHECK_THROW(myDatabase->execute("select * from paco"),VishnuException);
	// connection to database
	BOOST_REQUIRE(myDatabase->connect()==0);
	BOOST_TEST_MESSAGE("--- execute simple request");
	int count=0;
	BOOST_CHECK_NO_THROW(myDatabase->execute("select count(*) from paco").into(count));
	BOOST_CHECK(count>0);
	// empty request
	BOOST_TEST_MESSAGE("--- execute empty request");
	BOOST_CHECK_THROW(myDatabase->execute(""),VishnuException);
	// bad SQL Syntax
	BOOST_TEST_MESSAGE("--- execute bad SQL syntax");
	BOOST_CHECK_THROW(myDatabase->execute("this is bad syntax"),VishnuException);
	// SQL exception
	BOOST_TEST_MESSAGE("--- execute SQL error ---");
	BOOST_CHECK_THROW(myDatabase->execute("select * from unknowtable"),VishnuException);
	// disconnect from database
	BOOST_CHECK(myDatabase->disconnect()==0);

}

BOOST_AUTO_TEST_CASE( execute_with_transaction )
{
	BOOST_REQUIRE(myDatabase != NULL);
	// connection to database
	BOOST_REQUIRE(myDatabase->connect()==0);
	// start transaction
	BOOST_CHECK(myDatabase->startTransaction()==0);
	BOOST_TEST_MESSAGE("--- execute in valid transaction ---");
	BOOST_CHECK_NO_THROW(myDatabase->execute("select id from paco",0));
	// transaction 147 out of connection pool size
	BOOST_TEST_MESSAGE("--- execute in invalid transaction ---");
	BOOST_CHECK_THROW(myDatabase->execute("select * from paco",147),VishnuException);
	BOOST_CHECK_NO_THROW(myDatabase->execute("select * from paco",0));
	// end transaction
	BOOST_CHECK_NO_THROW(myDatabase->endTransaction(0));
	// with a required connection pool position
	BOOST_TEST_MESSAGE("--- execute in inactive transaction ---");
	BOOST_CHECK_NO_THROW(myDatabase->execute("select * from paco",1));
	// disconnect from database
	BOOST_CHECK(myDatabase->disconnect()==0);
}



BOOST_AUTO_TEST_CASE( clean_test )
{
	BOOST_REQUIRE(myDatabase != NULL);
	BOOST_CHECK(myDatabase->connect()==0);
	BOOST_CHECK(myDatabase->process("drop table if exists paco")==0);
	BOOST_CHECK(myDatabase->disconnect()==0);
}

BOOST_AUTO_TEST_CASE( finalize_test )
{
	// TODO this should be in a Fixture class

	try {
		if(myDatabase != NULL){
			delete myDatabase;
		}
		BOOST_CHECK(true);
	} catch (exception const & e)
	{
		BOOST_MESSAGE(e.what());
		BOOST_REQUIRE(false);
	}


}

BOOST_AUTO_TEST_SUITE_END()