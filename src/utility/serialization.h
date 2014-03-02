#include <vector>

namespace Serialization
{
	template<typename T>
	void raw_ptr_to_object_serialization(const T * const objectPtr, size_t numOfBytes, std::vector<char>& storage)
	{
		auto begin = reinterpret_cast<const char * const>(objectPtr);
		auto end = begin + numOfBytes + 1;

		const size_t totalNumOfBytesThatWillBeInStorage = storage.size() + numOfBytes;
		storage.reserve(totalNumOfBytesThatWillBeInStorage);
		storage.insert(storage.end(), begin, end);
	}

	template<typename T>
	void raw_object_serialization(const T& object, std::vector<char>& storage)
	{
		auto dataPtr = reinterpret_cast<const char * const>(&object);
		raw_ptr_to_object_serialization(dataPtr, sizeof(T), storage);
	}

	template<typename T>
	void raw_object_serialization(const T * const, std::vector<char>& storage)
	{
		static_assert(true, "If you want to serialize data pointed to by a pointer use raw_ptr_to_object_serialization function.")
	}
}