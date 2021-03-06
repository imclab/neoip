/*! \file
    \brief Implementation of the eloop_t

*/

#ifdef USE_ELOOP_GLIB
/* system include */
#include <iostream>
/* local include */
#include "neoip_eloop_glib.hpp"
#include "neoip_log.hpp"

NEOIP_NAMESPACE_BEGIN

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//                           CTOR/DTOR
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

/** \brief Default Constructor
 */
eloop_t::eloop_t()	throw()
{
	/* glib init */
	glib_mainloop	= g_main_loop_new(NULL, FALSE);
	DBG_ASSERT( glib_mainloop != NULL );	
}

/** \brief Default Constructor
 */
eloop_t::~eloop_t()	throw()
{
	// destroy the glib_mainloop
	g_main_loop_unref(glib_mainloop);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//                      action function
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/** \brief Start to run on the eloop_t
 */
void	eloop_t::loop_run()	throw()
{
	g_main_loop_run( glib_mainloop );
}

/** \brief Stop to run on the eloop_t
 */
void	eloop_t::loop_stop()	throw()
{
	g_main_loop_quit( glib_mainloop );
}

NEOIP_NAMESPACE_END
#endif 


