#include "LexicalAnalyzer.h"

std::vector<LexicalAnalyzer::Type> LexicalAnalyzer::Analyze(const std::wstring& text, const std::vector<std::wstring>& keywords) {
	std::vector<LexicalAnalyzer::Type> array(0, Other);
	std::wstring buffer = L"";

	for (uint32_t i = 0; i < text.size(); ++i) {
		// Space
		if (text[i] == L' ') {
			array.push_back(AnalyzeWord(buffer, keywords));
			buffer = L"";
			continue;
		}
		// Brackets
		if (text[i] == L'(' || text[i] == L')' || text[i] == L'[' || text[i] == L']' || text[i] == L'{' || text[i] == L'}') {
			array.push_back(AnalyzeWord(buffer, keywords));
			array.push_back(Bracket);
			buffer = L"";
			continue;
		}
		// Operators
		if (text[i] == L'+' || text[i] == L'-' || text[i] == L'/' || text[i] == L'%' || text[i] == L'//' || text[i] == L'*' || text[i] == L'^' ||
			text[i] == L'&' || text[i] == L'|' || text[i] == L'!' ||
			text[i] == L'&&' || text[i] == L'||') {
			array.push_back(AnalyzeWord(buffer, keywords));
			array.push_back(Operator);
			buffer = L"";
			continue;
		}
		// Punctuation
		if (text[i] == L',' || text[i] == L';') {
			array.push_back(AnalyzeWord(buffer, keywords));
			array.push_back(Punctuation);
			buffer = L"";
			continue;
		}
		buffer += text[i];
	}
	return array;
}

LexicalAnalyzer::Type LexicalAnalyzer::AnalyzeWord(const std::wstring& word, const std::vector<std::wstring>& keywords) {
	// Keyword check
	for (uint32_t i = 0; i < keywords.size(); ++i) {
		if (word == keywords[i]) {
			return Keyword;
		}
	}

	LexicalAnalyzer::Type curType = Indefier;

	for (uint32_t i = 0; i < word.size(); ++i) {
		if (word[i] >= L'A' && word[i] <= L'Z' || word[i] >= L'a' && word[i] <= L'z' || word[i] == L'_') {
			if (curType != Indefier) {
				curType = Other;
			}
		}
		else if (word[i] >= L'0' && word[i] <= L'9') {
			if (i == 0) {
				curType = Number;
			}
		}
	}
	return curType;
}