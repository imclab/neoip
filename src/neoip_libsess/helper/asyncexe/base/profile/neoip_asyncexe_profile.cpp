/*! \file
    \brief Definition of the \ref asyncexe_profile_t

*/


/* system include */
/* local include */
#include "neoip_asyncexe_profile.hpp"

NEOIP_NAMESPACE_BEGIN;

// definition of \ref asyncexe_profile_t constant
const delay_t	asyncexe_profile_t::EXPIRE_DELAY	= delay_t(delay_t::INFINITE_VAL);

// end of constants definition
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//                              ctor/dtor
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

/** \brief Constructor
 */
asyncexe_profile_t::asyncexe_profile_t()	throw()
{
	expire_delay	(EXPIRE_DELAY);
}

/** \brief Destructor
 */
asyncexe_profile_t::~asyncexe_profile_t()	throw()
{
}


//////////////////////////////////////////////////////////////////////////////	
//////////////////////////////////////////////////////////////////////////////	
//                     check function
//////////////////////////////////////////////////////////////////////////////	
//////////////////////////////////////////////////////////////////////////////	

/** \brief Check the validity of the profile
 */
libsess_err_t	asyncexe_profile_t::check()	const throw()
{
	// return no error
	return libsess_err_t::OK;
}

NEOIP_NAMESPACE_END

