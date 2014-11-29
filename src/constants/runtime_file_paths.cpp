#include "runtime_file_paths.h"

namespace Constants
{
	boost::filesystem::path RuntimeFilePaths::ui(boost::filesystem::current_path() / "ui");
	boost::filesystem::path RuntimeFilePaths::ui_cache(ui / "cache");
	boost::filesystem::path RuntimeFilePaths::ui_index(ui / "index.html");
	std::string RuntimeFilePaths::ui_localhost_path("http://localhost:3000");
}