#pragma once
#include <string>
#include <vector>

namespace LexicalAnalyzer
{
	enum Type { Keyword, Indefier, Number, Bracket, Operator, Punctuation, Other };

	std::vector<Type> Analyze(const std::wstring&, const std::vector<std::wstring>&);

	Type AnalyzeWord(const std::wstring&, const std::vector<std::wstring>&);
};

