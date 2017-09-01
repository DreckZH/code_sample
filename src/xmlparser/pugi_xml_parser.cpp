
#include "pugi_xml_parser.h"

namespace xml_parser_zxh 
{
	static string _path_tag("id");

	xml_parser::xml_parser() :m_doc(0), m_walker(0)	{}
	xml_parser::~xml_parser()	{
		delete m_doc;
		m_doc = 0;
	}

	string xml_parser::set_path_tag(string tag)
	{
		return _path_tag = tag;
	}

	int xml_parser::load_file(const char* file)
	{
		m_doc = new document();
		if (!m_doc)
			return -1;
		parse_esult result = m_doc->load_file(file);
		if (result.status == pugi::status_ok)	{
			if (m_walker)
				m_doc->traverse(*m_walker);
		}
		return result.status;
	}

	void xml_parser::unload_file()
	{
		delete m_doc;
		m_doc = 0;
	}


	std::map<string, str_str_map >* pugi_xml_parser::m_attrs  = 0;

	pugi_xml_parser::pugi_xml_parser()
	{
		m_attrs = new std::map <string, str_str_map> ;
		m_walker = new simple_walker;
	}

	pugi_xml_parser::~pugi_xml_parser()
	{
		m_attrs->clear();
		delete m_attrs; 
		m_attrs = 0;
		delete m_walker;
		m_walker = 0;
	}

	bool pugi_xml_parser::simple_walker::for_each(pugi::xml_node& node)
	{
		for (int i = 0; i <= this->depth(); ++i)
		{
			str_str_map row_attrs;
			for (attribute attr = node.first_attribute();
				attr;
				attr = attr.next_attribute())
			{				
				row_attrs[attr.name()] = attr.value();
			}
			string path = node.path() + row_attrs[_path_tag];
			(*(pugi_xml_parser::m_attrs))[path] = row_attrs;
		}
		return true;
	}

	str_str_map pugi_xml_parser::get_node_attributes(string nodename)
	{
		return (*m_attrs)[nodename];
	}
}



/* pugi_xml_parser test
using namespace xml_parser_zxh;
xml_parser *paser = new pugi_xml_parser();
paser->load_file("F:/personal/code_sample/cfgs/test.xml");
std::map<string, str_str_map >* attrs = paser->get_all_attributes();
*/
