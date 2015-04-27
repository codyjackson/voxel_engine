#pragma once

#include <boost/filesystem.hpp>

namespace Filesystem
{
	std::string get_file_contents(const boost::filesystem::path& path);
}