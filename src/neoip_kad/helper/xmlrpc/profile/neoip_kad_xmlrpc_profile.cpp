/*! \file
    \brief Definition of the \ref kad_xmlrpc_profile_t

*/


/* system include */
/* local include */
#include "neoip_kad_xmlrpc_profile.hpp"

NEOIP_NAMESPACE_BEGIN;

// definition of \ref kad_xmlrpc_profile_t constant
const delay_t	kad_xmlrpc_profile_t::SESSION_IDLE_TIMEOUT	= delay_t::from_sec(2*60);
// end of constants definition

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//                              ctor/dtor
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

/** \brief Constructor
 */
kad_xmlrpc_profile_t::kad_xmlrpc_profile_t()	throw()
{
	session_idle_timeout(SESSION_IDLE_TIMEOUT);
}

//////////////////////////////////////////////////////////////////////////////	
//////////////////////////////////////////////////////////////////////////////	
//                     check function
//////////////////////////////////////////////////////////////////////////////	
//////////////////////////////////////////////////////////////////////////////	

/** \brief Check the validity of the profile
 */
kad_err_t	kad_xmlrpc_profile_t::check()	const throw()
{
	// return no error
	return kad_err_t::OK;
}

NEOIP_NAMESPACE_END

