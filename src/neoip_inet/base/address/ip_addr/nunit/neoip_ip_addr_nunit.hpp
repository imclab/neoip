/*! \file
    \brief Header of the unit test of the ip_addr_t layer
*/


#ifndef __NEOIP_IP_ADDR_NUNIT_HPP__ 
#define __NEOIP_IP_ADDR_NUNIT_HPP__ 
/* system include */
/* local include */
#include "neoip_nunit_testclass_api.hpp"
#include "neoip_nunit_testclass_ftor.hpp"
#include "neoip_namespace.hpp"


NEOIP_NAMESPACE_BEGIN;

/** \brief Class which implement a nunit for the ip_addr_t
 */
class ip_addr_testclass_t : public nunit_testclass_api_t {
private:
public:
	/*************** nunit test function	*******************************/
	nunit_res_t	convert_from_string(const nunit_testclass_ftor_t &testclass_ftor)	throw();
	nunit_res_t	comparison_op(const nunit_testclass_ftor_t &testclass_ftor)		throw();
	nunit_res_t	serial_consistency(const nunit_testclass_ftor_t &testclass_ftor)	throw();
	nunit_res_t	general(const nunit_testclass_ftor_t &testclass_ftor)			throw();
};


NEOIP_NAMESPACE_END

#endif	/* __NEOIP_IP_ADDR_NUNIT_HPP__  */


