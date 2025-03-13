# include <Config.h>
# include <fstream>
# include <ryml_all.hpp>

ZDSJ::Config::Config(const char* _filename): m_file_name(_filename)
{
	std::ifstream input_stream(this->m_file_name);
	if(!input_stream.is_open())
	{
		int a = 0;
	}
	this->m_buffer = std::string((std::istreambuf_iterator<char>(input_stream)), \
	                             std::istreambuf_iterator<char>());
	this->m_tree =  ryml::parse_in_place(ryml::to_substr(this->m_buffer));
	input_stream.close();
}

ZDSJ::Config* ZDSJ::Config::getInstance()
{
	static ZDSJ::Config instance("./config.json");
	return &instance;
}

ryml::NodeRef ZDSJ::Config::root()
{
	return this->m_tree.rootref();
}

ZDSJ::Config::~Config()
{
	std::ofstream output_stream(this->m_file_name, std::ios::out);
	output_stream << ryml::emitrs_json<std::string>(this->m_tree);
}
