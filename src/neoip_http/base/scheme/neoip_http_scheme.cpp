/*! \file
    \brief Definition of the http_scheme_t

\par Note abouit RTMP
RTMP is not really a http_uri_t but i dont want to change all the naming
just for that, now.

*/


/* system include */
/* local include */
#include "neoip_http_scheme.hpp"

NEOIP_NAMESPACE_BEGIN;

NEOIP_STRTYPE_DEFINITION_START(http_scheme_t,int)
NEOIP_STRTYPE_DEFINITION_ITEM(http_scheme_t, HTTP	, "http" )
NEOIP_STRTYPE_DEFINITION_ITEM(http_scheme_t, HTTPS	, "https" )
NEOIP_STRTYPE_DEFINITION_ITEM(http_scheme_t, RTMP	, "rtmp" )
NEOIP_STRTYPE_DEFINITION_END(http_scheme_t)

NEOIP_NAMESPACE_END

