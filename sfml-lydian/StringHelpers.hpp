#ifndef STRINGHELPERS_HPP
#define STRINGHELPERS_HPP

#include <sstream>			// string stream

// convert to string
template <typename Type>
std::string toString(const Type& value);

// include inine file here
#include "StringHelpers.inl"
#endif