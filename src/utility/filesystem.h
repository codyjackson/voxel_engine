#pragma once

#include <boost/filesystem.hpp>

#include <functional>
#include <thread>


namespace Filesystem
{
	class FileWatcher : boost::noncopyable
	{
	public:
		FileWatcher();
		FileWatcher(const boost::filesystem::path& path, const std::function<void()>& callback);
		~FileWatcher();

		static void update_all_watchers();

	private:
		boost::filesystem::path _path;

	public:
		struct FailedToWatchFileException : public std::runtime_error
		{
			FailedToWatchFileException(const boost::filesystem::path& path, const std::string& why);
		};
	};

	std::string get_file_contents(const boost::filesystem::path& path);	
}