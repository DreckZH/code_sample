
#ifndef __XML_PARSER_H_INCLUDED__
#define __XML_PARSER_H_INCLUDED__

/*  Version macros for compile-time API version detection                     */
#define XMLPSER_VERSION_MAJOR 1
#define XMLPSER_VERSION_MINOR 0
#define XMLPSER_VERSION_PATCH 0

#define XMLPSER_MAKE_VERSION(major, minor, patch) \
    ((major) * 10000 + (minor) * 100 + (patch))
#define XMLPSER_VERSION \
    XMLPSER_MAKE_VERSION(XMLPSER_VERSION_MAJOR, XMLPSER_VERSION_MINOR, XMLPSER_VERSION_PATCH)


#include "../../thirdparty/pugixml_1_8/pugixml.hpp"
#include <map>

namespace xml_parser_zxh
{

	typedef pugi::xml_node			node;
	typedef pugi::xml_document		document;
	typedef pugi::xml_tree_walker	walker;
	typedef pugi::xml_attribute		attribute;
	typedef pugi::xml_parse_result	parse_esult;
	typedef std::string					string;
	typedef std::map<string, string>	str_str_map;


	class xml_parser
	{
	public:
		explicit xml_parser();
		virtual ~xml_parser();

		virtual int load_file(const char* file);
		virtual void unload_file();
		virtual string set_path_tag(string);
		virtual str_str_map get_node_attributes(string nodename) = 0;
		virtual std::map<string, str_str_map >* get_all_attributes() = 0;
	protected:
		document * m_doc;
		walker* m_walker;
	};

	class pugi_xml_parser : public xml_parser
	{
	public:
		explicit pugi_xml_parser();
		~pugi_xml_parser();

		struct simple_walker : walker
		{
			bool for_each(node& node);
		};

		str_str_map get_node_attributes(string nodename);
		std::map<string, str_str_map >* get_all_attributes(){ return m_attrs; }
	private:
		static std::map<string, str_str_map >* m_attrs;
	};

}

#endif

