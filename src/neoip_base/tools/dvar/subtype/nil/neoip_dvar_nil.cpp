/*! \file
    \brief Declaration of the dvar_nil_t
*/

/* system include */
/* local include */
#include "neoip_dvar_nil.hpp"
#include "neoip_log.hpp"
#include "neoip_nipmem_alloc.hpp"

NEOIP_NAMESPACE_BEGIN

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//                       ctor/dtor
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

/** \brief clone this object
 */
dvar_vapi_t *	dvar_nil_t::clone()	const throw()
{
	return nipmem_new dvar_nil_t();
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//                      compare() function
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/** \brief Compare the object ala memcmp
 */
int 	dvar_nil_t::compare(const dvar_vapi_t &other_vapi)	const throw()
{
	// here both are considered equal
	return 0;
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//                         display function
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/** \brief Convert the object to a std::string
 */
std::string	dvar_nil_t::to_string()	const throw()
{
	std::ostringstream	oss;
	// put the value
	oss << "NIL";
	// return the just built string
	return oss.str();
}

NEOIP_NAMESPACE_END







