#include "application_handlers.h"

#include "cef/cef_runnable.h"
#include "util.h"

#include <algorithm>
#include <sstream>
#include <string>

namespace
{
	class FunctionHandler : public CefV8Handler
	{
	public:
		FunctionHandler(const JSValue::Function& fn)
			:_fn(fn)
		{}

		virtual bool Execute(const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval, CefString& exception) override
		{
			try {
				retval = _fn(object, arguments);
			}
			catch (const std::exception& ex) {
				exception = ex.what();
			}
			catch (...) {
				exception = "A non-std::exception was thrown.";
			}

			return true;
		}

		IMPLEMENT_REFCOUNTING(FunctionHandler);
	private:
		JSValue::Function _fn;
	};

	class Task : public CefTask
	{
	public:
		Task(const std::function<void()>& fn)
			:_fn(fn)
		{}

		void Execute() override
		{
			_fn();
		}

		IMPLEMENT_REFCOUNTING(Task);
	private:
		std::function<void()> _fn;
	};

	void register_api(const JSValue& value, const std::string& key, CefRefPtr<CefV8Value> node)
	{
		if (!node->IsObject()) {
			return;
		}

		if (value.is_object()) {
			const JSValue::Object& o = static_cast<const JSValue::Object&>(value);
			std::for_each(std::begin(o), std::end(o), [&node](const JSValue::Object::value_type& kvp){
				auto candidateNode = node->GetValue(kvp.first);
				if ((candidateNode.get() == nullptr) || !candidateNode->IsObject()) {
					node->SetValue(kvp.first, CefV8Value::CreateObject(nullptr), V8_PROPERTY_ATTRIBUTE_NONE);
					candidateNode = node->GetValue(kvp.first);
				}

				register_api(kvp.second, kvp.first, candidateNode);
			});
		}
		else if (value.is_function()) {
			const JSValue::Function& fn = static_cast<const JSValue&>(value);
			node->SetValue(key, CefV8Value::CreateFunction(key, new FunctionHandler(fn)), V8_PROPERTY_ATTRIBUTE_NONE);
		}
	}
}

ApplicationHandlers::ApplicationHandlers(const std::string& indexPath, const RectSize& viewportSize, const std::function<void(const RectSize& fullSize, const CefRenderHandler::RectList&, const void*)>& onPaint)
:_path(indexPath), _onPaint(onPaint), _viewportSize(viewportSize)
{}

void ApplicationHandlers::execute_javascript(const std::string& js)
{
	_frame->ExecuteJavaScript(js, _frame->GetURL(), 0);
}

void ApplicationHandlers::update_viewport_size(const RectSize& viewportSize)
{
	_viewportSize = viewportSize;
}

bool ApplicationHandlers::is_context_created() const
{
	return _rootContext.get() != nullptr;
}

void ApplicationHandlers::register_api(const JSValue& api)
{
	if (!is_context_created()) {
		_api = std::make_shared<JSValue>(api);
		return;
	}
	CefPostTask(TID_RENDERER, new Task(std::bind(&ApplicationHandlers::register_api_impl, this, api)));
}

CefRefPtr<CefRenderProcessHandler> ApplicationHandlers::GetRenderProcessHandler()
{
	return this;
}

void ApplicationHandlers::OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context)
{
	_rootContext = context;
	_frame = frame;
	if (_api) {
		register_api(*_api);
		_api.reset();
	}
}

CefRefPtr<CefBrowserProcessHandler> ApplicationHandlers::GetBrowserProcessHandler()
{
	return this;
}

void ApplicationHandlers::OnContextInitialized() {
  REQUIRE_UI_THREAD();

  CefWindowInfo windowInfo;

  windowInfo.SetAsOffScreen(nullptr);
  windowInfo.SetTransparentPainting(true);

  CefBrowserSettings browserSettings;

  // Create the first browser window.
  CefBrowserHost::CreateBrowser(windowInfo, this, _path, browserSettings, NULL);
}

CefRefPtr<CefLoadHandler> ApplicationHandlers::GetLoadHandler()
{
	return this;
}

CefRefPtr<CefRenderHandler> ApplicationHandlers::GetRenderHandler()
{
	return this;
}

void ApplicationHandlers::OnLoadError(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, ErrorCode errorCode, const CefString& errorText, const CefString& failedUrl)
{
	REQUIRE_UI_THREAD();

	// Don't display an error for downloaded files.
	if (errorCode == ERR_ABORTED) {
		return;
	}

	// Display a load error message.
	std::stringstream ss;
	ss << "<html><body bgcolor=\"white\">"
		<< "<h2>Failed to load UI</h2> "
		<< "<h3>Error: "
		<< std::string(errorText)
		<< " (" << errorCode
		<< ")</body></html>";
	frame->LoadString(ss.str(), failedUrl);
}

bool ApplicationHandlers::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect)
{
	rect = CefRect(0, 0, _viewportSize.width, _viewportSize.height);
	return true;
}

void ApplicationHandlers::OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList &dirtyRects, const void *buffer, int width, int height)
{
	_onPaint(RectSize(width, height), dirtyRects, buffer);
}

void ApplicationHandlers::CloseAllBrowsers(bool forceClose)
{
	if (!CefCurrentlyOn(TID_UI)) {
		CefPostTask(TID_UI, NewCefRunnableMethod(this, &ApplicationHandlers::CloseAllBrowsers, forceClose));
		return;
	}

	if (_browserList.empty()) {
		return;
	}

	std::for_each(std::begin(_browserList), std::end(_browserList), [forceClose](CefRefPtr<CefBrowser> browser){
		browser->GetHost()->CloseBrowser(forceClose);
	});
}

void ApplicationHandlers::register_api_impl(JSValue api)
{
	_rootContext->Enter();
	::register_api(api, "", _rootContext->GetGlobal());
	execute_javascript("if(onApiRegistered){onApiRegistered();}");
	_rootContext->Exit();
}