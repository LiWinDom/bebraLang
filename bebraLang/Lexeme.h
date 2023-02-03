#pragma once
#include <string>

enum class LexemeType { Keyword, Indefier, Number, Bracket, Operator, Punctuation, Other };

class Lexeme
{
public:
	Lexeme(const LexemeType& type, const std::wstring& string) :
		_type(type), _string(string) {}


	LexemeType getType() {
		return _type;
	}

	std::wstring getString() {
		return _string;
	}

private:
	LexemeType _type;
	std::wstring _string;
};

