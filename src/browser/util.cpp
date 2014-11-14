#include "util.h"

namespace Browser
{
	std::string Util::get_register_api_message_name()
	{
		static std::string name("ra");
		return  name;
	}

	std::string Util::get_reject_promise_message_name()
	{
		static std::string name("rejp");
		return name;
	}

	std::string Util::get_resolve_promise_message_name()
	{
		static std::string name("resp");
		return name;
	}

	std::string Util::get_ipc_function_string_placeholder()
	{
		static std::string placeholder("--##ipc_function_string_placeholder##--");
		return placeholder;
	}


	Util::ContextOpener::ContextOpener(CefRefPtr<CefV8Context> context)
		:_context(context)
	{
		_context->Enter();
	}

	Util::ContextOpener::~ContextOpener()
	{
		_context->Exit();
	}

	CefRefPtr<CefV8Context> Util::ContextOpener::get() const
	{
		return _context;
	}
}
