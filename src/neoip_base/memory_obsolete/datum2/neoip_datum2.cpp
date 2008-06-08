/*! \file
    \brief Implementation of the datum2_t

\par Possible Improvement : overhead reduction by variable length encoding
- the serialization overhead could be reduced
  - currently the length of the datum2_t is stored in uint32_t to avoid limit
  - as most datum2_t are quite small and doesnt require that much
  - a possibility would be: 
    - if length < 255, store the length in 1-byte
    - if length < 65535, store a 255 in a byte, and then the length in 2-byte
    - else store 3 time 255 byte and then the length in 4-byte
    - it is a good solution as the addiotionnal overhead (up to 3 byte for datum2
      >= 2^16) is made negligible as it is proportionnal to the size of the datum2
  - NOTE: this is rather theorical and may cause some issue
    - it is a theorical improvement because in practice datum2_t are not serialized
      enougth to be a significant improvement
    - it may create issue because make the serialization space variable, so may
      cause issue in serializetion length computing
    - So this is not a complete no-go but for sure not a priority
*/

/* system include */
#include <iostream>
#include <iomanip>

/* local include */
#include "neoip_datum2.hpp"
#include "neoip_serial2.hpp"
#include "neoip_log.hpp"
#include "neoip_nipmem_alloc.hpp"

NEOIP_NAMESPACE_BEGIN;


// definition of \ref datum2_t constant
const datum2_flag_t	datum2_t::NOCOPY	= datum2_flag_t(1 << 0);
const datum2_flag_t	datum2_t::SECMEM	= datum2_flag_t(1 << 1);
const datum2_flag_t	datum2_t::FLAG_DFL	= datum2_flag_t(0);
// end of constants definition


//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//                     CTOR/DTOR
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

/** \brief internal constructor
 */
void datum2_t::internal_ctor(const void *orig_data, int orig_len, datum2_flag_t flag)	throw()
{
	// copy the parameter
	this->len	= orig_len;
	this->flag	= flag;
	// sanity check - the flag MUST NOT be NOCOPY and SECMEM
	DBG_ASSERT( !flag.is_nocopy() || !flag.is_secmem() );
	// if the length is 0, set data to NULL, and return
	if( len == 0 ){
		data = NULL;
		return;
	}
	// allocate the memory depending on the flag
	if( flag.is_nocopy() )		data	= (void *)orig_data;
	else if( flag.is_secmem() )	data	= nipmem_malloc_sec(len);
	else				data	= nipmem_malloc(len);	
	// copy the data IF there are any and IF the flag is NOT no copy
	if( !flag.is_nocopy() && orig_data )	memcpy( data, orig_data, len );
}

/** \brief free the data buffer if needed
 * 
 * - handle the nocopy and secure datum2_flag_t
 */
void	datum2_t::internal_dtor()			throw()
{
	// if there are no data to free, return now
	if( data == NULL || flag.is_nocopy() )	return;
	// if the data is in secmem, use a special free function
	if( flag.is_secmem() )			nipmem_free_sec(data, len);
	else					nipmem_free(data);
}


/** \brief overload the = operator
 */
datum2_t &	datum2_t::operator=(const datum2_t & other)	throw()
{
	// if auto-assignment, do nothing
	if( this == &other )	return *this;

	// destruct the current stuff
	internal_dtor();
	// init the local datum2_t
	internal_ctor(other.data, other.len, other.flag.inheritance());

	// return the object itself
	return *this;
}

/** \brief Swap the content of object
 */
