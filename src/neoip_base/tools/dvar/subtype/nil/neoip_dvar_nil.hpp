/*! \file
    \brief Declaration of the dvar_nil_t

*/


#ifndef __NEOIP_DVAR_NIL_HPP__
#define __NEOIP_DVAR_NIL_HPP__
/* system include */
/* local include */
#include "neoip_dvar_type.hpp"
#include "neoip_dvar_vapi.hpp"
#include "neoip_copy_ctor_checker.hpp"
#include "neoip_namespace.hpp"

NEOIP_NAMESPACE_BEGIN

/** \brief a dvar_t containing a dvar_type_t::NIL
 */
class dvar_nil_t : NEOIP_COPY_CTOR_ALLOW, public dvar_vapi_t {
private:
	/*************** clone() to copy via dvar_vapi_t	***************/
	dvar_vapi_t *	clone()	const throw();
public:
	/*************** ctor/dtor	***************************************/
	dvar_nil_t()	throw()	{}
	~dvar_nil_t()	throw()	{}

	/*************** Query function	***************************************/
	dvar_type_t	type()		const throw()	{ return dvar_type_t::NIL;	}

	/*************** Comparison function	*******************************/
	int	compare(const dvar_vapi_t &other_vapi)	const throw();
	bool 	operator == (const dvar_nil_t & other)	const throw()	{ return compare(other) == 0;	}
	bool 	operator != (const dvar_nil_t & other)	const throw()	{ return compare(other) != 0;	}
	bool 	operator <  (const dvar_nil_t & other)	const throw()	{ return compare(other) <  0;	}
	bool 	operator <= (const dvar_nil_t & other)	const throw()	{ return compare(other) <= 0;	}
	bool 	operator >  (const dvar_nil_t & other)	const throw()	{ return compare(other) >  0;	}
	bool 	operator >= (const dvar_nil_t & other)	const throw()	{ return compare(other) >= 0;	}

	/*************** display function	*******************************/
	std::string	to_string()	const throw();
	friend std::ostream & operator << ( std::ostream& os, const dvar_nil_t &dvar )
						{ return os << dvar.to_string();	}
};

NEOIP_NAMESPACE_END

#endif	/* __NEOIP_DVAR_NIL_HPP__  */


