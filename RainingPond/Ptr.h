#ifndef PTR_H_
#define PTR_H_
#include <memory>

namespace Basic{
	template<typename T>
	using Ptr = std::shared_ptr<T>;
	
	template<typename T>
	using uPtr = std::unique_ptr<T>;
}

#endif // !PTR_H_
