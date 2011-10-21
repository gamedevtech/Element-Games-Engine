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
		}
	}
}
