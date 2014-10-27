#pragma once
#include "browser.h"

#include <memory>

namespace Browser
{
	class ProcessHandler : boost::noncopyable
	{
	public:
		class Scoped;

		static Scoped make();
		static void tick();
		static std::shared_ptr<Browser> create_browser(const JSValue& api, const Browser::PaintCallbackFunction& paintFn);

		~ProcessHandler();

	private:
		static std::shared_ptr<ProcessHandler> _instance;
		static void destroy();
		ProcessHandler();
	};

	class ProcessHandler::Scoped
	{
	public:
		Scoped();
		~Scoped();
	};
}