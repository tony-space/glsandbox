#pragma once

#include <exception>
#include <stdexcept>
#include <string>
#include <sstream>

namespace libgl
{

namespace detail
{

inline void checkGLStatus(const GLenum status, const char* function, const char* file, const int line)
{
	if (status != GL_NO_ERROR)
	{
		std::stringstream errorStream;
		errorStream
			<< "glGetError error occured at "
			<< function << ' '
			<< file << ':'
			<< line << ' '
			<< "code: 0x"
			<< std::hex << status;
			
		throw std::runtime_error(errorStream.str());
	}
}

inline void checkGL(const char* function, const char* file, const int line)
{
	checkGLStatus(glGetError(), function, file, line);
}

}

#ifdef NDEBUG
#define checkGl()
#else
#define checkGl() libgl::detail::checkGL(__FUNCTION__, __FILE__, __LINE__);
#endif // NDEBUG


}
