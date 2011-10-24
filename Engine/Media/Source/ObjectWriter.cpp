#include "../ObjectWriter.h"

#include <fstream>

namespace EG{
	namespace Media{
		ObjectWriter::ObjectWriter(EG::Game::Object *_object){
			object = _object;
		}
		ObjectWriter::~ObjectWriter(void){
			//
		}

		void ObjectWriter::Write(std::string file_path){
			std::ofstream out;
			out.open(file_path.c_str());

			out << object->GetObjectName() << std::endl;

			out.close();
		}
	}
}
