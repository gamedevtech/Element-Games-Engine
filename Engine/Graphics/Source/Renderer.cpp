#include "../Renderer.h"

#include <iostream>
#include <sstream>

#include "../../Game/ObjectRenderingAttribute.h"
#include "../../Game/ObjectBasicAttribute.h"
#include "../../Game/ObjectEmissionAttribute.h"
#include "../../Graphics/MeshManager.h"
#include "../../Graphics/TextureManager.h"
#include "../../Utility/Font.h"
#include "../../Utility/Text.h"

namespace EG{
	namespace Graphics{
		Renderer::Renderer(void){
			initialized = false;
			gui_set = false;
			//meshes = new EG::Graphics::MeshManager();
			//textures = new EG::Graphics::TextureManager();
			//font_manager;
			//text_manager->AddText("frame-time", "Frame-Time: ", "default", 50.0f, 50.0f);
		}

		Renderer::~Renderer(void){
			if (initialized){
				delete camera;
				delete shaders;
			}
		}

		void Renderer::Initialize(void){
			shaders = new EG::Graphics::ShaderManager();
			shaders->Add("lighting", "Shaders/Basic/lighting.vert", "Shaders/Basic/lighting.frag");
			shaders->Add("textured", "Shaders/Basic/textured.vert", "Shaders/Basic/textured.frag");
			shaders->Add("font_rendering", "Shaders/Basic/font_rendering.vert", "Shaders/Basic/font_rendering.frag");
			shaders->Add("sphere_cube_map_gradient_decal", "Shaders/Basic/sphere_cube_mapped_with_gradient_decal.vert", "Shaders/Basic/sphere_cube_mapped_with_gradient_decal.frag");
			shaders->Add("sphere_cube_map_gradient_decal_with_lighting", "Shaders/Basic/sphere_cube_mapped_with_gradient_decal_with_lighting.vert", "Shaders/Basic/sphere_cube_mapped_with_gradient_decal_with_lighting.frag");

			camera = new EG::Graphics::Camera(45.0f, glm::ivec2(800, 500), glm::vec2(0.1f, 100.0f));
			camera->ComputeProjectionMatrix();
			camera->SetPosition(glm::vec3(0.0f, 0.0f, 5.0f));
			camera->Update();
			camera->SetCameraType(EG::Graphics::Camera::CAMERA_FPS);

			orthographics_projection_matrix = glm::gtc::matrix_transform::ortho(0.0f, 800.0f, 0.0f, 500.0f);

			initialized = true;
		}

		void Renderer::SetGUI(EG::Utility::RocketInterface *_gui){
			gui = _gui;
			gui_set = true;
		}

