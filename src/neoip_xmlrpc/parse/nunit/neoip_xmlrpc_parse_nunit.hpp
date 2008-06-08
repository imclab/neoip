/*! \file
    \brief Header of the test of xmlrpc_t
*/


#ifndef __NEOIP_XMLRPC_PARSE_NUNIT_HPP__ 
#define __NEOIP_XMLRPC_PARSE_NUNIT_HPP__ 
/* system include */
/* local include */
#include "neoip_nunit_testclass_api.hpp"
#include "neoip_nunit_testclass_ftor.hpp"
#include "neoip_namespace.hpp"

NEOIP_NAMESPACE_BEGIN;

/** \brief Class which implement a nunit for the xmlrpc_t
 */
class xmlrpc_parse_testclass_t : public nunit_testclass_api_t {
private:
public:
	/*************** nunit test function	*******************************/
	nunit_res_t	example_from_spec(const nunit_testclass_ftor_t &testclass_ftor)	throw();
	nunit_res_t	to_json(const nunit_testclass_ftor_t &testclass_ftor)		throw();
};


NEOIP_NAMESPACE_END

#endif	/* __NEOIP_XMLRPC_PARSE_NUNIT_HPP__  */


