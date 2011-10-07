#include "../RendererMultipass.h"

#include <iostream>

#include "../../Game/ObjectRenderingAttribute.h"
#include "../../Game/ObjectBasicAttribute.h"
#include "../../Game/ObjectEmissionAttribute.h"

namespace EG{
	namespace Graphics{
		RendererMultipass::RendererMultipass(void) : Renderer(){
			EG::Graphics::Triangle *rect_triangles = new EG::Graphics::Triangle[2];
			rect_triangles[0].vertices[0] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			rect_triangles[0].vertices[1] = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
			rect_triangles[0].vertices[2] = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
			rect_triangles[1].vertices[0] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			rect_triangles[1].vertices[1] = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
			rect_triangles[1].vertices[2] = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
			rect_triangles[0].texcoords[0] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			rect_triangles[0].texcoords[1] = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
			rect_triangles[0].texcoords[2] = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
			rect_triangles[1].texcoords[0] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			rect_triangles[1].texcoords[1] = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
			rect_triangles[1].texcoords[2] = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
			EG::Graphics::TriangleMesh *rect_triangle_mesh = new EG::Graphics::TriangleMesh(2, rect_triangles, true, true, true, true, true, true);
			rectangle = new EG::Graphics::Mesh(rect_triangle_mesh);
			output_type = EG::Graphics::RendererMultipass::MULTIPASS_OUTPUT_NORMAL;
		}

		RendererMultipass::~RendererMultipass(void){
			if (initialized){
				delete camera;
				delete shaders;
			}
		}

		void RendererMultipass::Initialize(void){
			shaders = new EG::Graphics::ShaderManager();
			shaders->Add("lighting", "Shaders/Multipass/lighting.vert", "Shaders/Multipass/lighting.frag");
			shaders->Add("textured", "Shaders/Multipass/textured.vert", "Shaders/Multipass/textured.frag");
			shaders->Add("debug", "Shaders/Multipass/debug.vert", "Shaders/Multipass/debug.frag");
			shaders->Add("composition", "Shaders/Multipass/composition.vert", "Shaders/Multipass/composition.frag");
			shaders->Add("convolution", "Shaders/Deferred/convolution.vert", "Shaders/Deferred/convolution.frag");
			shaders->Add("gaussian_v", "Shaders/Deferred/gaussian_v.vert", "Shaders/Deferred/gaussian_v.frag");
			shaders->Add("gaussian_h", "Shaders/Deferred/gaussian_h.vert", "Shaders/Deferred/gaussian_h.frag");
			shaders->Add("sphere_cube_map_gradient_decal", "Shaders/Multipass/sphere_cube_mapped_with_gradient_decal.vert", "Shaders/Multipass/sphere_cube_mapped_with_gradient_decal.frag");
			shaders->Add("sphere_cube_map_gradient_decal_with_lighting", "Shaders/Multipass/sphere_cube_mapped_with_gradient_decal_with_lighting.vert", "Shaders/Multipass/sphere_cube_mapped_with_gradient_decal_with_lighting.frag");

			camera = new EG::Graphics::Camera(45.0f, glm::ivec2(800, 500), glm::vec2(0.1f, 100.0f));
			camera->ComputeProjectionMatrix();
			camera->SetPosition(glm::vec3(0.0f, 0.0f, 5.0f));
			camera->Update();
			camera->SetCameraType(EG::Graphics::Camera::CAMERA_FPS);

			light_buffer = new EG::Graphics::OffscreenBuffer(800, 500, 1, true, EG::Graphics::OffscreenBuffer::OFFSCREEN_BUFFER_FILTERING_LINEAR);
			color_buffer = new EG::Graphics::OffscreenBuffer(800, 500, 1, true, EG::Graphics::OffscreenBuffer::OFFSCREEN_BUFFER_FILTERING_NONE);
			bloom_buffer = new EG::Graphics::OffscreenBuffer(800 / 4.0f, 500 / 4.0f, 3, true, EG::Graphics::OffscreenBuffer::OFFSCREEN_BUFFER_FILTERING_LINEAR);
			orthographics_projection_matrix = glm::gtc::matrix_transform::ortho(0.0f, 800.0f, 0.0f, 500.0f);

			initialized = true;
		}