		void Renderer::Render(EG::Game::Scene *scene, float frame_time){
			graphics->BeginFrame();
			camera->Update();

			glDepthFunc(GL_LEQUAL);
			glEnable(GL_BLEND);
			glBlendFunc(GL_ONE, GL_ONE);

			glm::vec3 camera_position = camera->GetPosition();
			shaders->Bind("lighting");
			shaders->SetMatrix4("projection_matrix", camera->GetProjectionMatrix());
			shaders->SetMatrix4("view_matrix", camera->GetViewMatrix());
			shaders->SetFloat3("camera_position", camera_position);
			shaders->SetInt("decal", 0);
			shaders->SetInt("height", 1);

			// Ambient Pass
			shaders->SetFloat3("light_color", 0.0f, 0.0f, 0.0f);
			shaders->SetFloat4("light_position", 0.0f, 0.0f, 0.0f, 1.0f);
			shaders->SetFloat3("light_attenuation", 1.0f, 1.0f, 1.0f);
			shaders->SetFloat("light_radius", 0.0000001f);

			// Render Objects
			EG::Utility::StringDictionary<EG::Game::Object *> *objects = scene->GetObjectManager()->GetObjects();
			EG::Utility::StringDictionaryKeysIterator object_iterator = objects->GetKeysBegin();
			while (object_iterator != objects->GetKeysEnd()){
				EG::Game::Object *object = objects->Get(*object_iterator);

				// Meshes
				std::vector<EG::Game::ObjectAttribute *> *mesh_attributes = object->GetAttributesByType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_RENDERING_MESH);
				std::vector<EG::Game::ObjectAttribute *>::iterator mesh_attribute_iterator = mesh_attributes->begin();
				while (mesh_attribute_iterator != mesh_attributes->end()){
					EG::Game::ObjectAttributeRenderingMesh *mesh_attribute = static_cast<EG::Game::ObjectAttributeRenderingMesh *>(*mesh_attribute_iterator);
					EG::Graphics::RenderingMaterial *material = mesh_attribute->GetMaterial();
					if (material->GetLit()){
						bool custom_shader = false;
						if (material->HasShader(EG::Graphics::RenderingMaterial::RENDERER_BASIC, EG::Graphics::RenderingMaterial::RENDERING_PHASE_LIGHTING_SHADER)){
							custom_shader = true;
							shaders->Unbind();
							shaders->Bind(material->GetShader(EG::Graphics::RenderingMaterial::RENDERER_BASIC, EG::Graphics::RenderingMaterial::RENDERING_PHASE_LIGHTING_SHADER));
							//shaders->Bind("sphere_cube_map_gradient_decal_with_lighting");

							shaders->SetMatrix4("projection_matrix", camera->GetProjectionMatrix());
							shaders->SetMatrix4("view_matrix", camera->GetViewMatrix());
							shaders->SetFloat3("camera_position", camera_position);
							shaders->SetInt("decal", 0);
							shaders->SetInt("height", 1);

							// Ambient Pass
							shaders->SetFloat3("light_color", 0.0f, 0.0f, 0.0f);
							shaders->SetFloat4("light_position", 0.0f, 0.0f, 0.0f, 1.0f);
							shaders->SetFloat3("light_attenuation", 1.0f, 1.0f, 1.0f);
							shaders->SetFloat("light_radius", 0.0000001f);
						}

						if (material->HasTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_DECAL)){
							graphics->BindTexture(scene->GetTextureManager()->GetTexture(material->GetTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_DECAL))->GetId());
						}else if (material->HasCubeMap(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_HEIGHT)){
							graphics->BindCubeMap(scene->GetTextureManager()->GetCubeMap(material->GetTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_DECAL))->GetId());
						}else{
							graphics->BindTexture(scene->GetTextureManager()->GetTexture("default_decal")->GetId());
						}
						if (material->HasTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_HEIGHT)){
							graphics->BindTexture(scene->GetTextureManager()->GetTexture(material->GetTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_HEIGHT))->GetId(), 1);
						}else if (material->HasCubeMap(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_HEIGHT)){
							graphics->BindCubeMap(scene->GetTextureManager()->GetCubeMap(material->GetCubeMap(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_HEIGHT))->GetId(), 1);
						}else{
							graphics->BindTexture(scene->GetTextureManager()->GetTexture("default_height")->GetId(), 1);
						}

						// Transformation
						std::vector<EG::Game::ObjectAttribute *> *transformation_attributes = object->GetAttributesByType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_BASIC_TRANSFORMATION);
						EG::Game::ObjectAttributeBasicTransformation *transformation_attribute = static_cast<EG::Game::ObjectAttributeBasicTransformation *>(transformation_attributes->at(0));
						glm::mat4 transformation = transformation_attribute->GetTransformation();
						glm::mat4 normal_matrix = EG::Math::Utility::GenerateNormalMatrix(transformation);
						shaders->SetMatrix4("model_matrix", transformation);
						shaders->SetMatrix4("normal_matrix", normal_matrix);

						EG::Graphics::Mesh *mesh = scene->GetMeshManager()->Get(mesh_attribute->GetMeshId());
						if (mesh){
							mesh->Draw();
						}

						if (custom_shader){
							shaders->Unbind();
							shaders->Bind("lighting");

							shaders->SetMatrix4("projection_matrix", camera->GetProjectionMatrix());
							shaders->SetMatrix4("view_matrix", camera->GetViewMatrix());
							shaders->SetFloat3("camera_position", camera_position);
							shaders->SetInt("decal", 0);
							shaders->SetInt("height", 1);

							// Ambient Pass
							shaders->SetFloat3("light_color", 0.0f, 0.0f, 0.0f);
							shaders->SetFloat4("light_position", 0.0f, 0.0f, 0.0f, 1.0f);
							shaders->SetFloat3("light_attenuation", 1.0f, 1.0f, 1.0f);
							shaders->SetFloat("light_radius", 0.0000001f);
						}
					}
					++mesh_attribute_iterator;
				}
				++object_iterator;
			}

			EG::Utility::StringDictionary<EG::Game::Object *> *light_objects = scene->GetObjectManager()->GetObjects();
			EG::Utility::StringDictionaryKeysIterator light_object_iterator = objects->GetKeysBegin();
			while (light_object_iterator != light_objects->GetKeysEnd()){
				EG::Game::Object *light_object = light_objects->Get(*light_object_iterator);
				if (light_object->HasAttributesOfType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_EMISSION_LIGHT)){
					std::vector<EG::Game::ObjectAttribute *> *light_attributes = light_object->GetAttributesByType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_EMISSION_LIGHT);
					std::vector<EG::Game::ObjectAttribute *>::iterator light_attribute_iterator = light_attributes->begin();
					while (light_attribute_iterator != light_attributes->end()){
						EG::Game::ObjectAttributeEmissionLight *light_attribute = static_cast<EG::Game::ObjectAttributeEmissionLight *>(*light_attribute_iterator);
						EG::Graphics::Light *light = light_attribute->GetLight();

						glm::vec3 light_color = light->GetColor();
						// Matrices seem inappropriate now... hmm
						glm::vec3 lp = light->GetPosition();
						glm::vec4 light_position = glm::vec4(lp.x, lp.y, lp.z, 1.0f);
						light_position.w = 1.0f;
						glm::vec4 light_view = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
						shaders->SetFloat3("light_color", light->GetColor());
						shaders->SetFloat4("light_position", light_position);
						shaders->SetFloat3("light_attenuation", light->GetAttenuation());
						shaders->SetFloat("light_radius", light->GetRadius());

						// Render Objects
						object_iterator = objects->GetKeysBegin();
						while (object_iterator != objects->GetKeysEnd()){
							EG::Game::Object *object = objects->Get(*object_iterator);

							// Meshes
							std::vector<EG::Game::ObjectAttribute *> *mesh_attributes = object->GetAttributesByType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_RENDERING_MESH);
							std::vector<EG::Game::ObjectAttribute *>::iterator mesh_attribute_iterator = mesh_attributes->begin();
							while (mesh_attribute_iterator != mesh_attributes->end()){
								EG::Game::ObjectAttributeRenderingMesh *mesh_attribute = static_cast<EG::Game::ObjectAttributeRenderingMesh *>(*mesh_attribute_iterator);
								EG::Graphics::RenderingMaterial *material = mesh_attribute->GetMaterial();
								if (material->GetLit()){
									bool custom_shader = false;
									if (material->HasShader(EG::Graphics::RenderingMaterial::RENDERER_BASIC, EG::Graphics::RenderingMaterial::RENDERING_PHASE_LIGHTING_SHADER)){
										custom_shader = true;
										shaders->Unbind();
										shaders->Bind(material->GetShader(EG::Graphics::RenderingMaterial::RENDERER_BASIC, EG::Graphics::RenderingMaterial::RENDERING_PHASE_LIGHTING_SHADER));
										//shaders->Bind("sphere_cube_map_gradient_decal_with_lighting");

										shaders->SetMatrix4("projection_matrix", camera->GetProjectionMatrix());
										shaders->SetMatrix4("view_matrix", camera->GetViewMatrix());
										shaders->SetFloat3("camera_position", camera_position);
										shaders->SetInt("decal", 0);
										shaders->SetInt("height", 1);

										shaders->SetFloat3("light_color", light->GetColor());
										shaders->SetFloat4("light_position", light_position);
										shaders->SetFloat3("light_attenuation", light->GetAttenuation());
										shaders->SetFloat("light_radius", light->GetRadius());
									}

									if (material->HasTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_DECAL)){
										graphics->BindTexture(scene->GetTextureManager()->GetTexture(material->GetTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_DECAL))->GetId());
									}else if (material->HasCubeMap(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_HEIGHT)){
										graphics->BindCubeMap(scene->GetTextureManager()->GetCubeMap(material->GetTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_DECAL))->GetId());
									}else{
										graphics->BindTexture(scene->GetTextureManager()->GetTexture("default_decal")->GetId());
									}
									if (material->HasTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_HEIGHT)){
										graphics->BindTexture(scene->GetTextureManager()->GetTexture(material->GetTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_HEIGHT))->GetId(), 1);
									}else if (material->HasCubeMap(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_HEIGHT)){
										graphics->BindCubeMap(scene->GetTextureManager()->GetCubeMap(material->GetCubeMap(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_HEIGHT))->GetId(), 1);
									}else{
										graphics->BindTexture(scene->GetTextureManager()->GetTexture("default_height")->GetId(), 1);
									}

									// Transformation
									std::vector<EG::Game::ObjectAttribute *> *transformation_attributes = object->GetAttributesByType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_BASIC_TRANSFORMATION);
									EG::Game::ObjectAttributeBasicTransformation *transformation_attribute = static_cast<EG::Game::ObjectAttributeBasicTransformation *>(transformation_attributes->at(0));
									glm::mat4 transformation = transformation_attribute->GetTransformation();
									glm::mat4 normal_matrix = EG::Math::Utility::GenerateNormalMatrix(transformation);
									shaders->SetMatrix4("model_matrix", transformation);
									shaders->SetMatrix4("normal_matrix", normal_matrix);

									EG::Graphics::Mesh *mesh = scene->GetMeshManager()->Get(mesh_attribute->GetMeshId());
									if (mesh){
										mesh->Draw();
									}

									if (custom_shader){
										shaders->Unbind();
										shaders->Bind("lighting");

										shaders->SetMatrix4("projection_matrix", camera->GetProjectionMatrix());
										shaders->SetMatrix4("view_matrix", camera->GetViewMatrix());
										shaders->SetFloat3("camera_position", camera_position);
										shaders->SetInt("decal", 0);
										shaders->SetInt("height", 1);

										shaders->SetFloat3("light_color", light->GetColor());
										shaders->SetFloat4("light_position", light_position);
										shaders->SetFloat3("light_attenuation", light->GetAttenuation());
										shaders->SetFloat("light_radius", light->GetRadius());
									}
								}
								++mesh_attribute_iterator;
							}
							++object_iterator;
						}
						++light_attribute_iterator;
					}
				}
				++light_object_iterator;
			}
			shaders->Unbind();
			glDisable(GL_BLEND);

			// Non Lit Objects (like lights)
			shaders->Bind("textured");
			shaders->SetMatrix4("projection_matrix", camera->GetProjectionMatrix());
			shaders->SetMatrix4("view_matrix", camera->GetViewMatrix());
			shaders->SetInt("decal", 0);
			shaders->SetInt("height", 1);
			object_iterator = objects->GetKeysBegin();
			//std::cout << "Rendering Non-Lit Objects" << std::endl;
			while (object_iterator != objects->GetKeysEnd()){
				EG::Game::Object *object = objects->Get(*object_iterator);

				// Meshes
				std::vector<EG::Game::ObjectAttribute *> *mesh_attributes = object->GetAttributesByType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_RENDERING_MESH);
				std::vector<EG::Game::ObjectAttribute *>::iterator mesh_attribute_iterator = mesh_attributes->begin();
				while (mesh_attribute_iterator != mesh_attributes->end()){
					EG::Game::ObjectAttributeRenderingMesh *mesh_attribute = static_cast<EG::Game::ObjectAttributeRenderingMesh *>(*mesh_attribute_iterator);
					EG::Graphics::RenderingMaterial *material = mesh_attribute->GetMaterial();
					if (!material->GetLit()){
						bool custom_shader = false;
						if (material->HasShader(EG::Graphics::RenderingMaterial::RENDERER_BASIC, EG::Graphics::RenderingMaterial::RENDERING_PHASE_TEXTURED_SHADER)){
							custom_shader = true;
							shaders->Unbind();
							shaders->Bind(material->GetShader(EG::Graphics::RenderingMaterial::RENDERER_BASIC, EG::Graphics::RenderingMaterial::RENDERING_PHASE_TEXTURED_SHADER));
							//shaders->Bind("sphere_cube_map_gradient_decal");
							//std::cout << material->GetShader(EG::Graphics::RenderingMaterial::RENDERER_BASIC, EG::Graphics::RenderingMaterial::RENDERING_PHASE_TEXTURED_SHADER) << std::endl;

							shaders->SetMatrix4("projection_matrix", camera->GetProjectionMatrix());
							shaders->SetMatrix4("view_matrix", camera->GetViewMatrix());
							shaders->SetInt("decal", 0);
							shaders->SetInt("height", 1);
						}

						if (material->HasTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_DECAL)){
							graphics->BindTexture(scene->GetTextureManager()->GetTexture(material->GetTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_DECAL))->GetId());
						}else if (material->HasCubeMap(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_HEIGHT)){
							graphics->BindCubeMap(scene->GetTextureManager()->GetCubeMap(material->GetTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_DECAL))->GetId());
						}else{
							graphics->BindTexture(scene->GetTextureManager()->GetTexture("default_decal")->GetId());
						}
						if (material->HasTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_HEIGHT)){
							graphics->BindTexture(scene->GetTextureManager()->GetTexture(material->GetTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_HEIGHT))->GetId(), 1);
						}else if (material->HasCubeMap(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_HEIGHT)){
							graphics->BindCubeMap(scene->GetTextureManager()->GetCubeMap(material->GetCubeMap(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_HEIGHT))->GetId(), 1);
						}else{
							graphics->BindTexture(scene->GetTextureManager()->GetTexture("default_height")->GetId(), 1);
						}

						glm::vec4 color = material->GetColor();
						shaders->SetFloat4("color", color);

						// Transformation
						std::vector<EG::Game::ObjectAttribute *> *transformation_attributes = object->GetAttributesByType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_BASIC_TRANSFORMATION);
						EG::Game::ObjectAttributeBasicTransformation *transformation_attribute = static_cast<EG::Game::ObjectAttributeBasicTransformation *>(transformation_attributes->at(0));
						glm::mat4 transformation = transformation_attribute->GetTransformation();

						glm::mat4 normal_matrix = EG::Math::Utility::GenerateNormalMatrix(transformation);
						shaders->SetMatrix4("model_matrix", transformation);
						shaders->SetMatrix4("normal_matrix", normal_matrix);

						EG::Graphics::Mesh *mesh = scene->GetMeshManager()->Get(mesh_attribute->GetMeshId());
						if (mesh){
							mesh->Draw();
						}

						if (custom_shader){
							shaders->Unbind();
							shaders->Bind("textured");

							shaders->SetMatrix4("projection_matrix", camera->GetProjectionMatrix());
							shaders->SetMatrix4("view_matrix", camera->GetViewMatrix());
							shaders->SetInt("decal", 0);
							shaders->SetInt("height", 1);
						}
					}
					++mesh_attribute_iterator;
				}

				++object_iterator;
			}
			shaders->Unbind();

			graphics->EndFrame();

			// 2D Overlays
			glDisable(GL_DEPTH_TEST);
			shaders->Bind("font_rendering");
			shaders->SetMatrix4("projection_matrix", orthographics_projection_matrix);
			shaders->SetMatrix4("view_matrix", glm::mat4(1.0f));
			shaders->SetMatrix4("model_matrix", glm::mat4(1.0f));
			shaders->SetInt("decal", 0);
			shaders->SetInt("use_decal", 1);
			shaders->SetFloat4("color", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
			glEnable(GL_BLEND);
			glEnable(GL_TEXTURE_2D);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			std::stringstream temp;
			temp << "Frame Time (s): ";
			temp << frame_time;
			temp.flush();
			//shaders->SetMatrix4("model_matrix", glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(40.0f, 40.0f, 0.0f)), glm::vec3(1.0f, 1.0f, 1.0f)));
			glm::vec3 position = glm::vec3(5.0f, 5.0f, 0.0f);
			glm::vec2 scale = glm::vec2(1.0f, 1.0f);
			//font_manager->DrawText(temp.str(), position, scale, shaders);

			if (gui_set){
				gui->Draw();
			}
			glDisable(GL_BLEND);
			glEnable(GL_DEPTH_TEST);
			shaders->Unbind();
		}
	}
}
