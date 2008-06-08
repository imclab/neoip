/*! \file
    \brief Header of the \ref http_status_t

*/


#ifndef __NEOIP_HTTP_STATUS_HPP__ 
#define __NEOIP_HTTP_STATUS_HPP__ 

/* system include */
/* local include */
#include "neoip_strtype.hpp"
#include "neoip_namespace.hpp"

NEOIP_NAMESPACE_BEGIN;

NEOIP_STRTYPE_DECLARATION_START(http_status_t	, 0)
NEOIP_STRTYPE_DECLARATION_ITEM_IDX(http_status_t, OK			, 200)
NEOIP_STRTYPE_DECLARATION_ITEM_IDX(http_status_t, PARTIAL_CONTENT	, 206)
NEOIP_STRTYPE_DECLARATION_ITEM_IDX(http_status_t, MOVED_PERMANENTLY	, 301)
NEOIP_STRTYPE_DECLARATION_ITEM_IDX(http_status_t, BAD_REQUEST		, 400)
NEOIP_STRTYPE_DECLARATION_ITEM_IDX(http_status_t, UNAUTHORIZED		, 401)
NEOIP_STRTYPE_DECLARATION_ITEM_IDX(http_status_t, NOT_FOUND		, 404)
NEOIP_STRTYPE_DECLARATION_ITEM_IDX(http_status_t, SERVICE_UNAVAILABLE	, 503)
NEOIP_STRTYPE_DECLARATION_ITEM_LAST(http_status_t)
NEOIP_STRTYPE_DECLARATION_END(http_status_t, int)

NEOIP_NAMESPACE_END

#endif	/* __NEOIP_HTTP_STATUS_HPP__  */
