/*! \file
    \brief Definition of the ntudp_resp_estarelay_t

\par Brief description
This modules handle the responder part for the estarelay connection establishment.

*/

/* system include */
/* local include */
#include "neoip_ntudp_resp_estarelay.hpp"
#include "neoip_ntudp_resp.hpp"
#include "neoip_ntudp_resp_profile.hpp"
#include "neoip_ntudp_peer.hpp"
#include "neoip_ntudp_aview_pool.hpp"
#include "neoip_ntudp_npos_saddrecho.hpp"
#include "neoip_ntudp_sock_pkttype.hpp"
#include "neoip_ntudp_sock_errcode.hpp"
#include "neoip_udp_client.hpp"
#include "neoip_log.hpp"
#include "neoip_nipmem_alloc.hpp"
#include "neoip_namespace.hpp"

NEOIP_NAMESPACE_BEGIN;


//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//                        ctor/dtor
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

/** \brief Constructor
 */
ntudp_resp_estarelay_t::ntudp_resp_estarelay_t(ntudp_resp_t *ntudp_resp
				, ntudp_resp_estarelay_cb_t *callback, void * userptr)	throw()
{
	// copy the parameter
	this->ntudp_resp	= ntudp_resp;
	this->callback		= callback;
	this->userptr		= userptr;
	// zero some fields
	udp_client		= NULL;	
	// link this object to the ntudp_resp_t
	ntudp_resp->estarelay_link(this);
}

/** \brief Desstructor
 */
