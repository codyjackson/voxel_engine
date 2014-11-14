#pragma once

#include <cef/cef_app.h>

namespace Browser
{
	class Deferred
	{
	public:
		Deferred();
		Deferred(CefRefPtr<CefV8Context> context);

		CefRefPtr<CefV8Value> get_promise() const;

		void resolve(const CefV8ValueList& args);
		void reject(const CefV8ValueList& args);

	private:
		void finish();
		CefRefPtr<CefV8Value> make_finish() const;
		CefRefPtr<CefV8Value> make_promise(CefRefPtr<CefV8Value> privateObject) const;
		CefRefPtr<CefV8Value> make_then(CefRefPtr<CefV8Value> privateObject) const;
		CefRefPtr<CefV8Value> make_finally(CefRefPtr<CefV8Value> privateObject) const;

		CefRefPtr<CefV8Context> _context;
		CefRefPtr<CefV8Value> _privateObject;
	};
}