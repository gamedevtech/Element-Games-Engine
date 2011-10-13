#include "../AssimpInterface.h"

namespace EG{
	namespace Media{
		AssimpInterface::AssimpInterface(void){
			//
		}

		AssimpInterface::~AssimpInterface(void){
			//delete scene;
		}

		bool AssimpInterface::Load(std::string file_path){
			Assimp::Importer importer;
			scene = importer.ReadFile(file_path, aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);
			if (!scene){
				return false;
			}

			// Scene Processing

			return true;
		}
	}
}
