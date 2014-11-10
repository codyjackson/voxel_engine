#include "util.h"

namespace Browser
{
	std::string Util::get_register_api_message_name()
	{
		static std::string name("ra");
		return  name;
	}

	std::string Util::get_ipc_function_string_placeholder()
	{
		static std::string placeholder("--##ipc_function_string_placeholder##--");
		return placeholder;
	}
}
