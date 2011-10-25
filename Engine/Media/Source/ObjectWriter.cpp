#include "../ObjectWriter.h"

#include "../../Game/ObjectRenderingAttribute.h"
#include "../../Game/ObjectBasicAttribute.h"
#include "../../Game/ObjectEmissionAttribute.h"
#include "../../Graphics/Mesh.h"
#include "../../Graphics/RenderingMaterial.h"
#include "../../Utility/StringMethods.h"

#include <fstream>

namespace EG{
	namespace Media{
		ObjectWriter::ObjectWriter(EG::Game::Object *_object, EG::Game::Scene *_scene, std::string _object_name, std::string _images_output_path, std::string _model_output_path){
			scene = _scene;
			object = _object;
			object_name = _object_name;
			images_output_path = _images_output_path;
			model_output_path = _model_output_path;
		}
		ObjectWriter::~ObjectWriter(void){
			//
		}

		void ObjectWriter::Write(std::string file_path){
			std::ofstream out;
			out.open((model_output_path + file_path).c_str());

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
			out << "MATERIAL" << std::endl;
			std::vector<EG::Game::ObjectAttribute *> *mesh_attributes = object->GetAttributesByType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_RENDERING_MESH);
			std::vector<EG::Game::ObjectAttribute *>::iterator mesh_attribute_iterator = mesh_attributes->begin();
			while (mesh_attribute_iterator != mesh_attributes->end()){
				EG::Game::ObjectAttributeRenderingMesh *mesh_attribute = static_cast<EG::Game::ObjectAttributeRenderingMesh *>(*mesh_attribute_iterator);
				EG::Graphics::RenderingMaterial *material = mesh_attribute->GetMaterial();

				if (material->HasTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_DECAL)){
					EG::Graphics::Texture *texture = scene->GetTextureManager()->GetTexture(material->GetTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_DECAL));
					std::string image_out_path = (images_output_path + EG::Utility::StringMethods::GetFilenameFromPath(texture->GetFilePath()));
					CopyFile(texture->GetFilePath(), image_out_path.c_str());
					out << "DECAL" << image_out_path << std::endl;
				}else{
					out << "DECAL" << std::endl;
				}
				if (material->HasTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_NORMAL)){
					EG::Graphics::Texture *texture = scene->GetTextureManager()->GetTexture(material->GetTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_NORMAL));
					std::string image_out_path = (images_output_path + EG::Utility::StringMethods::GetFilenameFromPath(texture->GetFilePath()));
					CopyFile(texture->GetFilePath(), image_out_path.c_str());
					out << "NORMAL" << image_out_path << std::endl;
				}else{
					out << "NORMAL" << std::endl;
				}
				if (material->HasTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_HEIGHT)){
					EG::Graphics::Texture *texture = scene->GetTextureManager()->GetTexture(material->GetTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_HEIGHT));
					std::string image_out_path = (images_output_path + EG::Utility::StringMethods::GetFilenameFromPath(texture->GetFilePath()));
					CopyFile(texture->GetFilePath(), image_out_path.c_str());
					out << "HEIGHT" << image_out_path << std::endl;
				}else{
					out << "HEIGHT" << std::endl;
				}
				if (material->HasTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_SPECULAR)){
					EG::Graphics::Texture *texture = scene->GetTextureManager()->GetTexture(material->GetTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_SPECULAR));
					std::string image_out_path = (images_output_path + EG::Utility::StringMethods::GetFilenameFromPath(texture->GetFilePath()));
					CopyFile(texture->GetFilePath(), image_out_path.c_str());
					out << "SPECULAR" << image_out_path << std::endl;
				}else{
					out << "SPECULAR" << std::endl;
				}
				if (material->GetLit()){
					out << "LIT1" << std::endl;
				}else{
					out << "LIT0" << std::endl;
				}
				if (material->GetCastsShadows()){
					out << "CASTSSHADOWS1" << std::endl;
				}else{
					out << "CASTSSHADOWS0" << std::endl;
				}
				if (material->GetTranslucent()){
					out << "TRANSLUCENT1" << std::endl;
				}else{
					out << "TRANSLUCENT0" << std::endl;
				}
				out << "LIGHTING " << material->GetAmbient() << ' ' << material->GetDiffuse() << ' ' << material->GetSpecular() << ' ' << material->GetSpecularExponent() << std::endl;
				out << "COLOR " << material->GetColor().x << ' ' << material->GetColor().y << ' ' << material->GetColor().z << ' ' << material->GetColor().w << std::endl;
				// Don't forget to output custom shaders!

				out << "MESH" << std::endl;
				EG::Graphics::Mesh *mesh = scene->GetMeshManager()->Get(mesh_attribute->GetMeshId());
				out << mesh->GetVertexCount() << std::endl;
				out << mesh->GetStride() << std::endl;
				if (mesh->HasVertices()){
					out << "VERTICES" << std::endl;
					float *d = mesh->GetVertices();
					for (unsigned int i = 0; i < mesh->GetVertexCount(); i++){
						out << d[i * 4] << ' ' << d[(i * 4) + 1] << ' ' << d[(i * 4) + 2] << ' ' << d[(i * 4) + 1];
						if (i != (mesh->GetVertexCount() - 1)){
							out << ' ';
						}
					}
					out << std::endl;
				}
				if (mesh->HasNormals()){
					out << "NORMALS" << std::endl;
					float *d = mesh->GetNormals();
					for (unsigned int i = 0; i < mesh->GetVertexCount(); i++){
						out << d[i * 4] << ' ' << d[(i * 4) + 1] << ' ' << d[(i * 4) + 2] << ' ' << d[(i * 4) + 1];
						if (i != (mesh->GetVertexCount() - 1)){
							out << ' ';
						}
					}
					out << std::endl;
				}
				if (mesh->HasTexCoords()){
					out << "TEXCOORDS" << std::endl;
					float *d = mesh->GetTexCoords();
					for (unsigned int i = 0; i < mesh->GetVertexCount(); i++){
						out << d[i * 4] << ' ' << d[(i * 4) + 1] << ' ' << d[(i * 4) + 2] << ' ' << d[(i * 4) + 1];
						if (i != (mesh->GetVertexCount() - 1)){
							out << ' ';
						}
					}
					out << std::endl;
				}
				if (mesh->HasBinormals()){
					out << "BINORMALS" << std::endl;
					float *d = mesh->GetBinormals();
					for (unsigned int i = 0; i < mesh->GetVertexCount(); i++){
						out << d[i * 4] << ' ' << d[(i * 4) + 1] << ' ' << d[(i * 4) + 2] << ' ' << d[(i * 4) + 1];
						if (i != (mesh->GetVertexCount() - 1)){
							out << ' ';
						}
					}
					out << std::endl;
				}
				if (mesh->HasBitangents()){
					out << "BITANGENTS" << std::endl;
					float *d = mesh->GetBitangents();
					for (unsigned int i = 0; i < mesh->GetVertexCount(); i++){
						out << d[i * 4] << ' ' << d[(i * 4) + 1] << ' ' << d[(i * 4) + 2] << ' ' << d[(i * 4) + 1];
						if (i != (mesh->GetVertexCount() - 1)){
							out << ' ';
						}
					}
					out << std::endl;
				}

				++mesh_attribute_iterator;
			}

			// Light Attributes
			//

			out.close();
		}
		void ObjectWriter::CopyFile(std::string in, std::string out){
			std::ifstream in_file(in.c_str(), std::fstream::binary);
			std::ofstream out_file(out.c_str(), std::fstream::trunc|std::fstream::binary);
			out_file << in_file.rdbuf();
			in_file.close();
			out_file.close();
		}
	}
}
