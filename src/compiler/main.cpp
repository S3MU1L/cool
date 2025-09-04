#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

bool check_file(std::string file_name);
void report(std::string message);
void error(std::string message);

int main(int argc, char *argv[])
{
	if (argc != 2)
		error("Usage: coolc <filePath>\n");

	if (!check_file(argv[1]))
		error("Unsupported file extension, please provide a .cl file");

	std::ifstream istream(argv[1]);
	if (!istream)
		error("Unable to open file " + std::string(argv[1]));

	std::ostringstream buffer;
	buffer << istream.rdbuf();
	std::string content = buffer.str();
	std::cout << content;
	return 0;
}

bool check_file(std::string file_name)
{
	std::size_t last_dot = file_name.find('.');
	if (last_dot == std::string::npos)
		return false;

	std::string_view view{file_name.substr(last_dot, file_name.size())};
	return view == ".cl";
}

void report(std::string message)
{
	std::cerr << message << '\n';
}

void error(std::string message)
{
	report(message);
	exit(1);
}
