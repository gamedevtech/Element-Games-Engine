#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>

namespace EG{
	namespace Utility{
		namespace StringMethods{
			std::string RemoveCharacter(std::string in, char c);
			std::string RemoveSpecialCharactersFromPathString(std::string in);
			std::string GetFilenameFromPath(std::string path);
			std::vector<std::string> Tokenize(const std::string& string_to_parse, const std::string& delimiters = " ");
			float *ConvertStringToFloatArray(std::string float_string);
			std::string SearchAndReplace(const std::string &string_to_parse, const std::string &search_for, const std::string &replace_with);
		}
	}
}

