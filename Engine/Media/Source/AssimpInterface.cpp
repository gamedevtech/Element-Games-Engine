#include "../AssimpInterface.h"

#include <iostream>

namespace EG{
	namespace Media{
		AssimpInterface::AssimpInterface(EG::Game::Scene *_scene){
			scene = _scene;
			count = 0;
		}

		AssimpInterface::~AssimpInterface(void){
			//delete ai_scene;
		}

		bool AssimpInterface::Load(std::string _file_path){
			Assimp::Importer importer;
			file_path = _file_path;
			ai_scene = importer.ReadFile(file_path, /*aiProcess_CalcTangentSpace | */aiProcess_Triangulate | /*aiProcess_JoinIdenticalVertices | */aiProcess_SortByPType);
			if (!ai_scene){
				return false;
			}

			std::cout << "Loading " << file_path << std::endl;
			//aiNode *root = ai_scene->mRootNode;
			for (unsigned int i = 0; i < ai_scene->mNumMeshes; i++){
				//std::cout << "Mesh Index: " << i << std::endl;
				const struct aiMesh *mesh = ai_scene->mMeshes[i];
				const struct aiMaterial *material = ai_scene->mMaterials[mesh->mMaterialIndex];

				LoadMaterial(material, i);
				LoadMesh(mesh, i);
				count += 1;
			}

			return true;
		}

		void AssimpInterface::LoadMesh(const aiMesh *ai_mesh, unsigned int index){
			if (ai_mesh->HasFaces()){
				std::string mesh_name = file_path + std::string(ai_mesh->mName.data);
				EG::Graphics::Triangle *faces = new EG::Graphics::Triangle[ai_mesh->mNumFaces];
				for (unsigned int i = 0; i < ai_mesh->mNumFaces; i++){
					const aiFace *ai_face = &(ai_mesh->mFaces[i]);
					// For each vertex which is garanteed to be 3 because we always triangulate!
					for (unsigned int v = 0; v < /*3*/ai_face->mNumIndices; v++){
						unsigned int vertex_index = ai_face->mIndices[v];
						aiVector3D vertex = ai_mesh->mVertices[vertex_index];
						aiVector3D normal = ai_mesh->mNormals[vertex_index];
						aiVector3D texcoord = ai_mesh->mTextureCoords[0][vertex_index];

						faces[i].vertices[2 - v] = glm::vec4(vertex.x, vertex.y, vertex.z, 1.0f);
						faces[i].texcoords[2 - v] = glm::vec4(texcoord.x, texcoord.y, 1.0f, 1.0f);
					}
				}
				EG::Graphics::TriangleMesh *tmesh = new EG::Graphics::TriangleMesh(ai_mesh->mNumFaces, faces, /*false, false*/true, true, true, true, true, true);
				std::cout << tmesh->GetTriangles()->at(0).tangents[0].x << ' ' << tmesh->GetTriangles()->at(0).tangents[0].y << ' ' << tmesh->GetTriangles()->at(0).tangents[0].z << ' ' << tmesh->GetTriangles()->at(0).tangents[0].w << std::endl;
				std::cout << tmesh->GetTriangles()->at(0).bitangents[0].x << ' ' << tmesh->GetTriangles()->at(0).bitangents[0].y << ' ' << tmesh->GetTriangles()->at(0).bitangents[0].z << ' ' << tmesh->GetTriangles()->at(0).bitangents[0].w << std::endl;
				scene->GetMeshManager()->Add(mesh_name, new EG::Graphics::Mesh(tmesh));
				meshes.Set(index, mesh_name);
			}
		}

		void AssimpInterface::LoadMaterial(const aiMaterial *ai_material, unsigned int index){
			EG::Graphics::RenderingMaterial *material = new EG::Graphics::RenderingMaterial();
			// Texture Loading
			aiString path;
			std::string texture_path;
			// Diffuse
			unsigned int diffuse_texture_count = ai_material->GetTextureCount(aiTextureType_DIFFUSE);
			if (diffuse_texture_count > 0){
				for (unsigned int mi = 0; mi < diffuse_texture_count; mi++){
					ai_material->GetTexture(aiTextureType_DIFFUSE, mi, &path);
				}
				texture_path = std::string("Assets/Models/") + std::string(path.data);
				if (!(scene->GetTextureManager()->HasTexture(texture_path))){
					scene->GetTextureManager()->AddTexture(texture_path, new EG::Graphics::Texture(texture_path));
				}
				material->SetTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_DECAL, texture_path);
			}
			// Normal
			unsigned int normal_texture_count = ai_material->GetTextureCount(aiTextureType_NORMALS);
			if (normal_texture_count > 0){
				for (unsigned int mi = 0; mi < normal_texture_count; mi++){
					ai_material->GetTexture(aiTextureType_NORMALS, mi, &path);
				}
				texture_path = std::string("Assets/Models/") + std::string(path.data);
				if (!(scene->GetTextureManager()->HasTexture(texture_path))){
					scene->GetTextureManager()->AddTexture(texture_path, new EG::Graphics::Texture(texture_path));
				}
				material->SetTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_NORMAL, texture_path);
			}
			// Height
			unsigned int height_texture_count = ai_material->GetTextureCount(aiTextureType_HEIGHT);
			if (height_texture_count > 0){
				for (unsigned int mi = 0; mi < height_texture_count; mi++){
					ai_material->GetTexture(aiTextureType_HEIGHT, mi, &path);
				}
				texture_path = std::string("Assets/Models/") + std::string(path.data);
				if (!(scene->GetTextureManager()->HasTexture(texture_path))){
					scene->GetTextureManager()->AddTexture(texture_path, new EG::Graphics::Texture(texture_path));
				}
				material->SetTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_HEIGHT, texture_path);
			}
			// Specular
			unsigned int specular_texture_count = ai_material->GetTextureCount(aiTextureType_SPECULAR);
			if (specular_texture_count > 0){
				for (unsigned int mi = 0; mi < specular_texture_count; mi++){
					ai_material->GetTexture(aiTextureType_SPECULAR, mi, &path);
				}
				texture_path = std::string("Assets/Models/") + std::string(path.data);
				if (!(scene->GetTextureManager()->HasTexture(texture_path))){
					scene->GetTextureManager()->AddTexture(texture_path, new EG::Graphics::Texture(texture_path));
				}
				material->SetTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_SPECULAR, texture_path);
			}
			materials.Set(index, material);
		}
	}
}
