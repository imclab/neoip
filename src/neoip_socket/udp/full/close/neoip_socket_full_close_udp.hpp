/*! \file
    \brief Header of the socket_full_t
*/


#ifndef __NEOIP_SOCKET_FULL_CLOSE_UDP_HPP__ 
#define __NEOIP_SOCKET_FULL_CLOSE_UDP_HPP__ 
/* system include */
#include <iostream>
/* local include */
#include "neoip_socket_full_vapi.hpp"
#include "neoip_socket_full_vapi_cb.hpp"
#include "neoip_timeout.hpp"
#include "neoip_zerotimer.hpp"
#include "neoip_copy_ctor_checker.hpp"
#include "neoip_namespace.hpp"

NEOIP_NAMESPACE_BEGIN

// list of forward declaration
class socket_full_udp_t;

/** \ref class to store the socket full while closing it
 */
class socket_full_close_udp_t : NEOIP_COPY_CTOR_DENY, private socket_full_vapi_cb_t, public timeout_cb_t
					, public zerotimer_cb_t {
private:
	socket_full_udp_t *	socket_full_udp;
	bool neoip_socket_full_vapi_cb(void *cb_userptr, socket_full_vapi_t &cb_full_vapi
					, const socket_event_t &socket_event)	throw();

	//! the list of state used by initiator
	enum state_t {
		NONE,
		LINGERING,	//!< when the socket is doing a linger (it happen iif 
				//!< should_linger() == true and mandatorily before state_t::CLOSING)
		CLOSING,	//!< when the socket is currently closing
		MAX
	};
	
	state_t		state;
	
	void		start_closure()		throw();
		
	// linger management function
	void		start_linger()		throw();
	void		stop_linger()		throw();
	bool		should_linger()		const throw();
	// linger timeout
	timeout_t	linger_timeout;		//!< timeout to stop linger to long to be completed
	bool		neoip_timeout_expire_cb(void *userptr, timeout_t &cb_timeout)	throw();

	// autodelete via zerotimer
	zerotimer_t	autodelete_zerotimer;
	bool		neoip_zerotimer_expire_cb(zerotimer_t &cb_zerotimer, void *userptr)	throw();
public:
	// ctor/dtor
	socket_full_close_udp_t(socket_full_udp_t *socket_full_udp)	throw();
	~socket_full_close_udp_t() 					throw();
	
	void	start()		throw();
};

NEOIP_NAMESPACE_END

#endif	/* __NEOIP_SOCKET_FULL_CLOSE_UDP_HPP__  */


