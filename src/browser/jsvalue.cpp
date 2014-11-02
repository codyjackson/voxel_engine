#include "jsvalue.h"

namespace 
{
	class MyV8Handler : public CefV8Handler {
	public:
		MyV8Handler(const std::function<void(CefRefPtr<CefV8Value>, const CefV8ValueList&)>& fn)
			:_fn(fn)
		{}

		virtual bool Execute(const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval, CefString& exception) OVERRIDE
		{
			try {
				_fn(object, arguments);
				return true;
			}
			catch (std::exception ex) {
				exception = ex.what();
				return false;
			}
		}

			// Provide the reference counting implementation for this class.
		IMPLEMENT_REFCOUNTING(MyV8Handler);

	private:
		std::function<void(CefRefPtr<CefV8Value>, const CefV8ValueList&)> _fn;
	};

	void to_js_value_helper(CefRefPtr<CefV8Value> val, JSValue& out)
	{
		if (val->IsArray()) {
			const int size = val->GetArrayLength();
			JSValue::Array a;
			a.reserve(size);
			for (int i = 0; i < size; ++i) {
				JSValue subVal;
				to_js_value_helper(val->GetValue(i), subVal);
			}
			out = a;
		}
		else if (val->IsObject()) {
			JSValue::Object o;
			std::vector<CefString> keys;
			val->GetKeys(keys);
			std::for_each(std::begin(keys), std::end(keys), [&o, &val](const CefString& key){
				to_js_value_helper(val->GetValue(key), o[key.ToString()]);
			});
			out = o;
		}
		else if (val->IsBool()) {
			out = JSValue(val->GetBoolValue());
		}
		else if (val->IsDouble()) {
			out = JSValue(val->GetDoubleValue());
		}
		else if (val->IsNull()) {
			out = JSValue::Null();
		}
		else if (val->IsString()) {
			out = JSValue(val->GetStringValue().ToString());
		}
		else {
			throw std::runtime_error("Type not supported.");
		}
	}

	CefRefPtr<CefV8Value> to_cef_v8_function(const JSValue::Function& fn);
	void to_cef_v8_value_helper(const JSValue& val, CefRefPtr<CefV8Value>& out)
	{
		if (val.is_array()) {
			const JSValue::Array& temp = static_cast<const JSValue::Array&>(val);
			CefRefPtr<CefV8Value> a = CefV8Value::CreateArray(temp.size());
			for (size_t i = 0; i < temp.size(); ++i) {
				CefRefPtr<CefV8Value> subValue;
				to_cef_v8_value_helper(val[i], subValue);
				a->SetValue(i, subValue);
			}
			out = a;
		}
		else if (val.is_object()) {
			const JSValue::Object& temp = static_cast<const JSValue::Object&>(val);
			CefRefPtr<CefV8Value> o = CefV8Value::CreateObject(NULL);
			std::for_each(std::begin(temp), std::end(temp), [&val, &o](const JSValue::Object::value_type& kvp){
				CefRefPtr<CefV8Value> subValue;
				const std::string& key = kvp.first;
				to_cef_v8_value_helper(val[key], subValue);
				o->SetValue(key, subValue, V8_PROPERTY_ATTRIBUTE_NONE);
			});
			out = o;
		}
		else if (val.is_bool()) {
			out = CefV8Value::CreateBool(static_cast<bool>(val));
		}
		else if (val.is_null()) {
			out = CefV8Value::CreateNull();
		}
		else if (val.is_string()) {
			out = CefV8Value::CreateString(static_cast<std::string>(val));
		}
		else if (val.is_function()) {
			out = to_cef_v8_function(val);
		}
		else {
			throw std::runtime_error("Type not supported.");
		}
	}


	CefRefPtr<CefV8Value> to_cef_v8_function(const JSValue::Function& fn)
	{
		auto wrappedFn = [&fn](CefRefPtr<CefV8Value> rawObject, const CefV8ValueList& rawArguments){
			JSValue object = JSValue::to_js_value(rawObject);
			JSValue::Array arguments;
			arguments.reserve(rawArguments.size());
			std::for_each(std::begin(rawArguments), std::end(rawArguments), [&arguments](CefRefPtr<CefV8Value> arg){
				arguments.push_back(JSValue::to_js_value(arg));
			});
			fn(arguments);
		};

		static CefString anonymous("anonymous");
		return CefV8Value::CreateFunction(anonymous, new MyV8Handler(wrappedFn));
	}
}
CefRefPtr<CefV8Value> JSValue::to_cef_v8_value(const JSValue& val)
{
	CefRefPtr<CefV8Value> out;
	to_cef_v8_value_helper(val, out);
	return out;
}

