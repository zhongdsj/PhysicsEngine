# include <Config.h>
# include <fstream>
# include <ryml_all.hpp>

ZDSJ::Config::Config(const char* filename)
{
	std::ifstream input_stream(filename);
	if(!input_stream.is_open())
	{
		int a = 0;
	}
	std::string content((std::istreambuf_iterator<char>(input_stream)), \
	                    std::istreambuf_iterator<char>());
	ryml::Tree tree = ryml::parse_in_place(ryml::to_substr(content));
}

ZDSJ::Config::~Config()
{
}
