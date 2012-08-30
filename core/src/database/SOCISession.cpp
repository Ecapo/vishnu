/**
 * \file SOCISession.cpp
 * \brief This file implements the SOCI session.
 * \author Jean-Baptiste Ghoul (jean-baptiste.ghoul@sysfera.com)
 * \date 03/07/12
 */

#include "SOCISession.hpp"

using namespace std;
using namespace soci;



/*
 * class SOCISession
 */

/*
 * \brief default constructor
 */
SOCISession::SOCISession()
	: msession(NULL),autoCommit(true)
{}
/*
 * \brief copy constructor
 */
SOCISession::SOCISession(const SOCISession & s)
	: msession(s.msession),pool_position(s.pool_position),autoCommit(s.autoCommit)
{

}
/*
 * \brief constructor with existing soci session
 */
SOCISession::SOCISession(soci::session* asession, size_t pos, bool isAutoCommit)
	: msession(asession),pool_position(pos),autoCommit(isAutoCommit)
{}

SOCISession::~SOCISession()
{

}



/*
 * public functions
 */

SOCITemporaryType
SOCISession::execute(std::string const & query)
{
	if(msession==NULL) {
		throw SystemException(ERRCODE_DBERR,"Cannot execute : session is null");
	}
	std::string request=query;
	// SOCI for ORACLE does not support semi-colom
	while (msession->get_backend_name()=="oracle" && query[query.length()-1] == ';') {
		request.erase(request.length()-1,1);
	}

	if (autoCommit) {
		begin();
	}
	SOCITemporaryType ret(*this);
	TRYCATCH( ret.once<<request, "")
	return ret;
}

SOCITemporaryType
SOCISession::operator<<(std::string const & query)
{
	return execute(query);
}




void
SOCISession::begin()
{
	if(msession==NULL) {
		throw SystemException(ERRCODE_DBERR,"Cannot begin : session is null");
	}
	TRYCATCH(msession->begin(),"Cannot begin transaction \n");
}
void
SOCISession::commit()
{
	if(msession==NULL) {
		throw SystemException(ERRCODE_DBERR,"Cannot commit : session is null");
	}
	try	{
		msession->commit();
	}
	catch (exception & e)	{
		throw SystemException(ERRCODE_DBERR,
				std::string("Cannot commit transaction : \n") + e.what());
	}
}
void
SOCISession::rollback()
{
	if(msession==NULL) {
		throw SystemException(ERRCODE_DBERR,"Cannot rollback : session is null");
	}
	try
	{
		msession->rollback();
	}
	catch (exception & e)
	{
		throw SystemException(ERRCODE_DBERR,
				std::string("Cannot commit transaction : \n") + e.what());
	}
}

SOCIStatement
SOCISession::getStatement()
{
	if(msession==NULL) {
		throw SystemException(ERRCODE_DBERR,"Cannot get statement : session is null");
	}
	return SOCIStatement(*this);
}

bool
SOCISession::got_data()
{
	if(msession==NULL) {
		throw SystemException(ERRCODE_DBERR,"session is null");
	}
	bool ret;
	TRYCATCH(ret=msession->got_data(),"")
	return ret;
}


soci::session &
SOCISession::getAdvanced()
{
	if(msession==NULL) {
		throw SystemException(ERRCODE_DBERR,"session is null");
	}
	return *msession;
}
/**
 * \brief get the pool position where the session is from
 */
size_t
SOCISession::getPoolPosition()
{
	if(msession==NULL) {
		throw SystemException(ERRCODE_DBERR,"session is null");
	}
	return pool_position;
}

bool
SOCISession::isAutoCommit() {
	return autoCommit;
}
