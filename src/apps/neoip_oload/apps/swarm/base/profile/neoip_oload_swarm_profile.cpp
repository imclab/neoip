/*! \file
    \brief Definition of the \ref oload_swarm_profile_t

*/


/* system include */
/* local include */
#include "neoip_oload_swarm_profile.hpp"

NEOIP_NAMESPACE_BEGIN;

// definition of \ref oload_swarm_profile_t constant
const delay_t	oload_swarm_profile_t::IDLE_TIMEOUT		= delay_t::from_sec(60);
const size_t	oload_swarm_profile_t::SLIDE_CURS_MAXLEN	= 3*1024*1024;
// end of constants definition

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//                              ctor/dtor
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

/** \brief Constructor
 */
oload_swarm_profile_t::oload_swarm_profile_t()	throw()
{
	idle_timeout		(IDLE_TIMEOUT);
	slide_curs_maxlen	(SLIDE_CURS_MAXLEN);
}

/** \brief Destructor
 */
oload_swarm_profile_t::~oload_swarm_profile_t()	throw()
{
}


//////////////////////////////////////////////////////////////////////////////	
//////////////////////////////////////////////////////////////////////////////	
//                     check function
//////////////////////////////////////////////////////////////////////////////	
//////////////////////////////////////////////////////////////////////////////	

/** \brief Check the validity of the profile
 */
bt_err_t	oload_swarm_profile_t::check()	const throw()
{
	// return no error
	return bt_err_t::OK;
}

NEOIP_NAMESPACE_END

