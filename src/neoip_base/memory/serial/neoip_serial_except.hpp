/*! \file
    \brief Header of the \ref serial_except_t

- TODO to rename neoip_except into base_except_t

*/


#ifndef __NEOIP_SERIAL_EXCEPT_HPP__
#define __NEOIP_SERIAL_EXCEPT_HPP__
/* system include */
/* local include */
#include "neoip_except.hpp"
#include "neoip_copy_ctor_checker.hpp"
#include "neoip_namespace.hpp"

NEOIP_NAMESPACE_BEGIN;

// forward declaration for convenience
class	serial_t;

/** \brief Definition for the serial_except_t
 */
class serial_except_t : NEOIP_COPY_CTOR_ALLOW, public neoip_except {
public:	
	serial_except_t(const std::string &reason)	: neoip_except(reason){}
	serial_except_t(const std::string &reason, const std::string &filename, int line
			, const std::string &fctname)	: neoip_except(reason, filename, line, fctname ){}
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//                  bunch of #define for convenience
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define serial_except_l(reason)	serial_except_t(reason, __FILE__, __LINE__, __func__)
#define nthrow_serial_errno(str)	NTHROW serial_except_l(std::string(str) \
							+ ". errno=" + neoip_strerror(errno) )
#define nthrow_serial_plain(str)	NTHROW serial_except_l(std::string(str))


NEOIP_NAMESPACE_END


#endif	/* __NEOIP_SERIAL_EXCEPT_HPP__  */









