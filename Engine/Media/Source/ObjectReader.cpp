#include "../ObjectReader.h"
#include <fstream>

namespace EG{
	namespace Media{
		ObjectReader::ObjectReader(void){
			object = NULL;
		}

		ObjectReader::~ObjectReader(void){
			//
		}

		bool ObjectReader::Read(std::string file_path){
			std::ifstream in;
			in.open(file_path.c_str());

			std::string line;
			std::getline(in, line);

			object = new EG::Game::Object(line);

			in.close();
		}

		EG::Game::Object *ObjectReader::GetObject(void){
			return object;
		}
	}
}

