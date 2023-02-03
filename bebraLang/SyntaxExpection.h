#pragma once
#include <exception>
#include <string>

class SyntaxExpection : public std::exception
{
public:
	SyntaxExpection(char const* const& message, const uint64_t& symbolPos) :
		exception(message), _symbolPos(symbolPos) {}


	std::string what(const std::wstring&);

private:
	uint64_t _symbolPos;
};

