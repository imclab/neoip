/*! \file
    \brief Implementation of \ref cookie_db_t

*/

/* system include */
/* local include */
#include "neoip_cookie.hpp"
#include "neoip_skey_auth.hpp"
#include "neoip_nipmem_alloc.hpp"
#include "neoip_log.hpp"

NEOIP_NAMESPACE_BEGIN

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//                            CTOR/DTOR
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

/** \brief Constructor for the class
 */
cookie_db_t::cookie_db_t(const delay_t &renewal_period)	throw()
{
	// init the current secret
	// - This isnt stored in secure memory because a cookie is only a DoS protection.
	cur_secret = cookie_id_t::build_random().to_datum();
	// init the old secret with the current secret, just as a way to bootstrap
	old_secret = cur_secret;
	// start the rekey timeout
	rekey_timeout.start(renewal_period, this, NULL);	
}

/** \brief Destructor for the class
 */
cookie_db_t::~cookie_db_t()	throw()
{
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//                   rekey_timeout callback
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

/** \brief callback called when the timeout_t expire
 */
bool	cookie_db_t::neoip_timeout_expire_cb(void *userptr, timeout_t &cb_timeout)			throw()
{
	// copy the current secret to the old secret
	old_secret	= cur_secret;
	// copy the current accepted cookie_id database to the old one
	old_accepted_db = cur_accepted_db;

	// init a new current secret
	cur_secret	= cookie_id_t::build_random().to_datum();
	// init a new accepted cookie_id database for it
	cur_accepted_db	= std::set<cookie_id_t>();
	return true;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/** \brief generate a cookie_id_t from a pathid
 */
cookie_id_t	cookie_db_t::cpu(const datum_t &pathid, const datum_t &secret)	const throw()
{
	skey_auth_t	skey_auth("sha1/hmac/20");
	// init the key
	skey_auth.init_key( secret.get_data(), secret.get_len() );
	// compute the skey_auth over the pathid
	skey_auth.update( pathid.get_data(), pathid.get_len() );
	// return a cookie_id_t containing the result
	return cookie_id_t( skey_auth.get_output() );
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//                action function
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/** \brief generate the current cookie_id_t from a pathid
 */
cookie_id_t	cookie_db_t::generate(const datum_t &pathid)			const throw()
{
	return cpu(pathid, cur_secret);
}

/** \brief return true if the cookie cookie_id is acceptable for this pathid
 * 
 * - it checks the value of the cookie_id if and if the cookie_id has no yet been
 *   accepted.
 */
bool	cookie_db_t::check(const datum_t &pathid, const cookie_id_t &cookie_id)	const throw()
{
// test with the current secret
	// test if the cookie_id can be generated by the current secret
	if( cookie_id == cpu(pathid, cur_secret) ){
		// if the cookie_id has already been accepted, return false
		if( cur_accepted_db.find(cookie_id) != cur_accepted_db.end() )	return false;
		// else the cookie_id is acceptable
		return true;
	}
// test with the old secret
	// test if the cookie_id can be generated by the old secret
	if( cookie_id == cpu(pathid, old_secret) ){
		// if the cookie_id has already been accepted, return false
		if( old_accepted_db.find(cookie_id) != old_accepted_db.end() )	return false;
		// else the cookie_id is acceptable
		return true;
	}
	// else the cookie_id isnt acceptable
	return false;
}

/** \brief accept the cookie_id.
 * 
 * - this cookie_id MUST have already been checked
 */
void	cookie_db_t::accept(const datum_t &pathid, const cookie_id_t &cookie_id)	throw()
{
	// test if the cookie_id can be generated by the current secret, insert it in the current accepted db
	if( cookie_id == cpu(pathid, cur_secret) ){
		// insert the cookie_id in the database
		bool succeed = cur_accepted_db.insert(cookie_id).second;
		// sanity check - as the cookie_id isnt in the database, the insert MUST work
		DBG_ASSERT( succeed );
		return;
	}
	// test if the cookie_id can be generated by the old secret, insert it in the old accepted db
	if( cookie_id == cpu(pathid, old_secret) ){
		// insert the cookie_id in the database
		bool succeed = old_accepted_db.insert(cookie_id).second;
		// sanity check - as the cookie_id isnt in the database, the insert MUST work
		DBG_ASSERT( succeed );
		return;
	}
	// NOTE: this point shouldnever be reached
	DBG_ASSERT( 0 );
}

NEOIP_NAMESPACE_END



