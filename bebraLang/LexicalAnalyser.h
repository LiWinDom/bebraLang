#pragma once
#include <vector>

#include "Lexeme.h"

class LexicalAnalyser
{
public:
	LexicalAnalyser(const std::vector<std::wstring>& keywords) : _keywords(keywords) {};


	std::vector<Lexeme> Analyse(const std::wstring&);

	Lexeme AnalyseWord(const std::wstring&, const uint64_t&);

private:
	std::vector<std::wstring> _keywords;
};

