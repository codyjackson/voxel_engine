#pragma once

#include <boost/filesystem.hpp>

namespace Constants
{
	struct RuntimeFilePaths
	{
		static boost::filesystem::path ui;
		static boost::filesystem::path ui_cache;
		static boost::filesystem::path ui_index;
		static std::string ui_localhost_path;
	};
}