#include "jsvalue.h"

JSValue::Function JSValue::wrap_void_function(const VoidFunction& fn)
{
	return [fn](CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments) -> CefRefPtr<CefV8Value> {
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