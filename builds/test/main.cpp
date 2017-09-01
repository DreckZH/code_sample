
#include "pugi_xml_parser.h"

int main(int argc, char* argv[])
{
	/* pugi_xml_parser test*/
	using namespace xml_parser_zxh;
	xml_parser *paser = new pugi_xml_parser();	
	paser->load_file("F:/personal/code_sample/cfgs/test.xml");
	std::map<string, str_str_map >* attrs = paser->get_all_attributes();

	return 0;
}