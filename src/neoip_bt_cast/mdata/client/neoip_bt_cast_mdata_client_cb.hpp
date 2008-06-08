/*! \file
    \brief Declaration of the bt_cast_mdata_client_t
    
*/


#ifndef __NEOIP_BT_CAST_MDATA_CLIENT_CB_HPP__ 
#define __NEOIP_BT_CAST_MDATA_CLIENT_CB_HPP__ 

/* system include */
/* local include */
#include "neoip_namespace.hpp"

NEOIP_NAMESPACE_BEGIN

// forward declaration
class	bt_cast_mdata_client_t;
class	bt_cast_mdata_t;
class	bt_err_t;

/** \brief the callback class for bt_cast_mdata_client_t
 */
class bt_cast_mdata_client_cb_t {
public:
	/** \brief callback notified by \ref bt_cast_mdata_client_t
	 */
	virtual bool neoip_bt_cast_mdata_client_cb(void *cb_userptr, bt_cast_mdata_client_t &cb_mdata_client
					, const bt_err_t & bt_err
					, const bt_cast_mdata_t &cast_mdata)	throw() = 0;
	// virtual destructor
	virtual ~bt_cast_mdata_client_cb_t() {};
};

NEOIP_NAMESPACE_END

#endif	/* __NEOIP_BT_CAST_MDATA_CLIENT_CB_HPP__  */