		void RendererMultipass::Render(EG::Game::Scene *scene){
			graphics->BeginFrame();
			camera->Update();

			graphics->StartOffscreenRender(light_buffer->GetBufferId(), 0, 800, 500);

			glDepthFunc(GL_LEQUAL);
			glEnable(GL_BLEND);
			glBlendFunc(GL_ONE, GL_ONE);
			// Lit Objects (Need ambient pass!) (Need to blend properly)
			shaders->Bind("lighting");

			shaders->SetMatrix4("projection_matrix", camera->GetProjectionMatrix());
			shaders->SetMatrix4("view_matrix", camera->GetViewMatrix());

			glm::vec3 camera_position = camera->GetPosition();
			shaders->SetFloat3("camera_position", camera_position);

			//shaders->SetInt("decal", 0);
			shaders->SetInt("normal_map", 0);
			shaders->SetInt("height_map", 1);

			// Ambient Pass
			shaders->SetFloat3("light_color", 0.0f, 0.0f, 0.0f);
			shaders->SetFloat4("light_position", 0.0f, 0.0f, 0.0f, 1.0f);
			shaders->SetFloat3("light_attenuation", 1.0f, 1.0f, 1.0f);
			shaders->SetFloat("light_radius", 0.01f);

			// Render Objects
			std::map<std::string, EG::Game::Object *> *objects = scene->GetObjectManager()->GetObjects();
			std::map<std::string, EG::Game::Object *>::iterator object_iterator = objects->begin();
			while (object_iterator != objects->end()){
				EG::Game::Object *object = object_iterator->second;

				// Meshes
				std::vector<EG::Game::ObjectAttribute *> *mesh_attributes = object->GetAttributesByType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_RENDERING_MESH);
				std::vector<EG::Game::ObjectAttribute *>::iterator mesh_attribute_iterator = mesh_attributes->begin();
				while (mesh_attribute_iterator != mesh_attributes->end()){
					EG::Game::ObjectAttributeRenderingMesh *mesh_attribute = static_cast<EG::Game::ObjectAttributeRenderingMesh *>(*mesh_attribute_iterator);
					EG::Graphics::RenderingMaterial *material = mesh_attribute->GetMaterial();
					if (material->GetLit()){
						/*bool custom_shader = false;
						if (material->HasShader(EG::Graphics::RenderingMaterial::RENDERER_MULTIPASS, EG::Graphics::RenderingMaterial::RENDERING_PHASE_LIGHTING_SHADER)){
							custom_shader = true;
							shaders->Unbind();
							shaders->Bind(material->GetShader(EG::Graphics::RenderingMaterial::RENDERER_MULTIPASS, EG::Graphics::RenderingMaterial::RENDERING_PHASE_LIGHTING_SHADER));

							shaders->SetMatrix4("projection_matrix", camera->GetProjectionMatrix());
							shaders->SetMatrix4("view_matrix", camera->GetViewMatrix());
							shaders->SetFloat3("camera_position", camera_position);
							shaders->SetInt("normal_map", 0);
							shaders->SetInt("height_map", 1);

							// Ambient Pass
							shaders->SetFloat3("light_color", 0.0f, 0.0f, 0.0f);
							shaders->SetFloat4("light_position", 0.0f, 0.0f, 0.0f, 1.0f);
							shaders->SetFloat3("light_attenuation", 1.0f, 1.0f, 1.0f);
							shaders->SetFloat("light_radius", 0.0000001f);
						}*/

						if (material->HasTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_NORMAL)){
							graphics->BindTexture(scene->GetTextureManager()->GetTexture(material->GetTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_NORMAL))->GetId(), 0);
						}else if (material->HasCubeMap(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_NORMAL)){
							graphics->BindCubeMap(scene->GetTextureManager()->GetCubeMap(material->GetCubeMap(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_NORMAL))->GetId(), 0);
						}else{
							graphics->BindTexture(scene->GetTextureManager()->GetTexture("default_normal")->GetId(), 0);
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

						/*if (custom_shader){
							shaders->Unbind();
							shaders->Bind("lighting");

							shaders->SetMatrix4("projection_matrix", camera->GetProjectionMatrix());
							shaders->SetMatrix4("view_matrix", camera->GetViewMatrix());
							shaders->SetFloat3("camera_position", camera_position);
							shaders->SetInt("normal_map", 0);
							shaders->SetInt("height_map", 1);

							// Ambient Pass
							shaders->SetFloat3("light_color", 0.0f, 0.0f, 0.0f);
							shaders->SetFloat4("light_position", 0.0f, 0.0f, 0.0f, 1.0f);
							shaders->SetFloat3("light_attenuation", 1.0f, 1.0f, 1.0f);
							shaders->SetFloat("light_radius", 0.0000001f);
						}*/
					}
					++mesh_attribute_iterator;
				}
				++object_iterator;
			}

