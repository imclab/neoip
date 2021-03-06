/*! \file
    \brief Definition of the \ref http_uri2_t

*/

/* system include */
/* local include */
#include "neoip_http_uri2.hpp"
#include "neoip_base64.hpp"	// NOTE: needed for the uri scramble
#include "neoip_log.hpp"

NEOIP_NAMESPACE_BEGIN


//! unreserved charset for url from rfc2396.2.3
const std::string	http_uri2_t::UNRESERVED_CHARSET	=
					"abcdefghijklmnopqrstuvwxyz"	// lower case letters
					"ABCDEFGHIJKLMNOPQRSTUVWXYZ"	// upper case letters
					"0123456789"			// digits
					"-_.!~*'()"			// mark
					;

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//                   CTOR/DTOR
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

/** \brief Parse the string as a http URI - as in rfc2616.3.2.2
 */
void	http_uri2_t::ctor_from_str(const std::string &str)	throw()
{
	std::string	str2parse	= str;
	size_t		strpos;
	http_scheme_t	tmp_scheme;
	std::string	tmp_host;
	size_t		tmp_port;
	file_path_t	tmp_path	= "/";		// the default http path
	std::string	tmp_anchor;
	// log to debug
	KLOG_DBG("enter str=" << str);

	// parse the scheme
	std::string	scheme_sepa	= "://";
	size_t		scheme_endpos	= string_t::find_substr(str2parse, scheme_sepa);
	tmp_scheme	= str2parse.substr(0,scheme_endpos);
	// if tmp_scheme is null, return a null http_uri2_t now
	if( tmp_scheme.is_null() )	return;
	// remove the scheme from str2parse
	str2parse.erase(0, scheme_endpos + scheme_sepa.size());

	// set the default_port based on the scheme
	tmp_port	= tmp_scheme.default_port();

	// try to find a delimiter port ':', or path '/' or query '?' or anchor '#'
	strpos	= str2parse.find_first_of(":/?#", 0);
	// get the host out of it
	tmp_host	= str2parse.substr(0, strpos);
	str2parse.erase(0, strpos);

	// if there is a port specified, parse it
	if( str2parse[0] == ':' ){
		// consume the port delimiter
		str2parse.erase(0, 1);
		// find the end of the number
		strpos		= str2parse.find_first_not_of("0123456789");
		// get the port out of it
		tmp_port	= atoi( str2parse.substr(0, strpos).c_str() );
		// consume the port string
		str2parse.erase(0, strpos);
	}

	// if there is a path specified, parse it
	if( str2parse[0] == '/' ){
		std::string	path_str;
		// find the end of the path
		strpos		= str2parse.find_first_of("?#");
		// extract the std::string of the path
		path_str	= str2parse.substr(0, strpos);
		// get the path out of it
		tmp_path	= string_t::unescape(path_str);
		// consume the path string
		str2parse.erase(0, strpos);
	}

	// if there is a query specified, parse it
	if( str2parse[0] == '?' ){
		// consume the query delimiter
		str2parse.erase(0, 1);
		// find the end of the query
		strpos		= str2parse.find_first_of("#");
		// get the query out of it
		std::string	query_str;
		query_str	= str2parse.substr(0, strpos);
		str2parse.erase(0, strpos);
		// parse the variables
		std::vector<std::string>	var = string_t::split(query_str, "&" );
		// populate the tmp_variable
		for(size_t i = 0; i < var.size(); i++){
			std::vector<std::string> name_value	= string_t::split(var[i], "=", 2);
			std::string var_key	= name_value[0];
			std::string var_val	= name_value.size() == 2 ? name_value[1] : std::string();
			uri_var.append( string_t::unescape(var_key), string_t::unescape(var_val) );
		}
	}

	// if there is a anchor specified, parse it
	if( str2parse[0] == '#' ){
		// consume the query delimiter
		str2parse.erase(0, 1);
		// the rest of the str2parse if the anchor
		tmp_anchor	= string_t::unescape(str2parse);
	}
	// log to debug
	KLOG_DBG("uri " << str << " is valid");

	// if this point is reached, the str is considered a valid http_uri2_t
	uri_scheme	= tmp_scheme;
	uri_host	= tmp_host;
	uri_port	= tmp_port;
	uri_path	= tmp_path;
	uri_anchor	= tmp_anchor;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//                     action function
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/** \brief Clear the hostport part of the http_uri2_t
 *
 * - aka replace hostport by "0.0.0.0:80" for http: and "0.0.0.0:443" for https:
 */
http_uri2_t &	http_uri2_t::clear_hostport()	throw()
{
	// sanity check - the http_uri2_t MUST NOT be null
	DBG_ASSERT( !is_null() );
	// set the 'cleared' value
	uri_host	= "0.0.0.0";
	uri_port	= scheme().default_port();
	// return the object itself
	return *this;
}

/** \brief Clear the pathquery part of the http_uri2_t
 */
http_uri2_t &	http_uri2_t::clear_pathquery()	throw()
{
	// sanity check - the http_uri2_t MUST NOT be null
	DBG_ASSERT( !is_null() );
	// set the 'cleared' value
	uri_path	= file_path_t();
	uri_var		= strvar_db_t();
	uri_anchor	= std::string();
	// return the object itself
	return *this;
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//                     main compare function
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/** \brief compare 2 objects (ala memcmp) - as in rfc2616.3.2.3
 *
 * - it return a value <  0 if the local object is less than the external one
 * - it return a value == 0 if the local object is equal to the external one
 * - it return a value >  0 if the local object is greater than the external one
 */
int http_uri2_t::compare( const http_uri2_t & other )  const throw()
{
	// handle the case where at least one is null
	if(  is_null() && !other.is_null() )	return -1;
	if( !is_null() &&  other.is_null() )	return +1;
	if(  is_null() &&  other.is_null() )	return  0;
	// NOTE: here both are NOT null

	// compare the scheme
	if( scheme()	< other.scheme() )	return -1;
	if( scheme()	> other.scheme() )	return +1;

	// compare the hostname - MUST be case-insensitive
	int host_cmp	= string_t::casecmp(host(), other.host());
	if( host_cmp )	return host_cmp;

	// compare the port
	if( port()	< other.port() )	return -1;
	if( port()	> other.port() )	return +1;

	// compare the path
	if( path()	< other.path() )	return -1;
	if( path()	> other.path() )	return +1;

	// compare the variable
	if( var() 	< other.var() )		return -1;
	if( var() 	> other.var() )		return +1;

	// compare the anchor
	if( anchor()	< other.anchor() )	return -1;
	if( anchor()	> other.anchor() )	return +1;

	// note: here both are considered equal
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//			uri scramble stuff
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/** \brief Return true if this http_uri2_t is scrambled, false otherwise
 */
bool		http_uri2_t::is_scrambled()		const throw()
{
	// log to debug
	KLOG_DBG("this=" << *this);
	KLOG_DBG("uri_path.size()=" << uri_path.size());
	KLOG_DBG("uri_path[0]=" << uri_path[0]);
	KLOG_DBG("uri_path[1]=" << uri_path[1]);
	// if there is uri_anchor, return false
	if( !uri_anchor.empty() )		return false;

	// if the uri_path is not of the proper form, return false
	if( uri_path.size() != 3 )		return false;
	if( uri_path[1] != "scrambled" )	return false;

	// if all previous test passed, this http_uri2_t is considered scambled, return true
	return true;
}
/** \brief unscramble this http_uri
 */
http_uri2_t	http_uri2_t::unscramble()		const throw()
{
	// sanity check - the http_uri2_t MUST be scrambled
	DBG_ASSERT( is_scrambled() );

	// get the encoded pathquery
	std::string pathquery_enc	= uri_path[2].to_string();
	// decode the pathquery
	std::string pathquery_dec	= base64_t::decode_safe(pathquery_enc).to_stdstring();
	// build unscrambled uri
	http_uri2_t	new_uri;
	new_uri		= scheme().to_string() + "://" + hostport_str() + "/" + pathquery_dec;
	// merge the var() of this http_uri2_t to the unscrambled one
	for(size_t i = 0; i < var().size(); i++ )	new_uri.var() += var()[i];
	// log to debug
	KLOG_DBG("path_query=" 		<< pathquery_enc);
	KLOG_DBG("decoded query_query=" << pathquery_dec);
	KLOG_DBG("decoded req_uri=" 	<< new_uri);
	// return the just-build new_uri
	return new_uri;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//                        display function
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/** \brief Return a string representing only the http_uri2_t host:port WITHOUT the "http://" scheme
 */
std::string	http_uri2_t::hostport_str()	const throw()
{
	std::ostringstream	oss;
	// put the host
	oss	<< host();
	// put the port is not the default
	if( port() != scheme().default_port())		oss << ":" << port();
	// return the just built string
	return oss.str();
}

/** \brief Return a string representing only the http_uri2_t path
 */
std::string	http_uri2_t::path_str()		const throw()
{
	// put the path if not default
	return string_t::escape_not_in(uri_path.to_string(), "/" + UNRESERVED_CHARSET);
}

/** \brief Return a string representing only the http_uri2_t query
 */
std::string	http_uri2_t::query_str()		const throw()
{
	std::ostringstream	oss;
	// put the variable if any
	if( !uri_var.empty() ){
		// insert the query separator
		oss << "?";
		// add each variable
		for(size_t i = 0; i < uri_var.size(); i++){
			const std::string &	key	= uri_var[i].key();
			const std::string &	val	= uri_var[i].val();
			// add the intervariable separactor
			if( i != 0 )	oss << "&";
			// add the variable key
			oss << string_t::escape_not_in(key, UNRESERVED_CHARSET);
			// add the variable value if any
			if( !val.empty() ){
				oss << "=";
				oss << string_t::escape_not_in(val, UNRESERVED_CHARSET);
			}
		}
	}
	// return the just built string
	return oss.str();
}


/** \brief Return a string representing only the http_uri2_t path and query
 */
std::string	http_uri2_t::pathquery_str()	const throw()
{
	std::ostringstream	oss;
	// put the path
	oss << path_str();
	// put the query
	oss << query_str();
	// return the just built string
	return oss.str();
}

/** \brief Convert the object to a string
 */
std::string	http_uri2_t::to_string()	const throw()
{
	std::ostringstream	oss;
	// if the object is null, return "null"
	if( is_null() )	return "null";
	// put the scheme
	oss 	<< scheme() << "://";
	oss	<< hostport_str();
	oss	<< path_str();
	oss	<< query_str();
	// put the anchor is any
	if( !anchor().empty() )	oss << "#" << anchor();

	// return the just built string
	return oss.str();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//                          serialization
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

/** \brief serialize a http_uri2_t
 *
 * - support null http_uri2_t
 */
serial_t& operator << (serial_t& serial, const http_uri2_t &http_uri)		throw()
{
	// serialize each field of the object
	serial << datum_t(http_uri.to_string());
	// return serial
	return serial;
}

/** \brief unserialze a http_uri2_t
 *
 * - support null http_uri2_t
 */
serial_t& operator >> (serial_t & serial, http_uri2_t &http_uri)		throw(serial_except_t)
{
	datum_t		http_uri2_datum;
	// reset the destination variable
	http_uri	= http_uri2_t();
	// unserialize the data
	serial >> http_uri2_datum;
	// set the returned variable
	http_uri	= http_uri2_datum.to_stdstring();
	// return serial
	return serial;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//                          xmlrpc http_uri2_t
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

/** \brief doserialize xmlrpc for http_uri2_t
 */
xmlrpc_build_t &operator << (xmlrpc_build_t& xmlrpc_build, const http_uri2_t &http_uri)	throw()
{
	// serialize the data
	xmlrpc_build << http_uri.to_string();
	// return the object itself
	return xmlrpc_build;
}

/** \brief unserialize xmlrpc for http_uri2_t
 */
xmlrpc_parse_t &operator >> (xmlrpc_parse_t& xmlrpc_parse, http_uri2_t &http_uri)	throw(xml_except_t)
{
	std::string	tmp;
	// unserialize the data
	xmlrpc_parse >> tmp;
	// set the unserialized value in the http_uri2_t
	http_uri	= http_uri2_t(tmp);;
	// return the object itself
	return xmlrpc_parse;
}

NEOIP_NAMESPACE_END;




