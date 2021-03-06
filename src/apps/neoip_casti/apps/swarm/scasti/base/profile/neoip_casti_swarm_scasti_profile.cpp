/*! \file
    \brief Definition of the \ref casti_swarm_scasti_profile_t

*/


/* system include */
/* local include */
#include "neoip_casti_swarm_scasti_profile.hpp"

NEOIP_NAMESPACE_BEGIN;

// definition of \ref casti_swarm_scasti_profile_t constant
const rate_estim_arg_t	casti_swarm_scasti_profile_t::RATE_ESTIM_ARG	= rate_estim_arg_t()
							.total_delay(delay_t::from_sec(20))
							.slice_delay(delay_t::from_sec(2));
// end of constants definition

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//                              ctor/dtor
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

/** \brief Constructor
 */
casti_swarm_scasti_profile_t::casti_swarm_scasti_profile_t()	throw()
{
	rate_estim_arg	(RATE_ESTIM_ARG);
}

/** \brief Destructor
 */
casti_swarm_scasti_profile_t::~casti_swarm_scasti_profile_t()	throw()
{
}


//////////////////////////////////////////////////////////////////////////////	
//////////////////////////////////////////////////////////////////////////////	
//                     check function
//////////////////////////////////////////////////////////////////////////////	
//////////////////////////////////////////////////////////////////////////////	

/** \brief Check the validity of the profile
 */
bt_err_t	casti_swarm_scasti_profile_t::check()	const throw()
{
	// return no error
	return bt_err_t::OK;
}

NEOIP_NAMESPACE_END

