#include "render_process_handler.h"

#include "util.h"

namespace 
{
	class ContextOpener
	{
	public:
		ContextOpener(CefRefPtr<CefV8Context> context)
			:_context(context)
		{
			_context->Enter();
		}

		~ContextOpener()
		{
			_context->Exit();
		}

		CefRefPtr<CefV8Context> get() const
		{
			return _context;
		}

	private:
		CefRefPtr<CefV8Context> _context;
	};
}

namespace Browser
{
	RenderProcessHandler::RenderProcessHandler()
	{
		_messageHandlers[Util::get_register_api_message_name()] = std::bind(&RenderProcessHandler::on_register_api, this, std::placeholders::_1, std::placeholders::_2);
	}

	bool RenderProcessHandler::OnProcessMessageReceived(CefRefPtr< CefBrowser > browser, CefProcessId source_process, CefRefPtr< CefProcessMessage > message)
	{
		const std::string name = message->GetName();
		auto messageHandlerIter = _messageHandlers.find(name);
		if (messageHandlerIter == std::end(_messageHandlers)) {
			throw std::runtime_error("Could not find an appropriate message handler.");
		}

		messageHandlerIter->second(browser, message->GetArgumentList());
		return true;
	}

	void RenderProcessHandler::on_register_api(CefRefPtr< CefBrowser > browser, CefRefPtr< CefListValue > arguments)
	{
		ContextOpener context(browser->GetMainFrame()->GetV8Context());
		CefRefPtr<CefV8Value> global = context.get()->GetGlobal();
		CefRefPtr<CefV8Value> val = CefV8Value::CreateString("test2");
		global->SetValue(CefString("test"), val, CefV8Value::PropertyAttribute::V8_PROPERTY_ATTRIBUTE_NONE);
	}
}