JSValue JSValue::to_js_value(CefRefPtr<CefV8Value> val)
{
	JSValue out;
	to_js_value_helper(val, out);
	return out;
}


JSValue::Function JSValue::wrap_void_function(const VoidFunction& fn)
{
	return [fn](const JSValue::Array& args) -> CefRefPtr<CefV8Value> {
		fn();
		return nullptr;
	};
}

JSValue::JSValue()
: Parent(Object())
{}

JSValue::JSValue(const std::string& s)
: Parent(s)
{}

JSValue::JSValue(const double& d)
: Parent(d)
{}

JSValue::JSValue(const Object& m)
: Parent(m)
{}

JSValue::JSValue(const Array& a)
: Parent(a)
{}

JSValue::JSValue(bool b)
: Parent(b)
{}

JSValue::JSValue(Null n)
: Parent(n)
{}

JSValue::JSValue(const Function& fn)
: Parent(fn)
{}

JSValue& JSValue::operator=(const std::string& rhs)
{
	Parent::operator=(rhs);
	return *this;
}

JSValue& JSValue::operator=(const double& rhs)
{
	Parent::operator=(rhs);
	return *this;
}

JSValue& JSValue::operator=(const Object& rhs)
{
	Parent::operator=(rhs);
	return *this;
}

JSValue& JSValue::operator=(const Array& rhs)
{
	Parent::operator=(rhs);
	return *this;
}

JSValue& JSValue::operator=(bool rhs)
{
	Parent::operator=(rhs);
	return *this;
}

JSValue& JSValue::operator=(Null rhs)
{
	Parent::operator=(rhs);
	return *this;
}

JSValue& JSValue::operator=(const Function& rhs)
{
	Parent::operator=(rhs);
	return *this;
}

JSValue::operator const std::string&() const
{
	return boost::get<const std::string&>(*this);
}

JSValue::operator const double&() const
{
	return boost::get<const double&>(*this);
}

JSValue::operator const JSValue::Object&() const
{
	return boost::get<const Object&>(*this);
}

JSValue::operator const JSValue::Array&() const
{
	return boost::get<const Array&>(*this);
}

JSValue::operator const bool&() const
{
	return boost::get<const bool&>(*this);
}

JSValue::operator const JSValue::Null&() const
{
	return boost::get<const Null&>(*this);
}

JSValue::operator const JSValue::Function&() const
{
	return boost::get<const Function&>(*this);
}

JSValue::operator std::string&()
{
	return boost::get<std::string&>(*this);
}
JSValue::operator double&()
{
	return boost::get<double&>(*this);
}
JSValue::operator Object&()
{
	return boost::get<Object&>(*this);
}
JSValue::operator Array&()
{
	return boost::get<Array&>(*this);
}
JSValue::operator bool&()
{
	return boost::get<bool&>(*this);
}
JSValue::operator Null&()
{
	return boost::get<Null&>(*this);
}

JSValue::operator Function&()
{
	return boost::get<Function&>(*this);
}

bool JSValue::is_string() const
{
	return boost::get<std::string>(this) != nullptr;
}

bool JSValue::is_double() const
{
	return boost::get<double>(this) != nullptr;
}

bool JSValue::is_object() const
{
	return boost::get<Object>(this) != nullptr;
}

bool JSValue::is_array() const
{
	return boost::get<Array>(this) != nullptr;
}

bool JSValue::is_bool() const
{
	return boost::get<bool>(this) != nullptr;
}

bool JSValue::is_null() const
{
	return boost::get<Null>(this) != nullptr;
}

bool JSValue::is_function() const
{
	return boost::get<Function>(this) != nullptr;
}

JSValue& JSValue::operator[](size_t n)
{
	if (!is_array()) {
		throw std::runtime_error("The JSValue is not an array.");
	}

	return static_cast<Array&>(*this)[n];
}

JSValue& JSValue::operator[](const std::string& key)
{
	if (!is_object()) {
		throw std::runtime_error("The JSValue is not an object.");
	}

	return static_cast<Object&>(*this)[key];
}

JSValue& JSValue::operator[](const char* key)
{
	return operator[](std::string(key));
}

const JSValue& JSValue::operator[](size_t n) const
{
	if (!is_array()) {
		throw std::runtime_error("The JSValue is not an array.");
	}

	return static_cast<const Array&>(*this)[n];
}

const JSValue& JSValue::operator[](const std::string& key) const
{
	if (!is_object()) {
		throw std::runtime_error("The JSValue is not an object.");
	}

	return static_cast<const Object&>(*this).at(key);
}

const JSValue& JSValue::operator[](const char* key) const
{
	return operator[](std::string(key));
}