void	datum2_t::swap(datum2_t &other)		throw()
{	
	std::swap( flag, other.flag );
	std::swap( data, other.data );
	std::swap( len, other.len );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//                   arithmetic operator
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

datum2_t &	datum2_t::operator +=(const datum2_t &other)			throw()
{
	size_t		tmp_len	= length() + other.length();
	// init the new datum2
	datum2_t	tmp_datum(tmp_len, (flag | other.flag).inheritance());
	// copy the datum2_a data into tmp_datum
	memcpy( tmp_datum.char_ptr(), char_ptr(), length() );
	// copy the datum2_b data into tmp_datum
	memcpy( tmp_datum.char_ptr() + length(), other.char_ptr(), other.length() );
	// assign the result tmp_datum to this datum2_t
	*this	= tmp_datum;
	// return the object itself
	return *this;
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/** \brief return a given range of data in a datum2_t
 * 
 * - the whole range is assumed to be in the original datum2_t
 * - the datum2_t::SECMEM is inherited in the produced datum2_t
 */
datum2_t	datum2_t::range(size_t range_offset, size_t range_len
					, const datum2_flag_t &range_flag)	const throw()
{
	// if the range_flag is nocopy, the range_flag and this flag MUST have the same inheritance
	DBG_ASSERT( range_flag.is_nocopy() || (flag.inheritance() == range_flag.inheritance()) );
	// sanity check - the whole range must be available
	DBG_ASSERT( range_offset + range_len <= length() );
	// return the produced range
	return datum2_t( char_ptr(range_offset), range_len, range_flag);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//                     main comparison function
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

/** \brief Compare 2 objects ala memcmp
 */
int	datum2_t::compare(const datum2_t &other)	const throw()
{
	// handle the case where at least one is null
	if(  is_null() && !other.is_null() )	return -1;
	if( !is_null() &&  other.is_null() )	return +1;
	if(  is_null() &&  other.is_null() )	return  0;
	// NOTE: here both are NOT null

	// handle them by the length - if both are not of the same length, the shorter is less than
	if( length() < other.length() )		return -1;
	if( length() > other.length() )		return +1;
	// NOTE: here both have the same length

	// if both length are equal, compare the data
	return memcmp( void_ptr(), other.void_ptr(), length() );
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//                        OSTREAM redirection (mainly debug)
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

/** \brief redirect the ostream << operator
 */
std::ostream & operator << (std::ostream & oss, const datum2_t &datum2)	throw()
{
	uint8_t	* p	= datum2.uint8_ptr();

	// add the length
	oss << "len="	<< datum2.length();
	
	// add the datum2_flag_t
	oss << " flag="	<< datum2.flag;
	
	// add the data
	oss << " data=";
	if( datum2.length() == 0 )	return oss;
	oss << "\n";
	for(size_t offset = 0 ; offset < datum2.length() ; offset += 16){
		// put the line offset
		oss << std::hex << std::setfill('0') << std::setw(4) << offset << ":";
		// put the data in hexa
		for(size_t i = 0 ; i < 16 ; i++){
			if( offset+i < datum2.length() ){
				oss << " " << std::hex << std::setfill('0') << std::setw(2) << (int)p[offset+i];
			}else{
				oss << "   ";
			}
		}
		// put the begining of the printable ascii part
		oss << "\t| ";
		// put the data in printable ascii
		for(size_t i = 0 ; i < 16 ; i++){
			if( offset+i < datum2.length() ){
				if( isprint(p[offset+i]) )	oss << p[offset+i];
				else				oss << ".";
			}else{
				oss << " ";
			}
		}
		// put the end of the printable ascii part
		oss << " |";
		// goto the next line
		if( offset + 16 < datum2.length() )	oss << "\n";
	}
	// return the oss itself
	return oss;
}

/** \brief convert the object to a string
 */
std::string	datum2_t::to_string()	const throw()
{
	std::ostringstream      oss;
	uint8_t	*		p	= uint8_ptr();
	// if the datum2 is empty, return a null string
	if( length() == 0 )	return oss.str();
	// else return the datum2 in a big "0xfffff" form
	oss << "0x";
	for(size_t offset = 0 ; offset < length() ; offset++)
		oss << std::hex << std::setfill('0') << std::setw(2) << (int)p[offset];
	// return the built string
	return oss.str();	
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//                          serial datum2_t
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

/** \brief serialize a \ref datum2_t
 */
serial2_t& operator << (serial2_t& serial, const datum2_t &datum)		throw()
{
	// serialize the length
	serial	<< (uint32_t)datum.length();
	// serialize the data
	serial.append(datum.void_ptr(), datum.length());
	// return the serial2_t itself
	return serial;
}

/** \brief unserialize a \ref datum2_t
 */
serial2_t& operator >> (serial2_t& serial, datum2_t &datum) 			throw(serial2_except_t)
{
	// get the length in the payload
	uint32_t	len;
	serial	>> len;
	// build the datum from the serial data
	// - the datum.flag.inheritance allow to unserialize into a datum2_t::SECMEM if the flag
	//   is set in the datum.
//	datum = serial.head_consume(len, datum.flag.inheritance());
	DBG_ASSERT( 0 );
	// return the serial2_t itself
	return serial;
}

#if 0
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//                          serial datum2_t
// - TODO to remove it is only for the datum_t transition
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

/** \brief serialize a \ref datum2_t
 */
serial_t& operator << (serial_t& serial, const datum2_t &datum)		throw()
{
	// serialize the length
	serial	<< (uint32_t)datum.get_len();
	// serialize the data
	serial.append( datum.get_data(), datum.get_len() );
	return serial;
}

/** \brief unserialize a \ref datum2_t
 */
serial_t& operator >> (serial_t& serial, datum2_t &datum) 		throw(serial_except_t)
{
	// get the length in the payload
	uint32_t	len;
	serial		>> len;
	// check the length
	if( serial.get_len() < len )	throw serial_except_t("datum2_t Payload Too Short");
	// build the datum
	datum = datum2_t(serial.get_data(), len);
	// consume the buffer
	serial.consume( len );
	return serial;
}
#endif

NEOIP_NAMESPACE_END