			std::map<std::string, EG::Game::Object *> *light_objects = scene->GetObjectManager()->GetObjects();
			std::map<std::string, EG::Game::Object *>::iterator light_object_iterator = light_objects->begin();
			while (light_object_iterator != light_objects->end()){
				EG::Game::Object *light_object = light_object_iterator->second;
				std::vector<EG::Game::ObjectAttribute *> *light_attributes = light_object->GetAttributesByType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_EMISSION_LIGHT);
				if (light_attributes->size() > 0){
					std::vector<EG::Game::ObjectAttribute *>::iterator light_attribute_iterator = light_attributes->begin();
					while (light_attribute_iterator != light_attributes->end()){
						EG::Game::ObjectAttributeEmissionLight *light_attribute = static_cast<EG::Game::ObjectAttributeEmissionLight *>(*light_attribute_iterator);
						EG::Graphics::Light *light = light_attribute->GetLight();

						// Matrices seem inappropriate now... hmm
						glm::vec3 lp = light->GetPosition();
						glm::vec4 light_position = glm::vec4(lp.x, lp.y, lp.z, 1.0f);
						light_position.w = 1.0f;
						//glm::vec4 light_view = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
						shaders->SetFloat3("light_color", light->GetColor());
						shaders->SetFloat4("light_position", light_position);
						shaders->SetFloat3("light_attenuation", light->GetAttenuation());
						shaders->SetFloat("light_radius", light->GetRadius());

						// Render Objects
						std::map<std::string, EG::Game::Object *> *objects = scene->GetObjectManager()->GetObjects();
						std::map<std::string, EG::Game::Object *>::iterator object_iterator = objects->begin();
						object_iterator = objects->begin();
						while (object_iterator != objects->end()){
							EG::Game::Object *object = object_iterator->second;

							// Meshes
							std::vector<EG::Game::ObjectAttribute *> *mesh_attributes = object->GetAttributesByType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_RENDERING_MESH);
							std::vector<EG::Game::ObjectAttribute *>::iterator mesh_attribute_iterator = mesh_attributes->begin();
							while (mesh_attribute_iterator != mesh_attributes->end()){
								EG::Game::ObjectAttributeRenderingMesh *mesh_attribute = static_cast<EG::Game::ObjectAttributeRenderingMesh *>(*mesh_attribute_iterator);
								EG::Graphics::RenderingMaterial *material = mesh_attribute->GetMaterial();

								if (material->GetLit()){
									/*bool custom_shader = false;
									if (material->HasShader(EG::Graphics::RenderingMaterial::RENDERER_MULTIPASS, EG::Graphics::RenderingMaterial::RENDERING_PHASE_LIGHTING_SHADER)){
										custom_shader = true;
										shaders->Unbind();
										shaders->Bind(material->GetShader(EG::Graphics::RenderingMaterial::RENDERER_MULTIPASS, EG::Graphics::RenderingMaterial::RENDERING_PHASE_LIGHTING_SHADER));
										//shaders->Bind("sphere_cube_map_gradient_decal_with_lighting");

										shaders->SetMatrix4("projection_matrix", camera->GetProjectionMatrix());
										shaders->SetMatrix4("view_matrix", camera->GetViewMatrix());
										shaders->SetFloat3("camera_position", camera_position);
										shaders->SetInt("normal_map", 0);
										shaders->SetInt("height_map", 1);

										shaders->SetFloat3("light_color", light->GetColor());
										shaders->SetFloat4("light_position", light_position);
										shaders->SetFloat3("light_attenuation", light->GetAttenuation());
										shaders->SetFloat("light_radius", light->GetRadius());
									}*/

									if (material->HasTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_NORMAL)){
										graphics->BindTexture(scene->GetTextureManager()->GetTexture(material->GetTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_NORMAL))->GetId(), 0);
									}else if (material->HasCubeMap(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_NORMAL)){
										graphics->BindCubeMap(scene->GetTextureManager()->GetCubeMap(material->GetCubeMap(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_NORMAL))->GetId(), 0);
									}else{
										graphics->BindTexture(scene->GetTextureManager()->GetTexture("default_normal")->GetId(), 0);
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

									/*if (custom_shader){
										shaders->Unbind();
										shaders->Bind("lighting");

										shaders->SetMatrix4("projection_matrix", camera->GetProjectionMatrix());
										shaders->SetMatrix4("view_matrix", camera->GetViewMatrix());
										shaders->SetFloat3("camera_position", camera_position);
										shaders->SetInt("normal_map", 0);
										shaders->SetInt("height_map", 1);

										shaders->SetFloat3("light_color", light->GetColor());
										shaders->SetFloat4("light_position", light_position);
										shaders->SetFloat3("light_attenuation", light->GetAttenuation());
										shaders->SetFloat("light_radius", light->GetRadius());
									}*/
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
			graphics->EndOffscreenRender();

			graphics->StartOffscreenRender(color_buffer->GetBufferId(), 0, 800, 500);

			// Non Lit Objects (like lights)
			shaders->Bind("textured");
			shaders->SetMatrix4("projection_matrix", camera->GetProjectionMatrix());
			shaders->SetMatrix4("view_matrix", camera->GetViewMatrix());
			shaders->SetInt("decal", 0);
			shaders->SetInt("height", 1);
			object_iterator = objects->begin();
			while (object_iterator != objects->end()){
				EG::Game::Object *object = object_iterator->second;

				// Meshes
				std::vector<EG::Game::ObjectAttribute *> *mesh_attributes = object->GetAttributesByType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_RENDERING_MESH);
				std::vector<EG::Game::ObjectAttribute *>::iterator mesh_attribute_iterator = mesh_attributes->begin();
				while (mesh_attribute_iterator != mesh_attributes->end()){
					EG::Game::ObjectAttributeRenderingMesh *mesh_attribute = static_cast<EG::Game::ObjectAttributeRenderingMesh *>(*mesh_attribute_iterator);
					EG::Graphics::RenderingMaterial *material = mesh_attribute->GetMaterial();
					bool custom_shader = false;
					if (material->HasShader(EG::Graphics::RenderingMaterial::RENDERER_MULTIPASS, EG::Graphics::RenderingMaterial::RENDERING_PHASE_TEXTURED_SHADER)){
						custom_shader = true;
						shaders->Unbind();
						shaders->Bind(material->GetShader(EG::Graphics::RenderingMaterial::RENDERER_MULTIPASS, EG::Graphics::RenderingMaterial::RENDERING_PHASE_TEXTURED_SHADER));
						//shaders->Bind("sphere_cube_map_gradient_decal");

						shaders->SetMatrix4("projection_matrix", camera->GetProjectionMatrix());
						shaders->SetMatrix4("view_matrix", camera->GetViewMatrix());
						shaders->SetInt("decal", 0);
						shaders->SetInt("height", 1);
					}

					// Transformation
					std::vector<EG::Game::ObjectAttribute *> *transformation_attributes = object->GetAttributesByType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_BASIC_TRANSFORMATION);
					EG::Game::ObjectAttributeBasicTransformation *transformation_attribute = static_cast<EG::Game::ObjectAttributeBasicTransformation *>(transformation_attributes->at(0));
					glm::mat4 transformation = transformation_attribute->GetTransformation();

					glm::mat4 normal_matrix = EG::Math::Utility::GenerateNormalMatrix(transformation);
					shaders->SetMatrix4("model_matrix", transformation);
					shaders->SetMatrix4("normal_matrix", normal_matrix);

					glm::vec4 color = material->GetColor();
					shaders->SetFloat4("color", color);
					if (material->HasTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_DECAL)){
						graphics->BindTexture(scene->GetTextureManager()->GetTexture(material->GetTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_DECAL))->GetId());
					}else if (material->HasCubeMap(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_DECAL)){
						graphics->BindCubeMap(scene->GetTextureManager()->GetCubeMap(material->GetCubeMap(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_DECAL))->GetId());
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

					++mesh_attribute_iterator;
				}

				++object_iterator;
			}
			shaders->Unbind();
			graphics->EndOffscreenRender();
			graphics->EndFrame();

