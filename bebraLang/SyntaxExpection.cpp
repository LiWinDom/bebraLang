#include "SyntaxExpection.h"

std::string SyntaxExpection::what(const std::wstring& text) {
	uint64_t lineNumber = 0;
	uint64_t symbolNumber = 1;

	for (uint64_t i = 0; i < _symbolPos; ++i) {
		++symbolNumber;
		if (text[i] == L'\n') {
			++lineNumber;
			symbolNumber = 1;
		}
	}

	return std::string(std::exception::what()) + " at " + std::to_string(lineNumber) + ":" + std::to_string(symbolNumber);
}