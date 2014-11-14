#include "deferred.h"

#include "util.h"

#include <functional>

namespace
{
	class PassThroughHandler : public CefV8Handler {
	public:
		typedef std::function<bool (const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval, CefString& exception)> Function;
		PassThroughHandler(const Function& fn)
			:_fn(fn)
		{}

		virtual bool Execute(const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval, CefString& exception) OVERRIDE
		{
			return _fn(name, object, arguments, retval, exception);
		}

			// Provide the reference counting implementation for this class.
		IMPLEMENT_REFCOUNTING(MyV8Handler);

	private:
		Function _fn;
	};

	CefRefPtr<CefV8Value> args_to_value(const CefV8ValueList& args)
	{
		CefRefPtr<CefV8Value> value = CefV8Value::CreateArray(args.size());
		for (size_t i = 0; i < args.size(); ++i) {
			value->SetValue(i, args[i]);
		}

		return value;
	}

	CefV8ValueList value_to_args(CefRefPtr<CefV8Value> value)
	{
		if (!value->IsArray()) {
			throw std::runtime_error("Can only convert an array to arguments.");
		}

		CefV8ValueList args;
		args.reserve(value->GetArrayLength());
		for (size_t i = 0; i < args.capacity(); ++i) {
			args.push_back(value->GetValue(i));
		}

		return args;
	}

	CefString get_resolved_arguments_key()
	{
		static CefString key("resolvedArguments");
		return key;
	}

	CefString get_rejected_arguments_key()
	{
		static CefString key("rejectedArguments");
		return key;
	}

	CefString get_finish_function_key()
	{
		static CefString key("finish");
		return key;
	}

	CefString get_success_function_key()
	{
		static CefString key("success");
		return key;
	}

	CefString get_failure_function_key()
	{
		static CefString key("failure");
		return key;
	}

	CefString get_finally_function_key()
	{
		static CefString key("finally");
		return key;
	}

	void finish(CefRefPtr<CefV8Value> privateObject)
	{
		privateObject->GetValue(get_finish_function_key())->ExecuteFunction(privateObject, CefV8ValueList());
	}
}

Browser::Deferred::Deferred()
{}

Browser::Deferred::Deferred(CefRefPtr<CefV8Context> context)
:_context(context), _privateObject(CefV8Value::CreateObject(nullptr))
{
	_privateObject->SetValue(get_finish_function_key(), make_finish(), CefV8Value::PropertyAttribute::V8_PROPERTY_ATTRIBUTE_NONE);
}

CefRefPtr<CefV8Value> Browser::Deferred::get_promise() const
{
	return make_promise(_privateObject);
}

void Browser::Deferred::resolve(const CefV8ValueList& args)
{
	Util::ContextOpener opener(_context);
	_privateObject->SetValue(get_resolved_arguments_key(), args_to_value(args), CefV8Value::PropertyAttribute::V8_PROPERTY_ATTRIBUTE_NONE);
}

void Browser::Deferred::reject(const CefV8ValueList& args)
{
	Util::ContextOpener opener(_context);
	_privateObject->SetValue(get_rejected_arguments_key(), args_to_value(args), CefV8Value::PropertyAttribute::V8_PROPERTY_ATTRIBUTE_NONE);
}

void Browser::Deferred::finish()
{
	Util::ContextOpener opener(_context);
	::finish(_privateObject);
}

CefRefPtr<CefV8Value> Browser::Deferred::make_finish() const
{
	return CefV8Value::CreateFunction(get_finish_function_key(), new PassThroughHandler([](const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval, CefString& exception){
		auto resolvedArgs = object->GetValue(get_resolved_arguments_key());
		auto rejectedArgs = object->GetValue(get_rejected_arguments_key());
		auto successFn = object->GetValue(get_success_function_key());
		auto failureFn = object->GetValue(get_failure_function_key());
		auto finallyFn = object->GetValue(get_finally_function_key());

		if (resolvedArgs->IsArray() && successFn->IsFunction()) {
			successFn->ExecuteFunction(nullptr, value_to_args(resolvedArgs));
		} else if (rejectedArgs->IsArray() && failureFn->IsFunction()) {
			failureFn->ExecuteFunction(nullptr, value_to_args(rejectedArgs));
		}

		if ((resolvedArgs->IsArray() || rejectedArgs->IsArray()) && finallyFn->IsFunction()) {
			finallyFn->ExecuteFunction(nullptr, CefV8ValueList());
		}

		return true;
	}));
}

CefRefPtr<CefV8Value> Browser::Deferred::make_promise(CefRefPtr<CefV8Value> privateObject) const
{
	CefRefPtr<CefV8Value> promise = CefV8Value::CreateObject(nullptr);
	promise->SetValue("then", make_then(privateObject), CefV8Value::PropertyAttribute::V8_PROPERTY_ATTRIBUTE_DONTDELETE);
	promise->SetValue("finally", make_finally(privateObject), CefV8Value::PropertyAttribute::V8_PROPERTY_ATTRIBUTE_DONTDELETE);
	return promise;
}

CefRefPtr<CefV8Value> Browser::Deferred::make_then(CefRefPtr<CefV8Value> privateObject) const
{
	return CefV8Value::CreateFunction("then", new PassThroughHandler([privateObject](const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval, CefString& exception){
		if ((arguments.size() < 1) || !arguments[0]->IsFunction()) {
			exception = "The first argument must be the success callback.";
			return true;
		}
		if ((arguments.size() > 1) && !arguments[1]->IsFunction()) {
			exception = "The first argument must be a failure callback if used.";
			return true;
		}
		if ((arguments.size() > 2) && !arguments[2]->IsFunction()) {
			exception = "The first argument must be a failure callback if used.";
			return true;
		}

		privateObject->SetValue(get_success_function_key(), arguments[0], CefV8Value::PropertyAttribute::V8_PROPERTY_ATTRIBUTE_NONE);
		if (arguments.size() > 1) {
			privateObject->SetValue(get_failure_function_key(), arguments[1], CefV8Value::PropertyAttribute::V8_PROPERTY_ATTRIBUTE_NONE);
		}
		if (arguments.size() > 2) {
			privateObject->SetValue(get_finally_function_key(), arguments[2], CefV8Value::PropertyAttribute::V8_PROPERTY_ATTRIBUTE_NONE);
		}

		::finish(privateObject);
		return true;
	}));
}

CefRefPtr<CefV8Value> Browser::Deferred::make_finally(CefRefPtr<CefV8Value> privateObject) const
{
	return CefV8Value::CreateFunction("finally", new PassThroughHandler([privateObject](const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval, CefString& exception){
		if ((arguments.size() != 1) || !arguments[0]->IsFunction()) {
			exception = "The first argument must be the finally callback.";
			return true;
		}

		privateObject->SetValue("finally", arguments[0], CefV8Value::PropertyAttribute::V8_PROPERTY_ATTRIBUTE_NONE);
		return true;
	}));
}