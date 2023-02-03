#include <iostream>
#include <fstream>
#include <locale>
#include <codecvt>

#include "LexicalAnalyser.h"
#include "SyntaxExpection.h"

std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

int main() {
	std::ifstream prog("prog.txt"), keywords("keywords.txt");

	if (!prog.is_open() || !keywords.is_open()) {
		std::cout << "cannot open";
		return 1;
	}

	std::wstring progText = L"";
	std::string line;
	while (getline(prog, line)) {
		progText += L'\n' + converter.from_bytes(line);
	}
	prog.close();

	std::vector<std::wstring> keywordsArray;
	while (getline(keywords, line)) {
		keywordsArray.push_back(converter.from_bytes(line));
	}
	keywords.close();

	LexicalAnalyser lexicalAnalyser(keywordsArray);

	std::vector<Lexeme> array;
	try {
		array = lexicalAnalyser.Analyse(progText);
	}
	catch (SyntaxExpection exp) {
		std::cout << exp.what(progText);
	}
	array;
}
