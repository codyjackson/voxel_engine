#include "filesystem.h"

#include "../lib/simple_file_watcher/FileWatcher.h"

#include <boost/filesystem/fstream.hpp>
#include <Windows.h>

#include <unordered_map>
#include <unordered_set>

namespace
{
	class SingleFileWatcher : public FW::FileWatchListener
	{
	public:
		void handleFileAction(FW::WatchID watchid, const FW::String& dir, const FW::String& filename, FW::Action action)
		{
			if (action != FW::Action::Modified) {
				return;
			}

			auto path = dir + "/" + filename;
			auto iter = _filePathToCallback.find(path);
			if (iter == _filePathToCallback.end()) {
				return;
			}

			iter->second();
		}

		void watch(const boost::filesystem::path& filePath, const std::function<void ()>& callback)
		{
			auto iter = _filePathToCallback.find(filePath.string());
			if (iter != _filePathToCallback.end()) {
				throw Filesystem::FileWatcher::FailedToWatchFileException(filePath, "The path specified is already being watched.");
			}
			_filePathToCallback.insert(std::make_pair(filePath.string(), callback));

			auto directoryPath = get_directory_path(filePath);
			auto it = _directoryToNumberOfFilesWatched.find(directoryPath);
			if (it != _directoryToNumberOfFilesWatched.end()) {
				++it->second;
				return;
			}
			_directoryToNumberOfFilesWatched.insert(std::make_pair(directoryPath, 1));

			_fileWatcher.addWatch(directoryPath, this);
		}

		void stop_watching(const boost::filesystem::path& filePath)
		{
			auto iter = _filePathToCallback.find(filePath.string());
			if (iter == _filePathToCallback.end()) {
				return;
			}
			_filePathToCallback.erase(iter);

			auto directoryPath = get_directory_path(filePath);
			auto it = _directoryToNumberOfFilesWatched.find(directoryPath);
			if (--it->second == 0) {
				_fileWatcher.removeWatch(directoryPath);
			}
		}

		void update()
		{
			_fileWatcher.update();
		}

	private:

		static std::string get_directory_path(const boost::filesystem::path& filePath)
		{
			return filePath.parent_path().string();
		}

		FW::FileWatcher _fileWatcher;
		std::unordered_map<std::string, std::function<void()>> _filePathToCallback;
		std::unordered_map<std::string, int> _directoryToNumberOfFilesWatched;
	};

	SingleFileWatcher THE_FILE_WATCHER;
}

namespace Filesystem
{

	FileWatcher::FailedToWatchFileException::FailedToWatchFileException(const boost::filesystem::path& path, const std::string& why)
		: std::runtime_error("FailedToWatchFile: " + path.string() + " Why: " + why)
	{}


	FileWatcher::FileWatcher()
	{}

	FileWatcher::FileWatcher(const boost::filesystem::path& path, const std::function<void()>& callback)
		:_path(path)
	{
		if (!boost::filesystem::is_regular_file(_path)) {
			throw FailedToWatchFileException(path, "Path is not a file.");
			_path.clear();
		}

		THE_FILE_WATCHER.watch(path, callback);
	}

	FileWatcher::~FileWatcher()
	{
		if (!_path.empty()) {
			THE_FILE_WATCHER.stop_watching(_path);
		}
	}

	boost::filesystem::path FileWatcher::get_path() const
	{
		return _path;
	}

	void FileWatcher::update_all_watchers()
	{
		THE_FILE_WATCHER.update();
	}

	std::string get_file_contents(const boost::filesystem::path& path)
	{
		if (!boost::filesystem::exists(path)) {
			throw std::invalid_argument("The path specified doesn't exist:" + path.string());
		}
		boost::filesystem::ifstream stream(path);
		return std::string(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>());
	}
}