			// 2D Part
			if (output_type == EG::Graphics::RendererMultipass::MULTIPASS_OUTPUT_BLOOM){
				Bloom();
				shaders->Bind("debug");
				shaders->SetMatrix4("projection_matrix", orthographics_projection_matrix);
				shaders->SetMatrix4("view_matrix", glm::mat4(1.0f));
				shaders->SetInt("out_map", 0);
				shaders->SetMatrix4("model_matrix", glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 250.0f, 0.0f)), glm::vec3(400.0f, 250.0f, 1.0f)));
				graphics->BindTexture(light_buffer->GetTextureId(0), 0);
				rectangle->Draw();
				shaders->SetMatrix4("model_matrix", glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(400.0f, 250.0f, 0.0f)), glm::vec3(400.0f, 250.0f, 1.0f)));
				graphics->BindTexture(bloom_buffer->GetTextureId(0), 0);
				rectangle->Draw();
				shaders->SetMatrix4("model_matrix", glm::scale(glm::mat4(1.0f), glm::vec3(400.0f, 250.0f, 1.0f)));
				graphics->BindTexture(bloom_buffer->GetTextureId(1), 0);
				rectangle->Draw();
				shaders->SetMatrix4("model_matrix", glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(400.0f, 0.0f, 0.0f)), glm::vec3(400.0f, 250.0f, 1.0f)));
				graphics->BindTexture(bloom_buffer->GetTextureId(2), 0);
				rectangle->Draw();
				shaders->Unbind();
			}else if (output_type == EG::Graphics::RendererMultipass::MULTIPASS_OUTPUT_PREPASS){
				Bloom();
				shaders->Bind("debug");
				shaders->SetMatrix4("projection_matrix", orthographics_projection_matrix);
				shaders->SetMatrix4("view_matrix", glm::mat4(1.0f));
				shaders->SetInt("out_map", 0);
				shaders->SetMatrix4("model_matrix", glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 250.0f, 0.0f)), glm::vec3(400.0f, 250.0f, 1.0f)));
				graphics->BindTexture(color_buffer->GetTextureId(0), 0);
				rectangle->Draw();
				shaders->SetMatrix4("model_matrix", glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(400.0f, 250.0f, 0.0f)), glm::vec3(400.0f, 250.0f, 1.0f)));
				graphics->BindTexture(light_buffer->GetTextureId(0), 0);
				rectangle->Draw();
				shaders->SetMatrix4("model_matrix", glm::scale(glm::mat4(1.0f), glm::vec3(400.0f, 250.0f, 1.0f)));
				graphics->BindTexture(bloom_buffer->GetTextureId(0), 0);
				rectangle->Draw();
				shaders->SetMatrix4("model_matrix", glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(400.0f, 0.0f, 0.0f)), glm::vec3(400.0f, 250.0f, 1.0f)));
				graphics->BindTexture(bloom_buffer->GetTextureId(2), 0);
				rectangle->Draw();
				shaders->Unbind();
			}else if (output_type == EG::Graphics::RendererMultipass::MULTIPASS_OUTPUT_NORMAL){
				Bloom();
				shaders->Bind("composition");
				shaders->SetMatrix4("projection_matrix", orthographics_projection_matrix);
				shaders->SetMatrix4("view_matrix", glm::mat4(1.0f));
				shaders->SetMatrix4("model_matrix", glm::scale(glm::mat4(1.0f), glm::vec3(800.0f, 500.0f, 1.0f)));
				graphics->BindTexture(color_buffer->GetTextureId(0), 0);
				shaders->SetInt("decal_map", 0);
				graphics->BindTexture(light_buffer->GetTextureId(0), 1);
				shaders->SetInt("light_map", 1);
				graphics->BindTexture(bloom_buffer->GetTextureId(2), 1);
				shaders->SetInt("bloom_map", 2);
				rectangle->Draw();
				shaders->Unbind();
			}
		}

		void RendererMultipass::Bloom(void){
			glDisable(GL_BLEND);
			glm::mat4 model_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(800.0f / 4.0f, 500.0f / 4.0f, 1.0f));
			glm::vec2 size = glm::vec2(800.0f / 4.0f, 500.0f / 4.0f);

			graphics->StartOffscreenRender(bloom_buffer->GetBufferId(), 0, 800, 500);
			shaders->Bind("convolution");
			graphics->BindTexture(light_buffer->GetTextureId(0), 0);
			shaders->SetInt("map_in", 0);
			shaders->SetFloat2("size", size);
			shaders->SetMatrix4("projection_matrix", orthographics_projection_matrix);
			shaders->SetMatrix4("view_matrix", glm::mat4(1.0f));
			shaders->SetMatrix4("model_matrix", model_matrix);
			rectangle->Draw();
			shaders->Unbind();
			graphics->EndOffscreenRender();

			graphics->StartOffscreenRender(bloom_buffer->GetBufferId(), 1, 800, 500);
			shaders->Bind("gaussian_h");
			graphics->BindTexture(bloom_buffer->GetTextureId(0), 0);
			shaders->SetInt("map_in", 0);
			shaders->SetFloat2("size", size);
			shaders->SetMatrix4("projection_matrix", orthographics_projection_matrix);
			shaders->SetMatrix4("view_matrix", glm::mat4(1.0f));
			shaders->SetMatrix4("model_matrix", model_matrix);
			rectangle->Draw();
			shaders->Unbind();
			graphics->EndOffscreenRender();

			graphics->StartOffscreenRender(bloom_buffer->GetBufferId(), 2, 800, 500);
			shaders->Bind("gaussian_v");
			graphics->BindTexture(bloom_buffer->GetTextureId(1), 0);
			shaders->SetInt("map_in", 0);
			shaders->SetFloat2("size", size);
			shaders->SetMatrix4("projection_matrix", orthographics_projection_matrix);
			shaders->SetMatrix4("view_matrix", glm::mat4(1.0f));
			shaders->SetMatrix4("model_matrix", model_matrix);
			rectangle->Draw();
			shaders->Unbind();
			graphics->EndOffscreenRender();

			// Luminance
			/*StartOffscreenRender(luminance_hdr_buffer, FBO_COLOR_0, width, height, true);
			shaders->Bind("luminance");
			graphics_utility->ActiveTexture(0);
			shaders->SetInt("light_buffer", 0);
			glBindTexture(GL_TEXTURE_2D, in);
			graphics_utility->ActiveTexture(1);
			shaders->SetInt("bloom_buffer", 1);
			glBindTexture(GL_TEXTURE_2D, bloom_buffer->GetTexture(2));
			graphics_utility->DrawTexturedRectangle(0, 0, 16, 16);
			shaders->Unbind();
			EndOffscreenRender(luminance_hdr_buffer, true, width, height);

			graphics_utility->SetTexture(luminance_hdr_buffer->GetTexture(0), 0);
			graphics_utility->GetTextureDataFloats(luminance_buffer, 0);
			graphics_utility->SetTexture(0, 0); // Unbind the texture
			luminance_color.Zero();
			unsigned int luminance_index = 0;
			luminance_time_multiplier = 0.1f;
			for (unsigned int luminance_x_index = luminance_buffer_sample_min_x; luminance_x_index < luminance_buffer_sample_max_x; luminance_x_index++){
				for (unsigned int luminance_y_index = luminance_buffer_sample_min_y; luminance_y_index < luminance_buffer_sample_max_y; luminance_y_index++){
					luminance_index = (luminance_y_index * 16 * 4) + (luminance_x_index * 4);
					luminance_color += EG::Base::Math::Vector3f(luminance_buffer[luminance_index], luminance_buffer[luminance_index + 1], luminance_buffer[luminance_index + 2]);
				}
			}

			luminance_color /= (luminance_buffer_sample_max_x - luminance_buffer_sample_min_x) * (luminance_buffer_sample_max_y - luminance_buffer_sample_min_y);
			target_luminance = luminance_color.Average();
			if (luminance <= target_luminance + 0.025 && luminance >= target_luminance - 0.025){
				luminance = target_luminance;
			}else if (luminance > target_luminance){
				luminance -= EG::Base::Utility::GlobalTimer::GetFrameTime() * luminance_time_multiplier;
			}else if (luminance < target_luminance){
				luminance += EG::Base::Utility::GlobalTimer::GetFrameTime() * luminance_time_multiplier;
			}
			graphics_utility->ErrorCheck("Bloom End");*/
		}

		void RendererMultipass::CycleOutputType(void){
			if (output_type == EG::Graphics::RendererMultipass::MULTIPASS_OUTPUT_BLOOM){
				output_type = EG::Graphics::RendererMultipass::MULTIPASS_OUTPUT_NORMAL;
			}else if (output_type == EG::Graphics::RendererMultipass::MULTIPASS_OUTPUT_NORMAL){
				output_type = EG::Graphics::RendererMultipass::MULTIPASS_OUTPUT_PREPASS;
			}else if (output_type == EG::Graphics::RendererMultipass::MULTIPASS_OUTPUT_PREPASS){
				output_type = EG::Graphics::RendererMultipass::MULTIPASS_OUTPUT_BLOOM;
			}
		}
	}
}
