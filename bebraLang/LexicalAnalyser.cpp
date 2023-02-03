#include "LexicalAnalyser.h"
#include "SyntaxExpection.h"

std::vector<Lexeme> LexicalAnalyser::Analyse(const std::wstring& text) {
	std::vector<Lexeme> array;
	std::wstring buffer = L"";

	// efjern
	bool inString = false;
	bool inShortComment = false;
	bool inLongComment = false;

	for (uint32_t i = 0; i < text.size(); ++i) {
		// Checking if we are not in comment or in string
		if (inString) {
			if (text[i] == L'\n') {
				throw SyntaxExpection("Unexpexted end of line", i);
			}
			if (text[i] == L'\\' && text.size() - 1 > i) {
				if (text[i + 1] == L'n') buffer += L'\n';
				else if (text[i + 1] == L'\\') buffer += L'\\';
				else if (text[i + 1] == L'r') buffer += L'\r';
				else if (text[i + 1] == L'"') buffer += L'\"';
				else i -= 1;
				i += 1;
				continue;
			}
			if (text[i] == L'\"') {
				array.push_back(Lexeme(LexemeType::Other, buffer));
				array.push_back(Lexeme(LexemeType::Bracket, L"\""));
				buffer = L"";
				inString = false;
				continue;
			}
			buffer += text[i];
			continue;
		}
		if (inShortComment) {
			if (text[i] == L'\n') {
				inShortComment = false;
			}
			continue;
		}
		if (inLongComment) {
			if (text.size() - 1 > i) {
				if (text[i] == L'*' && text[i + 1] == L'/') {
					inLongComment = false;
				}
			}
			continue;
		}

		// String
		if (text[i] == L'"') {
			if (buffer != L"") {
				array.push_back(AnalyseWord(buffer, i));
				buffer = L"";
			}
			array.push_back(Lexeme(LexemeType::Bracket, L"\""));
			inString = true;
			continue;
		}
		// Comment
		if (text[i] == L'/') {
			if (text.size() - 1 > i) {
				if (text[i + 1] == L'/') {
					if (buffer != L"") {
						array.push_back(AnalyseWord(buffer, i));
						buffer = L"";
					}
					inShortComment = true;
					continue;
				}
				if (text[i + 1] == L'*') {
					if (buffer != L"") {
						array.push_back(AnalyseWord(buffer, i));
						buffer = L"";
					}
					inLongComment = true;
					continue;
				}
			}
		}

		// Space
		if (text[i] == L' ' || text[i] == L'\n' || text[i] == 9 || text[i] == L'\r') {
			if (buffer != L"") {
				array.push_back(AnalyseWord(buffer, i));
				buffer = L"";
			}
			continue;
		}
		// Brackets
		if (text[i] == L'(' || text[i] == L')' || text[i] == L'[' || text[i] == L']' || text[i] == L'{' || text[i] == L'}') {
			if (buffer != L"") {
				array.push_back(AnalyseWord(buffer, i));
				buffer = L"";
			}
			array.push_back(Lexeme(LexemeType::Bracket, std::wstring(text.begin() + i, text.begin() + i + 1)));
			continue;
		}

		// Operators
		// <<= << <= <
		if (text[i] == L'<') {
			if (buffer != L"") {
				array.push_back(AnalyseWord(buffer, i));
				buffer = L"";
			}
			if (text.size() - 1 > i) {
				if (text[i + 1] == L'<') {
					if (text.size() - 2 > i) {
						if (text[i + 1] == L'=') {
							array.push_back(Lexeme(LexemeType::Operator, L"<<="));
							i += 2;
							continue;
						}
					}
					array.push_back(Lexeme(LexemeType::Operator, L"<<"));
					i += 1;
					continue;
				}
				if (text[i + 1] == L'=') {
					array.push_back(Lexeme(LexemeType::Operator, L"<="));
					i += 1;
					continue;
				}
			}
			array.push_back(Lexeme(LexemeType::Operator, L"<"));
			continue;
		}
		// >>= >> >= >
		if (text[i] == L'>') {
			if (buffer != L"") {
				array.push_back(AnalyseWord(buffer, i));
				buffer = L"";
			}
			if (text.size() - 1 > i) {
				if (text[i + 1] == L'>') {
					if (text.size() - 2 > i) {
						if (text[i + 1] == L'=') {
							array.push_back(Lexeme(LexemeType::Operator, L">>="));
							i += 2;
							continue;
						}
					}
					array.push_back(Lexeme(LexemeType::Operator, L">>"));
					i += 1;
					continue;
				}
				if (text[i + 1] == L'=') {
					array.push_back(Lexeme(LexemeType::Operator, L">="));
					i += 1;
					continue;
				}
			}
			array.push_back(Lexeme(LexemeType::Operator, L">"));
			continue;
		}
		// += ++ +
		if (text[i] == L'+') {
			if (buffer != L"") {
				array.push_back(AnalyseWord(buffer, i));
				buffer = L"";
			}
			if (text.size() - 1 > i) {
				if (text[i + 1] == L'=') {
					array.push_back(Lexeme(LexemeType::Operator, L"+="));
					i += 1;
					continue;
				}
				if (text[i + 1] == L'+') {
					array.push_back(Lexeme(LexemeType::Operator, L"++"));
					i += 1;
					continue;
				}
			}
			array.push_back(Lexeme(LexemeType::Operator, L"+"));
			continue;
		}
		// -= -- -
		if (text[i] == L'-') {
			if (buffer != L"") {
				array.push_back(AnalyseWord(buffer, i));
				buffer = L"";
			}
			if (text.size() - 1 > i) {
				if (text[i + 1] == L'=') {
					array.push_back(Lexeme(LexemeType::Operator, L"-="));
					i += 1;
					continue;
				}
				if (text[i + 1] == L'+') {
					array.push_back(Lexeme(LexemeType::Operator, L"--"));
					i += 1;
					continue;
				}
			}
			array.push_back(Lexeme(LexemeType::Operator, L"-"));
			continue;
		}
		// **= ** *= *
		if (text[i] == L'*') {
			if (buffer != L"") {
				array.push_back(AnalyseWord(buffer, i));
				buffer = L"";
			}
			if (text.size() - 1 > i) {
				if (text[i + 1] == L'*') {
					if (text.size() - 2 > i) {
						if (text[i + 1] == L'=') {
							array.push_back(Lexeme(LexemeType::Operator, L"**="));
							i += 2;
							continue;
						}
					}
					array.push_back(Lexeme(LexemeType::Operator, L"**"));
					i += 1;
					continue;
				}
				if (text[i + 1] == L'=') {
					array.push_back(Lexeme(LexemeType::Operator, L"*="));
					i += 1;
					continue;
				}
			}
			array.push_back(Lexeme(LexemeType::Operator, L"*"));
			continue;
		}
		// /= /
		if (text[i] == L'/') {
			if (buffer != L"") {
				array.push_back(AnalyseWord(buffer, i));
				buffer = L"";
			}
			if (text.size() - 1 > i) {
				if (text[i + 1] == L'=') {
					array.push_back(Lexeme(LexemeType::Operator, L"/="));
					i += 1;
					continue;
				}
			}
			array.push_back(Lexeme(LexemeType::Operator, L"/"));
			continue;
		}
		// ^= ^
		if (text[i] == L'^') {
			if (buffer != L"") {
				array.push_back(AnalyseWord(buffer, i));
				buffer = L"";
			}
			if (text.size() - 1 > i) {
				if (text[i + 1] == L'=') {
					array.push_back(Lexeme(LexemeType::Operator, L"^="));
					i += 1;
					continue;
				}
			}
			array.push_back(Lexeme(LexemeType::Operator, L"^"));
			continue;
		}
		// &&= && &= &
		if (text[i] == L'&') {
			if (buffer != L"") {
				array.push_back(AnalyseWord(buffer, i));
				buffer = L"";
			}
			if (text.size() - 1 > i) {
				if (text[i + 1] == L'&') {
					if (text.size() - 2 > i) {
						if (text[i + 1] == L'=') {
							array.push_back(Lexeme(LexemeType::Operator, L"&&="));
							i += 2;
							continue;
						}
					}
					array.push_back(Lexeme(LexemeType::Operator, L"&&"));
					i += 1;
					continue;
				}
				if (text[i + 1] == L'=') {
					array.push_back(Lexeme(LexemeType::Operator, L"&="));
					i += 1;
					continue;
				}
			}
			array.push_back(Lexeme(LexemeType::Operator, L"&"));
			continue;
		}
		// ||= || |= |
		if (text[i] == L'|') {
			if (buffer != L"") {
				array.push_back(AnalyseWord(buffer, i));
				buffer = L"";
			}
			if (text.size() - 1 > i) {
				if (text[i + 1] == L'|') {
					if (text.size() - 2 > i) {
						if (text[i + 1] == L'=') {
							array.push_back(Lexeme(LexemeType::Operator, L"||="));
							i += 2;
							continue;
						}
					}
					array.push_back(Lexeme(LexemeType::Operator, L"||"));
					i += 1;
					continue;
				}
				if (text[i + 1] == L'=') {
					array.push_back(Lexeme(LexemeType::Operator, L"|="));
					i += 1;
					continue;
				}
			}
			array.push_back(Lexeme(LexemeType::Operator, L"|"));
			continue;
		}
		// %= %
		if (text[i] == L'%') {
			if (buffer != L"") {
				array.push_back(AnalyseWord(buffer, i));
				buffer = L"";
			}
			if (text.size() - 1 > i) {
				if (text[i + 1] == L'=') {
					array.push_back(Lexeme(LexemeType::Operator, L"%="));
					i += 1;
					continue;
				}
			}
			array.push_back(Lexeme(LexemeType::Operator, L"%"));
			continue;
		}
		// == => =
		if (text[i] == L'=') {
			if (buffer != L"") {
				array.push_back(AnalyseWord(buffer, i));
				buffer = L"";
			}
			if (text.size() - 1 > i) {
				if (text[i + 1] == L'=') {
					array.push_back(Lexeme(LexemeType::Operator, L"=="));
					i += 1;
					continue;
				}
				if (text[i + 1] == L'=>') {
					array.push_back(Lexeme(LexemeType::Operator, L"=>"));
					i += 1;
					continue;
				}
			}
			array.push_back(Lexeme(LexemeType::Operator, L"="));
			continue;
		}
		// != !
		if (text[i] == L'!') {
			if (buffer != L"") {
				array.push_back(AnalyseWord(buffer, i));
				buffer = L"";
			}
			if (text.size() - 1 > i) {
				if (text[i + 1] == L'=') {
					array.push_back(Lexeme(LexemeType::Operator, L"!="));
					i += 1;
					continue;
				}
			}
			array.push_back(Lexeme(LexemeType::Operator, L"!"));
			continue;
		}
		// Other
		if (text[i] == L'.' || text[i] == L',') {
			if (buffer != L"") {
				array.push_back(AnalyseWord(buffer, i));
				buffer = L"";
			}
			array.push_back(Lexeme(LexemeType::Operator, std::wstring(text.begin() + i, text.begin() + i + 1)));
			continue;
		}
		// Punctuation
		if (text[i] == L';') {
			if (buffer != L"") {
				array.push_back(AnalyseWord(buffer, i));
				buffer = L"";
			}
			array.push_back(Lexeme(LexemeType::Punctuation, std::wstring(text.begin() + i, text.begin() + i + 1)));
			continue;
		}
		buffer += text[i];
	}
	return array;
}

Lexeme LexicalAnalyser::AnalyseWord(const std::wstring& word, const uint64_t& pos) {
	// Keyword check
	for (uint32_t i = 0; i < _keywords.size(); ++i) {
		if (word == _keywords[i]) {
			return Lexeme(LexemeType::Keyword, word);
		}
	}

	LexemeType curType = LexemeType::Indefier;

	for (uint32_t i = 0; i < word.size(); ++i) {
		if (word[i] >= L'A' && word[i] <= L'Z' || word[i] >= L'a' && word[i] <= L'z' || word[i] == L'_') {
			if (curType != LexemeType::Indefier) {
				throw SyntaxExpection("Unexpected symbol", pos - word.size() + i);
			}
			continue;
		}
		if (word[i] >= L'0' && word[i] <= L'9') {
			if (i == 0) {
				curType = LexemeType::Number;
			}
			continue;
		}
		throw SyntaxExpection("Unexpected symbol", pos - word.size() + i);
	}
	return Lexeme(curType, word);
}