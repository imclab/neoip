/*! \file
    \brief Declaration of the ntudp_npos_saddrecho_t
    
*/


#ifndef __NEOIP_NTUDP_NPOS_SADDRECHO_CB_HPP__ 
#define __NEOIP_NTUDP_NPOS_SADDRECHO_CB_HPP__ 
/* system include */
/* local include */
#include "neoip_namespace.hpp"

NEOIP_NAMESPACE_BEGIN

// forward declaration
class ntudp_npos_saddrecho_t;
class ntudp_npos_event_t;

/** \brief the callback class for ntudp_npos_saddrecho_t
 */
class ntudp_npos_saddrecho_cb_t {
public:
	/** \brief callback notified by \ref ntudp_npos_saddrecho_t when to notify an event
	 * 
	 * @param userptr 		 	the userptr associated with this callback
	 * @param ntudp_npos_saddrecho 	the ntudp_npos_saddrecho_t which notified this callback
	 * @param ntudp_npos_event		notified event
	 * @return true if the ntudp_npos_saddrecho_t is still valid after the callback
	 */
	virtual bool neoip_ntudp_npos_saddrecho_event_cb(void *cb_userptr
					, ntudp_npos_saddrecho_t &cb_ntudp_npos_saddrecho
					, const ntudp_npos_event_t &ntudp_npos_event)	throw() = 0;
	virtual ~ntudp_npos_saddrecho_cb_t() {};
};

NEOIP_NAMESPACE_END

#endif	/* __NEOIP_NTUDP_NPOS_SADDRECHO_CB_HPP__  */


