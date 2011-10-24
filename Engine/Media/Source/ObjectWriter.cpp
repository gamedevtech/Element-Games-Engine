#include "../ObjectWriter.h"

#include "../../Game/ObjectRenderingAttribute.h"
#include "../../Game/ObjectBasicAttribute.h"
#include "../../Game/ObjectEmissionAttribute.h"
#include "../../Graphics/Mesh.h"
#include "../../Graphics/RenderingMaterial.h"

#include <fstream>

namespace EG{
	namespace Media{
		ObjectWriter::ObjectWriter(EG::Game::Object *_object, EG::Game::Scene *_scene, std::string _object_name){
			scene = _scene;
			object = _object;
			object_name = _object_name;
		}
		ObjectWriter::~ObjectWriter(void){
			//
		}

		void ObjectWriter::Write(std::string file_path){
			std::ofstream out;
			out.open(file_path.c_str());

			out << object_name << std::endl;

			// Transformation Attribute
			std::vector<EG::Game::ObjectAttribute *> *transformation_attributes = object->GetAttributesByType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_BASIC_TRANSFORMATION);
			EG::Game::ObjectAttributeBasicTransformation *transformation_attribute = static_cast<EG::Game::ObjectAttributeBasicTransformation *>(transformation_attributes->at(0));
			glm::mat4 transformation = transformation_attribute->GetTransformation();
			for (unsigned int i = 0; i < 4; i++){
				for (unsigned int j = 0; j < 4; j++){
					out << transformation[i][j];
					if (i != 3 || j != 3){
						out << ' ';
					}
				}
			}
			out << std::endl;

			// Mesh/Material Attributes
			std::vector<EG::Game::ObjectAttribute *> *mesh_attributes = object->GetAttributesByType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_RENDERING_MESH);
			std::vector<EG::Game::ObjectAttribute *>::iterator mesh_attribute_iterator = mesh_attributes->begin();
			while (mesh_attribute_iterator != mesh_attributes->end()){
				EG::Game::ObjectAttributeRenderingMesh *mesh_attribute = static_cast<EG::Game::ObjectAttributeRenderingMesh *>(*mesh_attribute_iterator);
				EG::Graphics::RenderingMaterial *material = mesh_attribute->GetMaterial();

				if (material->HasTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_DECAL)){
					//graphics->BindTexture(scene->GetTextureManager()->GetTexture(material->GetTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_DECAL))->GetId());
				}else if (material->HasCubeMap(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_HEIGHT)){
					//graphics->BindCubeMap(scene->GetTextureManager()->GetCubeMap(material->GetTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_DECAL))->GetId());
				}
				if (material->HasTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_HEIGHT)){
					//graphics->BindTexture(scene->GetTextureManager()->GetTexture(material->GetTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_HEIGHT))->GetId(), 1);
				}else if (material->HasCubeMap(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_HEIGHT)){
					//graphics->BindCubeMap(scene->GetTextureManager()->GetCubeMap(material->GetCubeMap(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_HEIGHT))->GetId(), 1);
				}

				//EG::Graphics::Mesh *mesh = scene->GetMeshManager()->Get(mesh_attribute->GetMeshId());

				++mesh_attribute_iterator;
			}

			// Light Attributes
			//

			out.close();
		}
	}
}
