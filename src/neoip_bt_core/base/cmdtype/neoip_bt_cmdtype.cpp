/*! \file
    \brief Implementation of the \ref bt_cmdtype_t
*/

/* system include */
/* local include */
#include "neoip_bt_cmdtype.hpp"
#include "neoip_log.hpp"

NEOIP_NAMESPACE_BEGIN;

NEOIP_STRTYPE_DEFINITION_START(bt_cmdtype_t	, uint8_t)
NEOIP_STRTYPE_DEFINITION_ITEM(bt_cmdtype_t	, UNAUTH_REQ	, "UNAUTH_REQ")
NEOIP_STRTYPE_DEFINITION_ITEM(bt_cmdtype_t	, DOAUTH_REQ	, "DOAUTH_REQ")
NEOIP_STRTYPE_DEFINITION_ITEM(bt_cmdtype_t	, DOWANT_REQ	, "DOWANT_REQ")
NEOIP_STRTYPE_DEFINITION_ITEM(bt_cmdtype_t	, UNWANT_REQ	, "UNWANT_REQ")
NEOIP_STRTYPE_DEFINITION_ITEM(bt_cmdtype_t	, PIECE_ISAVAIL	, "PIECE_ISAVAIL")
NEOIP_STRTYPE_DEFINITION_ITEM(bt_cmdtype_t	, PIECE_BFIELD	, "PIECE_BFIELD")
NEOIP_STRTYPE_DEFINITION_ITEM(bt_cmdtype_t	, BLOCK_REQ	, "BLOCK_REQ")
NEOIP_STRTYPE_DEFINITION_ITEM(bt_cmdtype_t	, BLOCK_REP	, "BLOCK_REP")
NEOIP_STRTYPE_DEFINITION_ITEM(bt_cmdtype_t	, BLOCK_DEL	, "BLOCK_DEL")
NEOIP_STRTYPE_DEFINITION_ITEM(bt_cmdtype_t	, ML_DHT_PORT	, "ML_DHT_PORT")
NEOIP_STRTYPE_DEFINITION_ITEM(bt_cmdtype_t	, PIECE_SUGGEST	, "PIECE_SUGGEST")
NEOIP_STRTYPE_DEFINITION_ITEM(bt_cmdtype_t	, HAVE_ALL	, "HAVE_ALL")
NEOIP_STRTYPE_DEFINITION_ITEM(bt_cmdtype_t	, HAVE_NONE	, "HAVE_NONE")
NEOIP_STRTYPE_DEFINITION_ITEM(bt_cmdtype_t	, REQ_REJECT	, "REQ_REJECT")
NEOIP_STRTYPE_DEFINITION_ITEM(bt_cmdtype_t	, UTMSG_PAYL	, "UTMSG_PAYL")
NEOIP_STRTYPE_DEFINITION_ITEM(bt_cmdtype_t	, KEEPALIVE	, "KEEPALIVE")
NEOIP_STRTYPE_DEFINITION_END(bt_cmdtype_t)

NEOIP_NAMESPACE_END




