#include "filesystem.h"

#include <boost/filesystem/fstream.hpp>

namespace Filesystem
{
	std::string get_file_contents(const boost::filesystem::path& path)
	{
		if (!boost::filesystem::exists(path)) {
			throw std::invalid_argument("The path specified doesn't exist:" + path.string());
		}
		boost::filesystem::ifstream stream(path);
		return std::string(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>());
	}
}
