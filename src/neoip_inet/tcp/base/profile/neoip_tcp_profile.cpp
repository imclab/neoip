/*! \file
    \brief Definition of the \ref tcp_profile_t

*/


/* system include */
/* local include */
#include "neoip_tcp_profile.hpp"

NEOIP_NAMESPACE_BEGIN;

// definition of \ref tcp_profile_t constant
// end of constants definition

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//                              ctor/dtor
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

/** \brief Constructor
 */
tcp_profile_t::tcp_profile_t()	throw()
{
}

//////////////////////////////////////////////////////////////////////////////	
//////////////////////////////////////////////////////////////////////////////	
//                     check function
//////////////////////////////////////////////////////////////////////////////	
//////////////////////////////////////////////////////////////////////////////	

/** \brief Check the validity of the profile
 */
inet_err_t	tcp_profile_t::check()	const throw()
{
	// return no error
	return inet_err_t::OK;
}

NEOIP_NAMESPACE_END
