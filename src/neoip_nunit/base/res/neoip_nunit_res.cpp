/*! \file
    \brief Definition of the nunit_res_t

*/

/* system include */
#include <sstream>
/* local include */
#include "neoip_nunit_res.hpp"


NEOIP_NAMESPACE_BEGIN;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//                     	ctor/dtor
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/** \brief Constructor
 */
nunit_res_t::nunit_res_t(const nunit_err_t &error, int line, const std::string &filename
					, const std::string &fctname)		throw()
{
	// copy some parameter
	error_val	= error;
	line_val	= line;
	filename_val	= filename;
	fctname_val	= fctname;
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//                      to_string() function
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/** \brief Convert the object into a string
 */
std::string	nunit_res_t::to_string()		const throw()
{
	std::ostringstream	oss;
	// build the string
	oss << error();
	// add the location IIF the error() is NOT nunit_err_t::OK
	if( error() != nunit_err_t::OK )
		oss << " (at " << filename() << ":" << line() << ":" << fctname() << "()))";
	// return the result
	return oss.str();
}



NEOIP_NAMESPACE_END

