/*! \file
    \brief Definition of the \ref serial2_stat_t

*/


/* system include */
#include <sstream>
/* local include */
#include "neoip_serial2_stat.hpp"

NEOIP_NAMESPACE_BEGIN;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//                         ctor/dtor
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/** \brief Default constructor
 */
serial2_stat_t::serial2_stat_t()	throw()
{
	// zero some field
	m_len_copied	= 0;
	m_obj_copied	= 0;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//                        arithmetic operator
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/** \brief the += operator
 */
serial2_stat_t &	serial2_stat_t::operator +=(const serial2_stat_t &other)	throw()
{
	// sum every fields
	m_len_copied	+= other.m_len_copied;
	m_obj_copied	+= other.m_obj_copied;
	// return the object itself
	return *this;
}


//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//                     main compare function
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

/** \brief main compare functions
 */
int serial2_stat_t::compare(const serial2_stat_t &other)	const throw()
{
	// compare the len_copied
	if( len_copied() < other.len_copied() )		return -1;
	if( len_copied() > other.len_copied() )		return +1;
	// NOTE: here both len_copied are equal

	// compare the obj_copied
	if( obj_copied() < other.obj_copied() )		return -1;
	if( obj_copied() > other.obj_copied() )		return +1;
	// NOTE: here both obj_copied are equal

	// here both are considered equal
	return 0;
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//                        display function
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

/** \brief convert the object to a string
 */
std::string	serial2_stat_t::to_string()				const throw()
{
	std::ostringstream	oss;
	// build the string to return
	oss        << "len_copied="	<< len_copied();
	oss << " " << "obj_copied="	<< obj_copied();	
	// return the just built string
	return oss.str();
}


NEOIP_NAMESPACE_END
