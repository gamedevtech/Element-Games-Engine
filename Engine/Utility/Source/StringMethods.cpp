#include "../StringMethods.h"

namespace EG{
	namespace Utility{
		namespace StringMethods{
			std::string RemoveCharacter(std::string in, char c){
				std::string out = "";

				for (unsigned int i = 0; i < in.size(); i++){
					if (in[i] != c){
						out += in[i];
					}
				}

				return out;
			}

			std::string RemoveSpecialCharactersFromPathString(std::string in){
				std::string out;
				out = EG::Utility::StringMethods::RemoveCharacter(in, '\n');
				out = EG::Utility::StringMethods::RemoveCharacter(out, '\0');
				out = EG::Utility::StringMethods::RemoveCharacter(out, '\r');
				out = EG::Utility::StringMethods::RemoveCharacter(out, '\t');
				out = EG::Utility::StringMethods::RemoveCharacter(out, char(int(8)));
				return out;
			}

			std::string GetFilenameFromPath(std::string path){
				unsigned int pos = path.find_last_of('/');
				if (pos != std::string::npos){
					return path.substr(pos + 1);
				}
				return "";
			}

			float *ConvertStringToFloatArray(std::string float_string){
				std::vector<std::string> float_array_string_list = Tokenize(float_string);
				unsigned int count = float_array_string_list.size();
				float *out = new float[count];
				for (unsigned int i = 0; i < count; i++){
					out[i] = atof(float_array_string_list[i].c_str());
				}
				return out;
			}

			std::vector<std::string> Tokenize(const std::string& string_to_parse, const std::string& delimiters){
				std::string::size_type lastPos = string_to_parse.find_first_not_of(delimiters, 0);
				std::string::size_type pos = string_to_parse.find_first_of(delimiters, lastPos);
				std::vector<std::string> tokens;
				while (std::string::npos != pos || std::string::npos != lastPos){
					tokens.push_back(string_to_parse.substr(lastPos, pos - lastPos));
					// Skip delimiters.  Note the "not_of"
					lastPos = string_to_parse.find_first_not_of(delimiters, pos);
					// Find next "non-delimiter"
					pos = string_to_parse.find_first_of(delimiters, lastPos);
				}
				return tokens;
			}

			std::string SearchAndReplace(const std::string &string_to_parse, const std::string &search_for, const std::string &replace_with){
				std::string output = string_to_parse;
				std::string::size_type found = output.find(search_for);
				while (found != std::string::npos){
					output.replace(found, search_for.length(), replace_with);
					found = output.find(search_for);
				}
				return output;
			}
		}
	}
}
