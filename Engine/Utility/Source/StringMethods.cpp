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
		}
	}
}
