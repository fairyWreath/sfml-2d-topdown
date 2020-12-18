template <typename Type>
std::string toString(const Type& value)
{
	// use stringstream to convert to string
	std::stringstream stream;
	stream << value;
	return stream.str();
}