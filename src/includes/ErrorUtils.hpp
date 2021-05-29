//
// Created by pmerku on 28/05/2021.
//

#ifndef ERRORUTILS_HPP
#define ERRORUTILS_HPP

#include <iostream>
#include <cstdio>

#define ERROR_INFO __FILE__ << ":" << __LINE__ << " "

#ifdef DEBUG_THROW
# define ERROR_THROW_NO_ARG() do { \
	std::cerr << ERROR_INFO << std::endl; \
	throw; \
} while(0)

# define ERROR_THROW(x) do { \
	std::cerr << ERROR_INFO << std::endl; \
	throw x; \
} while(0)
#else
# define ERROR_THROW_NO_ARG() (throw)
# define ERROR_THROW(x) (throw x)
#endif

#endif //ERRORUTILS_HPP
