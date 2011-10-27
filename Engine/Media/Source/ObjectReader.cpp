#include "../ObjectReader.h"
#include "../../Game/ObjectRenderingAttribute.h"
#include "../../Game/ObjectBasicAttribute.h"
#include "../../Game/ObjectEmissionAttribute.h"
#include "../../Graphics/Mesh.h"
#include "../../Graphics/RenderingMaterial.h"
#include <fstream>

namespace EG{
	namespace Media{
		ObjectReader::ObjectReader(void){
			object = NULL;
		}

		ObjectReader::~ObjectReader(void){
			//
		}

		/*
			string line;
			ifstream myfile ("example.txt");
			if (myfile.is_open()){
				while ( myfile.good() ){
					getline (myfile,line);
					cout << line << endl;
				}
				myfile.close();
			}
		*/

		bool ObjectReader::Read(std::string file_path, EG::Game::Scene *scene){
			std::ifstream in;
			in.open(file_path.c_str());

			std::string line;

			// Get Object Name
			std::getline(in, line);
			object = new EG::Game::Object(line);

			// Get Object Basic Transformation Attribute
			std::getline(in, line);
			float *t = EG::Utility::StringMethods::ConvertStringToFloatArray(line);
			glm::mat4 transformation = glm::mat4(1.0f);
			unsigned int fi = 0;
			for (unsigned int i = 0; i < 4; i++){
				for (unsigned int j = 0; j < 4; j++){
					transformation[i][j] = t[fi];
					fi += 1;
				}
			}
			object->AddAttribute(new EG::Game::ObjectAttributeBasicTransformation(transformation));

			std::getline(in, line);
			EG::Graphics::RenderingMaterial *material = new EG::Graphics::RenderingMaterial();

			std::getline(in, line);
			// DECAL
			std::string decal_path = line.substr(5);
			decal_path = EG::Utility::StringMethods::RemoveSpecialCharactersFromPathString(decal_path);
			if (decal_path.size() > 2){
				if (!(scene->GetTextureManager()->HasTexture(decal_path))){
					scene->GetTextureManager()->AddTexture(decal_path, new EG::Graphics::Texture(decal_path));
				}
				material->SetTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_DECAL, decal_path);
			}

			std::getline(in, line);
			// NORMAL
			std::string normal_path = line.substr(6);
			normal_path = EG::Utility::StringMethods::RemoveSpecialCharactersFromPathString(normal_path);
			if (normal_path.size() > 2){
				if (!(scene->GetTextureManager()->HasTexture(normal_path))){
					EG::Graphics::Texture *new_tex = new EG::Graphics::Texture(normal_path);
					scene->GetTextureManager()->AddTexture(normal_path, new_tex);
				}
				material->SetTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_NORMAL, normal_path);
			}

			std::getline(in, line);
			// HEIGHT
			std::string height_path = line.substr(6);
			height_path = EG::Utility::StringMethods::RemoveSpecialCharactersFromPathString(height_path);
			if (height_path.size() > 2){
				if (!(scene->GetTextureManager()->HasTexture(height_path))){
					scene->GetTextureManager()->AddTexture(height_path, new EG::Graphics::Texture(height_path));
				}
				material->SetTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_HEIGHT, height_path);
			}

			std::getline(in, line);
			// SPECULAR
			std::string specular_path = line.substr(8);
			specular_path = EG::Utility::StringMethods::RemoveSpecialCharactersFromPathString(specular_path);
			if (specular_path.size() > 2){
				if (!(scene->GetTextureManager()->HasTexture(specular_path))){
					scene->GetTextureManager()->AddTexture(specular_path, new EG::Graphics::Texture(specular_path));
				}
				material->SetTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_SPECULAR, specular_path);
			}

			// LIT
			std::getline(in, line);
			unsigned int is_lit = atoi(line.substr(3).c_str());
			if (is_lit != 1){
				material->SetLit(false);
			}

			// CASTS SHADOWS
			std::getline(in, line);
			unsigned int is_casts_shadows = atoi(line.substr(12).c_str());
			if (is_casts_shadows != 1){
				material->SetCastsShadows(false);
			}

			// TRANSLUCENT
			std::getline(in, line);
			unsigned int is_translucent = atoi(line.substr(11).c_str());
			if (is_translucent == 1){
				material->SetTranslucent(true);
			}

			// LIGHTING
			std::getline(in, line);
			float *lighting_parameters = EG::Utility::StringMethods::ConvertStringToFloatArray(line.substr(9));
			material->SetAmbient(lighting_parameters[0]);
			material->SetDiffuse(lighting_parameters[1]);
			material->SetSpecular(lighting_parameters[2]);
			material->SetSpecularExponent(lighting_parameters[3]);

			// COLOR
			std::getline(in, line);
			float *color = EG::Utility::StringMethods::ConvertStringToFloatArray(line.substr(6));
			material->SetColor(glm::vec4(color[0], color[1], color[2], color[3]));

			std::getline(in, line);
			std::getline(in, line);
			std::string mesh_name = line;
			std::getline(in, line);
			unsigned int vertex_count = atoi(line.c_str());
			std::getline(in, line);
			bool has_vers = false;
			bool has_nors = false;
			bool has_texs = false;
			bool has_bins = false;
			bool has_bits = false;
			float *vers, *nors, *texs, *bins, *bits;
			while (in.good()){
				std::getline(in, line);
				std::string simple_prefix = line.substr(0, 3);
				std::getline(in, line);
				if (simple_prefix == "VER"){
					vers = EG::Utility::StringMethods::ConvertStringToFloatArray(line);
					has_vers = true;
				}else if (simple_prefix == "NOR"){
					//if (graphics->CheckVersion(3)){
						nors = EG::Utility::StringMethods::ConvertStringToFloatArray(line);
					//}else{
						//float *tn = EG::Utility::StringMethods::ConvertStringToFloatArray(line);
					//}
					has_nors = true;
				}else if (simple_prefix == "TEX"){
					texs = EG::Utility::StringMethods::ConvertStringToFloatArray(line);
					has_texs = true;
				}else if (simple_prefix == "BIT"){
					bits = EG::Utility::StringMethods::ConvertStringToFloatArray(line);
					has_bits = true;
				}else if (simple_prefix == "BIN"){
					bins = EG::Utility::StringMethods::ConvertStringToFloatArray(line);
					has_bins = true;
				}
			}
			EG::Graphics::Mesh *mesh = new EG::Graphics::Mesh(vertex_count, 4, vers, has_vers, texs, has_texs, nors, has_nors, bins, has_bins, bits, has_bits);
			scene->GetMeshManager()->Add(mesh_name, mesh);

			object->AddAttribute(new EG::Game::ObjectAttributeRenderingMesh(mesh_name, material));

			in.close();
		}

		EG::Game::Object *ObjectReader::GetObject(void){
			return object;
		}
	}
}