ntudp_resp_estarelay_t::~ntudp_resp_estarelay_t()		throw()
{
	// unlink this object from the ntudp_resp_t
	ntudp_resp->estarelay_unlink(this);
	// delete the udp_client if needed
	if(udp_client)	nipmem_delete	udp_client;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//                     Setup Function
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

/** \brief Start the action
 * 
 * @return a datum containing the reply to the request_datum
 */
datum_t	ntudp_resp_estarelay_t::start(const datum_t &request_datum)	throw()
{
	ntudp_peer_t *		ntudp_peer	= ntudp_resp->ntudp_peer;
	ntudp_sock_pkttype_t	pkttype;
	ntudp_peerid_t		src_peerid, dst_peerid;
	ntudp_portid_t		src_portid, dst_portid;	
	ipport_addr_t		remote_ipport;
	pkt_t			pkt(request_datum);
	// parse the incoming packet
	try {
		pkt >> pkttype;
		pkt >> client_nonce;
		pkt >> dst_peerid >> dst_portid;
		pkt >> src_peerid >> src_portid;
		pkt >> remote_ipport;
	}catch(serial_except_t &e){
		// log the event
		KLOG_ERR("Can't parse incoming packet due to " << e.what() );
		return datum_t();
	}
	
	// sanity check - the packet type MUST be ntudp_sock_pkttype_t::ESTARELAY_CNX_REQUEST
	DBG_ASSERT( pkttype == ntudp_sock_pkttype_t::ESTARELAY_CNX_REQUEST );
	
	// set the local/remote ntudp_addr_t from the peerid/portid of the packet
	m_local_addr	= ntudp_addr_t(dst_peerid, dst_portid);
	m_remote_addr	= ntudp_addr_t(src_peerid, src_portid);

	// if the cnxaddr is already bound, reply an error packet ALREADY_BOUND
	if( ntudp_peer->cnxaddr_is_bound(local_addr(), remote_addr()) ){
		// send back a error packet to the remote peer
		pkt_t pkt_err = ntudp_sock_errcode_build_pkt(ntudp_sock_errcode_t::ALREADY_BOUND, client_nonce);
		// autodelete this
		nipmem_delete this;
		// return the reply
		return pkt_err.to_datum();
	}
	
	// try to steal a ntudp_npos_saddrecho_t from the aview_pool
	ntudp_aview_pool_t *	aview_pool	= ntudp_peer->aview_pool();
	ntudp_npos_saddrecho_t *saddrecho	= aview_pool->steal_one_saddrecho();
	// if no ntudp_npos_saddrecho_t have been stolen, ignore this packet
	if( !saddrecho ){
		nipmem_delete this;
		return datum_t();
	}
	
	// extract the ipport_aview_t from the saddrecho and delete it
	ipport_aview_t	local_aview	= saddrecho->local_addr_aview();
	nipmem_zdelete	saddrecho;

	// create and start the udp_client on the ipport_aview_t from the aview_pool
	inet_err_t	inet_err;
	udp_client	= nipmem_new udp_client_t();
	inet_err	= udp_client->set_local_addr(local_aview.lview());
	if( inet_err.succeed() )	inet_err = udp_client->start(remote_ipport, this, NULL);
	if( inet_err.failed() ){
		nipmem_delete this;
		return datum_t();
	}
	
	// return a ntudp_sock_pkttype_t::ESTARELAY_CNX_REPLY
	return build_estarelay_cnx_reply(local_aview.pview()).to_datum();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//                     timeout callback
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

/** \brief callback called when the neoip_timeout expire
 */
bool ntudp_resp_estarelay_t::neoip_timeout_expire_cb(void *userptr, timeout_t &cb_timeout)	throw()
{
	// log to debug
	KLOG_DBG("enter");
	// notify the expiration of the timeout to the delaygen
	rxmit_delaygen.notify_expiration();
	// if the delaygen is is_timedout, notify the caller of the faillure
	if( rxmit_delaygen.is_timedout() )	return notify_callback(NULL, pkt_t(), pkt_t());

	// if the udp_client_t is still in itor phase, do nothing
	// - NOTE: due to the replacement of udp_client_t when it report fatal event, this function may 
	//   be called when the new udp_client_t is still in itor.
	if( udp_client->is_itor() )		return true;

	// build the packet to send
	pkt_t	pkt	= build_estarelay_cnx_r2i_ack();

	// send the request
	udp_client->send( pkt );

	// set the next timer
	rxmit_timeout.change_period(rxmit_delaygen.pre_inc());
	// return a 'tokeep'
	return true;
}


//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//                        packet BUILD
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

/** \brief build the ESTARELAY_CNX_REPLY
 */
pkt_t ntudp_resp_estarelay_t::build_estarelay_cnx_reply(const ipport_addr_t &local_addr_pview)	const throw()
{
	pkt_t	pkt;
	// put the packet type
	pkt << ntudp_sock_pkttype_t(ntudp_sock_pkttype_t::ESTARELAY_CNX_REPLY);
	// put the nonce
	pkt << client_nonce;
	// put the local_addr_pview
	pkt << local_addr_pview;
	// return the packet
	return pkt;
}

/** \brief build the ESTARELAY_CNX_R2I_ACK
 */
pkt_t ntudp_resp_estarelay_t::build_estarelay_cnx_r2i_ack()	const throw()
{
	pkt_t	pkt;
	// put the packet type
	pkt << ntudp_sock_pkttype_t(ntudp_sock_pkttype_t::ESTARELAY_CNX_R2I_ACK);
	// put the nonce
	pkt << client_nonce;
	// return the packet
	return pkt;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//                     udp_client_t callback
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

/** \brief callback notified by \ref udp_client_t when a connection is established
 */
bool	ntudp_resp_estarelay_t::neoip_udp_client_event_cb(void *userptr, udp_client_t &cb_udp_client
						, const udp_event_t &udp_event)	throw()
{
	// log to debug
	KLOG_DBG("enter event=" << udp_event);
	// sanity check - the event MUST be client_ok
	DBG_ASSERT( udp_event.is_client_ok() );
	
	// handle the fatal events
	if( udp_event.is_fatal() ){
		// log the event
		KLOG_INFO("received fatal event on " << *udp_client );
		// special handling for udp_client_t fatal event - to avoid some network race
		return handle_udp_fatal_event();
	}

	// handle each possible events from its type
	switch( udp_event.get_value() ){
	case udp_event_t::CNX_ESTABLISHED:
			// init delaygen for rxmit of packet
			rxmit_delaygen = delaygen_t(ntudp_resp->profile->itor_pkt_rxmit());
			// start the timer immediatly
			rxmit_timeout.start(rxmit_delaygen.current(), this, NULL);
			break;
	case udp_event_t::RECVED_DATA:
			// handle the received packet
			return handle_recved_data(*udp_event.get_recved_data());
	default:	break;
	}
	// return 'tokeep'
	return true;
}

/** \brief Handle fatal udp_event_t received by the udp_client_t
 * 
 * - this function deletes the current udp_client_t and recreate one with the 
 *   same local_ipport/remote_ipport.
 * - it is made to ignore udp_client_t fatal error while sending ESTARELAY_CNX_I2R_ACK
 * 
 * Motivation
 * - it starts sending ESTARELAY_CNX_I2R_ACK immediatly after receiving ESTARELAY_CNX_REQUEST
 *   and replying a ESTARELAY_CNX_REPLY (containing the local_addr_pview)
 * - so if the ESTARELAY_CNX_I2R_ACK reaches the nat box of the itor before it received
 *   the ESTARELAY_CNX_REPLY and opened its own local_addr_pview, the nat box may 
 *   returns an icmp error which would cause udp_client_t to report a fatal udp_event_t
 * - so the udp_client_t is restarted at each fatal udp_event_t during the whole
 *   life of its ntudp_resp_estarelay_t
 * 
 * @return a 'tokeep' for the udp_clien_t which generated this fatal event
 */
bool ntudp_resp_estarelay_t::handle_udp_fatal_event()	throw()
{
	ipport_addr_t	local_ipport	= udp_client->get_local_addr();
	ipport_addr_t	remote_ipport	= udp_client->get_remote_addr();
	inet_err_t	inet_err;
	// delete the current udp_client_t
	nipmem_delete	udp_client;
	// create a new one to replace the failing one
	udp_client	= nipmem_new udp_client_t();
	inet_err	= udp_client->set_local_addr(local_ipport);
	if( inet_err.succeed() )	inet_err = udp_client->start(remote_ipport, this, NULL);
	if( inet_err.failed() )	notify_callback(NULL, pkt_t(), pkt_t());

	// return dontkeep as the udp_client_t which produced this event has been deleted
	return false;	
}

/** \brief Handle received data on the cnx_t
 * 
 * @return a 'tokeep/dontkeep' for the udp_full_t
 */
bool	ntudp_resp_estarelay_t::handle_recved_data(pkt_t &pkt)	throw()
{
	ntudp_sock_pkttype_t	pkttype;
	// log to debug
	KLOG_DBG("enter");

	try {	// read the pkttype (without consuming)	
		pkt.unserial_peek( pkttype );
		// handle the packet according to the pkttype
		switch( pkttype.get_value() ){
		case ntudp_sock_pkttype_t::ESTARELAY_CNX_I2R_ACK:return recv_estarelay_cnx_i2r_ack(pkt);
		case ntudp_sock_pkttype_t::ERROR_PKT:		return recv_error_pkt(pkt);
		default:	KLOG_ERR("Unexpected packet type " << pkttype );
				return true;
		}
	}catch(serial_except_t &e){
		// log the event
		KLOG_ERR("Can't parse incoming packet due to " << e.what() );
		return true;
	}

	// return 'tokeep'
	return true;
}


//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//                        packet RECV
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

/** \brief handle the reception of a ESTARELAY_CNX_I2R_ACK
 * 
 * @return a 'tokeep/dontkeep' for the udp_full_t
 */
bool	ntudp_resp_estarelay_t::recv_estarelay_cnx_i2r_ack(pkt_t &pkt_i2r_ack)	throw(serial_except_t)
{
	ntudp_sock_pkttype_t	pkttype;	
	ntudp_nonce_t		incoming_nonce;
	pkt_t			pkt = pkt_i2r_ack;	
	// log to debug
	KLOG_ERR("enter");
	// read the pkttype
	pkt >> pkttype;
	// read the incoming nounce
	pkt >> incoming_nonce;
	// if the incoming_nonce is different thand client_nonce, discard the packet
	if( incoming_nonce != client_nonce )	return true;

	// steal the udp_full_t from the udp_client_t - after this the udp_client_t MUST be deleted
	udp_full_t *	udp_full_stolen	= udp_client->steal_full();
	// delete the udp_client and mark it unused
	nipmem_zdelete	udp_client;

	// notify the sucess to the caller
	pkt_t	pkt_r2i_ack	= build_estarelay_cnx_r2i_ack();
	notify_callback(udp_full_stolen, pkt_i2r_ack, pkt_r2i_ack);
	
	// return dontkeep as the udp_client has been deleted
	return false;
}

/** \brief handle the reception of a ERROR_PKT
 * 
 * @return a 'tokeep/dontkeep' for the udp_full_t
 */
bool	ntudp_resp_estarelay_t::recv_error_pkt(pkt_t &pkt)		throw(serial_except_t)
{
	ntudp_sock_pkttype_t	pkttype;	
	ntudp_nonce_t		incoming_nonce;
	ntudp_sock_errcode_t	sock_errcode;
	// log to debug
	KLOG_ERR("enter");
	// read the pkttype
	pkt >> pkttype;
	// read the incoming nounce
	pkt >> incoming_nonce;
	// if the incoming_nonce is different thand client_nonce, discard the packet
	if( incoming_nonce != client_nonce )	return true;
	
	// read the ntudp_sock_errcode_t
	pkt >> sock_errcode;
	
	// log to debug
	KLOG_ERR("sock_errcode=" << sock_errcode);
	
	// notify the faillure to the caller
	return notify_callback(NULL, pkt_t(), pkt_t());
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//                     main function to notify event to the caller
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

/** \brief notify the caller callback
 * 
 * @return a tokeep
 */
bool 	ntudp_resp_estarelay_t::notify_callback(udp_full_t *udp_full, const pkt_t &estapkt_in
						, const pkt_t &estapkt_out)	throw()
{
	// sanity check - the callback MUST NOT be NULL
	DBG_ASSERT( callback );
	// backup the tokey_check_t context to check after the callback notification
	TOKEEP_CHECK_BACKUP_DFL(*callback);
	// notify the caller
	bool tokeep = callback->neoip_ntudp_resp_estarelay_cb(userptr, *this, udp_full
							, estapkt_in, estapkt_out);
	// sanity check - tokeep MUST be false if the local object has been deleted, true otherwise 
	TOKEEP_CHECK_MATCH_DFL(tokeep);
	// sanity check - specific to ntudp_resp_estarelay_t, object MUST be deleted during notification
	DBG_ASSERT( tokeep == false );		
	// return the tokeep
	return tokeep;
}


NEOIP_NAMESPACE_END

