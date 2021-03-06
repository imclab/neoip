/*! \file
    \brief Declaration of the bt_io_sfile_wikidbg_t

*/

/* system include */
/* local include */
#include "neoip_bt_io_sfile_wikidbg.hpp"
#include "neoip_bt_io_sfile.hpp"
#include "neoip_string.hpp"
#include "neoip_lib_httpd_wikidbg.hpp"
#include "neoip_log.hpp"

NEOIP_NAMESPACE_BEGIN

/** \brief \ref bt_swarm_http_t defines the wikidbg stuff for \ref bt_io_sfile_t
 */
class bt_io_sfile_wikidbg_t : NEOIP_COPY_CTOR_ALLOW {
public:
	static std::string	oneword(const std::string &keyword, void *object_ptr)		throw();
	static std::string	oneword_pageurl(const std::string &keyword, void *object_ptr)	throw();
	static std::string	page_title_attr(const std::string &keyword, void *object_ptr)	throw();
	static httpd_err_t	page(const std::string &keyword, void *object_ptr
								, httpd_request_t &request)	throw();
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/** \brief define all the handled keyword and their attached callbacks
 */
void	bt_io_sfile_wikidbg_init(wikidbg_keyword_db_t &keyword_db)	throw() 
{
	// add the keywords
	keyword_db.insert_html("oneword"	, bt_io_sfile_wikidbg_t::oneword);
	keyword_db.insert_html("oneword_pageurl", bt_io_sfile_wikidbg_t::oneword_pageurl);
	keyword_db.insert_html("page_title_attr", bt_io_sfile_wikidbg_t::page_title_attr);
	keyword_db.insert_page("page"		, bt_io_sfile_wikidbg_t::page);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/** \brief Handle the "oneword" keyword
 */
std::string bt_io_sfile_wikidbg_t::oneword(const std::string &keyword, void *object_ptr) throw()
{
	bt_io_sfile_t *		io_sfile	= (bt_io_sfile_t *)object_ptr;
	std::ostringstream	oss;

	oss << "bt_io_vapi_t " << io_sfile->mode();

	// return the built string
	return oss.str();
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/** \brief Handle the "oneword_pageurl" keyword
 */
std::string bt_io_sfile_wikidbg_t::oneword_pageurl(const std::string &keyword, void *object_ptr) throw()
{
	bt_io_sfile_t *		io_sfile	= (bt_io_sfile_t *)object_ptr;
	std::ostringstream	oss;
	html_builder_t		h;

	oss << h.s_link(wikidbg_url("page", io_sfile), wikidbg_html("page_title_attr", io_sfile));
	oss << wikidbg_html("oneword", io_sfile);
	oss << h.e_link();
	
	// return the built string
	return oss.str();
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/** \brief Handle the "page_title_attr" keyword
 */
std::string bt_io_sfile_wikidbg_t::page_title_attr(const std::string &keyword, void *object_ptr) throw()
{
	std::ostringstream	oss;

	oss << "title=\"";
	oss << "io_sfile established connection";
	oss << "\"";
	// return the built string
	return oss.str();
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/** \brief Handle the "page" keyword
 */
httpd_err_t bt_io_sfile_wikidbg_t::page(const std::string &keyword, void *object_ptr
						, httpd_request_t &request) throw()
{
	bt_io_sfile_t *		io_sfile	= (bt_io_sfile_t *)object_ptr;
	std::ostringstream	&oss 		= request.get_reply();
	html_builder_t		h;

	// put the title
	oss << h.pagetitle("bt_io_sfile_t Page");

	oss << h.br();
	oss << h.s_table_packed_noborder();
		oss << h.s_tr();
			oss << h.s_td() << h.s_b() << "io_mode"	<< h.e_b() << h.e_td();
			oss << h.s_td() << ": " << io_sfile->mode()
					<< h.e_td();
			oss << h.e_tr();
	oss << h.e_table();
	oss << h.br();

	// bt_io_stats_t
	oss << h.s_sub1title() << "bt_io_stats_t:" << h.e_sub1title();
	oss << wikidbg_html("page_info", &io_sfile->stats());

	// return no error
	return httpd_err_t::OK;
}


NEOIP_NAMESPACE_END







