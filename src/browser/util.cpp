#include "util.h"

namespace Browser
{
	std::string Util::get_register_api_message_name()
	{
		static std::string name("ra");
		return  name;
	}
}
