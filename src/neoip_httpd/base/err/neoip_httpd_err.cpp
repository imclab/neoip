/*! \file
    \brief Definition of the \ref httpd_err_t

*/

/* system include */
/* local include */
#include "neoip_httpd_err.hpp"
#include "neoip_namespace.hpp"

NEOIP_NAMESPACE_BEGIN

NEOIP_ERRTYPE_DEFINITION_START(httpd_err_t)
NEOIP_ERRTYPE_DEFINITION_ITEM(httpd_err_t	, OK		, "OK")
NEOIP_ERRTYPE_DEFINITION_ITEM(httpd_err_t	, NOT_FOUND	, "NOT_FOUND")
NEOIP_ERRTYPE_DEFINITION_ITEM(httpd_err_t	, BOGUS_REQUEST	, "BOGUS_REQUEST")
NEOIP_ERRTYPE_DEFINITION_ITEM(httpd_err_t	, MOVED_TEMP	, "MOVED_TEMP")
NEOIP_ERRTYPE_DEFINITION_ITEM(httpd_err_t	, INTERNAL_ERROR, "INTERNAL_ERROR")
NEOIP_ERRTYPE_DEFINITION_ITEM(httpd_err_t	, DELAYED_REPLY	, "DELAYED_REPLY")
NEOIP_ERRTYPE_DEFINITION_ITEM(httpd_err_t	, SEND_FILE	, "SEND_FILE")
NEOIP_ERRTYPE_DEFINITION_ITEM(httpd_err_t	, UNAUTHORIZED	, "UNAUTHORIZED")
NEOIP_ERRTYPE_DEFINITION_END(httpd_err_t)

NEOIP_NAMESPACE_END





