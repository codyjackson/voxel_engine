#pragma once

#include <boost/variant.hpp>
#include <cef/cef_render_handler.h>
#include <cef/cef_v8.h>

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

struct JSNullRaw
{};

typedef std::function<CefRefPtr<CefV8Value>(CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments)> JSRawFunction;

class JSValue : public boost::variant<std::string, double, std::unordered_map<std::string, JSValue>, std::vector<JSValue>, bool, JSNullRaw, std::function<CefRefPtr<CefV8Value> (const std::vector<JSValue>&) >>
{
public:
	typedef std::unordered_map<std::string, JSValue> Object;
	typedef std::vector<JSValue> Array;
	typedef JSNullRaw Null;
	typedef std::function<CefRefPtr<CefV8Value> (const JSValue::Array& arguments)> Function;
	typedef std::function<void ()> VoidFunction;
	typedef boost::variant<std::string, double, Object, Array, bool, Null, Function> Parent;

	static CefRefPtr<CefV8Value> to_cef_v8_value(const JSValue& val);
	static JSValue to_js_value(CefRefPtr<CefV8Value> val);
	static Function wrap_void_function(const VoidFunction& fn);

	JSValue();
	JSValue(const std::string& s);
	JSValue(const double& d);
	JSValue(const Object& o);
	JSValue(const Array& a);
	JSValue(bool b);
	JSValue(Null n);
	JSValue(const Function& fn);

	JSValue& operator=(const std::string& rhs);
	JSValue& operator=(const double& rhs);
	JSValue& operator=(const Object& rhs);
	JSValue& operator=(const Array& rhs);
	JSValue& operator=(bool rhs);
	JSValue& operator=(Null rhs);
	JSValue& operator=(const Function& rhs);

	operator const std::string&() const;
	operator const double&() const;
	operator const Object&() const;
	operator const Array&() const;
	operator const bool&() const;
	operator const Null&() const;
	operator const Function&() const;

	operator std::string&();
	operator double&();
	operator Object&();
	operator Array&();
	operator bool&();
	operator Null&();
	operator Function&();

	bool is_string() const;
	bool is_double() const;
	bool is_object() const;
	bool is_array() const;
	bool is_bool() const;
	bool is_null() const;
	bool is_function() const;

	JSValue& operator[](size_t n);
	JSValue& operator[](const std::string& key);
	JSValue& operator[](const char* key);

	const JSValue& operator[](size_t n) const;
	const JSValue& operator[](const std::string& key) const;
	const JSValue& operator[](const char* key) const;
};