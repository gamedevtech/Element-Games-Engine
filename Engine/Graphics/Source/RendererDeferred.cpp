#include "../RendererDeferred.h"

#include <iostream>
#include <sstream>

#include "../MeshGenerator.h"
#include "../../Game/ObjectRenderingAttribute.h"
#include "../../Game/ObjectBasicAttribute.h"
#include "../../Game/ObjectEmissionAttribute.h"

namespace EG{
    namespace Graphics{
        RendererDeferred::RendererDeferred(void) : Renderer(){
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
            light_sphere = EG::Graphics::GenerateSphere(4, 4);
            output_type = EG::Graphics::RendererDeferred::DEFERRED_OUTPUT_NORMAL;
            shadow_mapping_offset_0 = 1.1f;
            shadow_mapping_offset_1 = 4.0f;
            shadow_mapping_bias = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f)), glm::vec3(0.5f, 0.5f, 0.5f));
        }

        RendererDeferred::~RendererDeferred(void){
            if (initialized){
                delete shaders;
            }
        }

        void RendererDeferred::Initialize(void){
            shaders = new EG::Graphics::ShaderManager();
            shaders->Add("prepass", "Shaders/Deferred/prepass.vert", "Shaders/Deferred/prepass.frag", "", "", "", 4);
            shaders->Add("planet_atmosphere", "Shaders/Deferred/planet_atmosphere.vert", "Shaders/Deferred/planet_atmosphere.frag", "", "", "", 4);
            shaders->Add("prepass_debug", "Shaders/Deferred/prepass_debug.vert", "Shaders/Deferred/prepass_debug.frag");
            shaders->Add("font_rendering", "Shaders/Basic/font_rendering.vert", "Shaders/Basic/font_rendering.frag");
            shaders->Add("lighting", "Shaders/Deferred/lighting.vert", "Shaders/Deferred/lighting.frag");
            shaders->Add("composition", "Shaders/Deferred/composition.vert", "Shaders/Deferred/composition.frag");
            shaders->Add("convolution", "Shaders/Deferred/convolution.vert", "Shaders/Deferred/convolution.frag");
            shaders->Add("gaussian_v", "Shaders/Deferred/gaussian_v.vert", "Shaders/Deferred/gaussian_v.frag");
            shaders->Add("gaussian_h", "Shaders/Deferred/gaussian_h.vert", "Shaders/Deferred/gaussian_h.frag");
            shaders->Add("ssao", "Shaders/Deferred/ssao.vert", "Shaders/Deferred/ssao.frag");
            shaders->Add("luminance", "Shaders/Deferred/luminance.vert", "Shaders/Deferred/luminance.frag");
            shaders->Add("shadow_map", "Shaders/Deferred/shadow_map.vert", "Shaders/Deferred/shadow_map.frag");
            shaders->Add("depth_debug", "Shaders/Deferred/depth_debug.vert", "Shaders/Deferred/depth_debug.frag");
            shaders->Add("gui_rendering", "Shaders/Deferred/font_rendering.vert", "Shaders/Deferred/font_rendering.frag");
            shaders->Add("dof", "Shaders/Deferred/dof.vert", "Shaders/Deferred/dof.frag");
            shaders->Add("billboarding", "Shaders/Deferred/billboarding.vert", "Shaders/Deferred/billboarding.frag", "", "", "", 4);
            if (graphics->CheckVersion(4, 1)) {
                shaders->Add("sphere_cube_mapped_gradient_decal_prepass", "Shaders/Deferred/sphere_cube_mapped_gradient_decal_prepass.vert", "Shaders/Deferred/sphere_cube_mapped_gradient_decal_prepass.frag", "", "Shaders/Deferred/sphere_cube_mapped_gradient_decal_prepass.cont", "Shaders/Deferred/sphere_cube_mapped_gradient_decal_prepass.eval", 4);
            } else {
                shaders->Add("sphere_cube_mapped_gradient_decal_prepass", "Shaders/Deferred/sphere_cube_mapped_gradient_decal_prepass_no_tessellation.vert", "Shaders/Deferred/sphere_cube_mapped_gradient_decal_prepass_no_tessellation.frag", "", "", "", 4);
            }

            deferred_buffer = new EG::Graphics::OffscreenBuffer(graphics->GetViewportWidth(), graphics->GetViewportHeight(), 4, true, EG::Graphics::OffscreenBuffer::OFFSCREEN_BUFFER_FILTERING_NONE);
            light_buffer = new EG::Graphics::OffscreenBuffer(graphics->GetViewportWidth(), graphics->GetViewportHeight(), 1, true, EG::Graphics::OffscreenBuffer::OFFSCREEN_BUFFER_FILTERING_LINEAR);
            bloom_buffer = new EG::Graphics::OffscreenBuffer(graphics->GetViewportWidth() / 4.0f, graphics->GetViewportHeight() / 4.0f, 3, true, EG::Graphics::OffscreenBuffer::OFFSCREEN_BUFFER_FILTERING_LINEAR);
            hdr_buffer = new EG::Graphics::OffscreenBuffer(16, 16, 1, true, EG::Graphics::OffscreenBuffer::OFFSCREEN_BUFFER_FILTERING_LINEAR);
            ssao_buffer = new EG::Graphics::OffscreenBuffer(graphics->GetViewportWidth() / 2.0f, graphics->GetViewportHeight() / 2.0f, 3, true, EG::Graphics::OffscreenBuffer::OFFSCREEN_BUFFER_FILTERING_LINEAR);
            composition_buffer = new EG::Graphics::OffscreenBuffer(graphics->GetViewportWidth(), graphics->GetViewportHeight(), 1, true, EG::Graphics::OffscreenBuffer::OFFSCREEN_BUFFER_FILTERING_LINEAR);
            orthographics_projection_matrix = glm::ortho(0.0f, float(graphics->GetViewportWidth()), 0.0f, float(graphics->GetViewportHeight()));

            /* Renderer Settings */
            ssao_enabled = 0;
            bloom_enabled = 1;
            normal_mapping_enabled = 1;
            shadows_enabled = 1;
            dof_enabled = 0;
            // HDR
            luminance_buffer_sample_min_x = 4;
            luminance_buffer_sample_min_y = 4;
            luminance_buffer_sample_max_x = 12;
            luminance_buffer_sample_max_y = 12;
            luminance = 0.0f;
            luminance_scale = 0.35f;
            // Bloom
            bloom_scale = 0.5f;
            // ETC

            initialized = true;
        }

        void RendererDeferred::Render(EG::Game::Scene *scene, EG::Utility::Time *time){
            EG::Graphics::Camera *camera = scene->GetCurrentCamera();
            frame_time = time->GetFrameTime();
            graphics->BeginFrame();

            ShadowMapping(scene);

            camera->Update();

            Prepass(scene);
            Lighting(scene);

            graphics->EndOffscreenRender();

            graphics->EndFrame();
            ComposeScene(scene);
            Overlays(scene, time);
        }

        void RendererDeferred::RenderObject(EG::Game::Scene *scene, EG::Game::Object *object){
            EG::Graphics::Camera *camera = scene->GetCurrentCamera();
            // Meshes
            if (object->HasAttributesOfType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_RENDERING_MESH)){
                std::vector<EG::Game::ObjectAttribute *> *mesh_attributes = object->GetAttributesByType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_RENDERING_MESH);
                std::vector<EG::Game::ObjectAttribute *>::iterator mesh_attribute_iterator = mesh_attributes->begin();
                while (mesh_attribute_iterator != mesh_attributes->end()){
                    EG::Game::ObjectAttributeRenderingMesh *mesh_attribute = static_cast<EG::Game::ObjectAttributeRenderingMesh *>(*mesh_attribute_iterator);
                    EG::Graphics::RenderingMaterial *material = mesh_attribute->GetMaterial();
                    bool tessellation_shader = false;
                    bool custom_shader = false;
                    bool billboarding_shader = false;
                    if (material->HasShader(EG::Graphics::RenderingMaterial::RENDERER_DEFERRED, EG::Graphics::RenderingMaterial::RENDERING_PHASE_PREPASS_SHADER)){
                        std::string shader_id = material->GetShader(EG::Graphics::RenderingMaterial::RENDERER_DEFERRED, EG::Graphics::RenderingMaterial::RENDERING_PHASE_PREPASS_SHADER);
                        if (shaders->Has(shader_id) && shader_id != current_shader_id) {
                            custom_shader = true;
                            shaders->Unbind();
                            current_shader_id = shader_id;
                            shaders->Bind(current_shader_id);
                            if (current_shader_id == "sphere_cube_mapped_gradient_decal_prepass" && graphics->CheckVersion(4, 1)) {
                                tessellation_shader = true;
                            }
                            BindShaderBeginUniforms(current_shader_id, scene, NULL);
                        }

                        // HACK: This should really be reading some other attribute, so others can write their own billboarding shaders!!!
                        if (material->GetShader(EG::Graphics::RenderingMaterial::RENDERER_DEFERRED, EG::Graphics::RenderingMaterial::RENDERING_PHASE_PREPASS_SHADER) == "billboarding"){
                            billboarding_shader = true;
                            // HACK: Assuming particle systems for now!
                            //glDisable(GL_DEPTH_TEST);
                            glDepthMask(GL_FALSE);
                        }
                        graphics->SetBlendingMode(material->GetBlendingMode());
                    } else {
                        if (current_shader_id != "prepass") {
                            current_shader_id = "prepass";
                            shaders->Unbind();
                            shaders->Bind(current_shader_id);
                            BindShaderBeginUniforms(current_shader_id, scene, NULL); // TODO: REUSE THIS WHERE THE SHADER IS INITIALLY BOUND TOO!
                        }
                    }

                    // Transformation
                    std::vector<EG::Game::ObjectAttribute *> *transformation_attributes = object->GetAttributesByType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_BASIC_TRANSFORMATION);
                    EG::Game::ObjectAttributeBasicTransformation *transformation_attribute = static_cast<EG::Game::ObjectAttributeBasicTransformation *>(transformation_attributes->at(0));
                    glm::mat4 transformation = transformation_attribute->GetTransformation();
                    BindEngineShaderUniforms(object, current_shader_id, "Prepass", transformation, material, scene);
                    BindCustomShaderUniforms(object, current_shader_id);

                    EG::Graphics::Mesh *mesh = scene->GetMeshManager()->Get(mesh_attribute->GetMeshId());
                    if (mesh){
                        if (tessellation_shader){
                            graphics->SetUsingTessellation(true);
                        }
                        mesh->Draw();
                        if (tessellation_shader){
                            graphics->SetUsingTessellation(false);
                        }
                    }

                    if (billboarding_shader){
                        //glDisable(GL_DEPTH_TEST);
                        glDepthMask(GL_TRUE);
                    }
                    graphics->SetBlendingMode();
                    ++mesh_attribute_iterator;
                }
            }

            if (object->HasAttributesOfType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_EMISSION_PARTICLE_SYSTEM)){
                std::vector<EG::Game::ObjectAttribute *> *attrs = object->GetAttributesByType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_EMISSION_PARTICLE_SYSTEM);
                std::vector<EG::Game::ObjectAttribute *>::iterator attr_iter = attrs->begin();
                while (attr_iter != attrs->end()){
                    EG::Game::ObjectAttributeEmissionParticleSystem *pattr = static_cast<EG::Game::ObjectAttributeEmissionParticleSystem *>(*attr_iter);
                    EG::Graphics::ParticleSystem *psys = pattr->GetParticleSystem();
                    std::list<EG::Graphics::Particle *>::iterator piter = psys->GetParticles()->begin();
                    while (piter != psys->GetParticles()->end()){
                        RenderObject(scene, *piter);
                        ++piter;
                    }
                    ++attr_iter;
                }
            }
        }

        void RendererDeferred::Prepass(EG::Game::Scene *scene){
            EG::Graphics::Camera *camera = scene->GetCurrentCamera();
            int draw_buffers[] = {0, 1, 2, 3};
            graphics->StartMultiBufferOffscreenRender(deferred_buffer->GetBufferId(), 4, draw_buffers, graphics->GetViewportWidth(), graphics->GetViewportHeight());
            current_shader_id = "prepass";
            shaders->Bind(current_shader_id);
            BindShaderBeginUniforms(current_shader_id, scene, NULL);

            // Render Objects
            EG::Utility::UnsignedIntDictionary<EG::Game::Object *> *objects = scene->GetObjectManager()->GetObjects();
            EG::Utility::UnsignedIntDictionaryKeysIterator object_iterator = objects->GetKeysBegin();
            while (object_iterator != objects->GetKeysEnd()){
                EG::Game::Object *object = objects->Get(*object_iterator);
                RenderObject(scene, object);
                ++object_iterator;
            }
            graphics->EndMultiBufferOffscreenRender();
        }

        void RendererDeferred::CalculateLighting(EG::Game::Scene *scene, EG::Graphics::Light *light){
            EG::Graphics::Camera *camera = scene->GetCurrentCamera();
            glm::vec3 lp = light->GetPosition();
            glm::vec4 light_position = glm::vec4(lp.x, lp.y, lp.z, 1.0f);
            float light_radius = light->GetRadius();
            //glm::mat4 model_matrix = glm::scale(light_transformation, glm::vec3(light_radius * 2.0f, light_radius * 2.0f, light_radius * 2.0f));
            //glm::mat4 normal_matrix = EG::Math::Utility::GenerateNormalMatrix(model_matrix);
            //EG::Math::Utility::PrintMat4(model_matrix);
            shaders->SetFloat3("light_color", light->GetColor());
            shaders->SetFloat3("light_position", light_position.x, light_position.y, light_position.z);
            shaders->SetFloat3("light_attenuation", light->GetAttenuation());
            shaders->SetFloat("light_radius", light_radius);

            // Shadow Mapping
            if (light->GetCastsShadows() && shadows_enabled == 1){
                graphics->SetActiveTexture(2);
                shaders->SetInt("shadow_mapping_enabled", 1);
                glm::mat4 shadow_mapping_transformation = shadow_mapping_bias * light->GetProjectionMatrix() * light->GetViewMatrix();
                shaders->SetMatrix4("shadow_mapping_bias", shadow_mapping_transformation);
                graphics->BindTexture(light->GetShadowBuffer()->GetDepthTextureId(), 2);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE_ARB);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
                graphics->SetActiveTexture(0);
                shaders->SetFloat2("shadow_map_size", glm::vec2(light->GetShadowMapResolution(), light->GetShadowMapResolution()));
            }else{
                shaders->SetInt("shadow_mapping_enabled", 0);
                shaders->SetMatrix4("shadow_mapping_bias", glm::mat4(1.0f));
                graphics->BindTexture(scene->GetTextureManager()->GetTexture("default_decal")->GetId(), 2);
            }

            // Sphere Method
            //shaders->SetMatrix4("model_matrix", model_matrix);
            //shaders->SetMatrix4("normal_matrix", normal_matrix);
            //light_sphere->Draw();

            // Quad Method
            rectangle->Draw();

            if (light->GetCastsShadows() && shadows_enabled == 1){
                graphics->SetActiveTexture(2);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
                graphics->SetActiveTexture(0);
            }
        }

        void RendererDeferred::Lighting(EG::Game::Scene *scene){
            EG::Graphics::Camera *camera = scene->GetCurrentCamera();
            graphics->StartOffscreenRender(light_buffer->GetBufferId(), 0, graphics->GetViewportWidth(), graphics->GetViewportHeight());
            shaders->Bind("lighting");

            // Sphere Method
            //shaders->SetMatrix4("projection_matrix", camera->GetProjectionMatrix());
            //shaders->SetMatrix4("view_matrix", camera->GetViewMatrix());
            //shaders->SetMatrix4("model_matrix", glm::mat4(1.0f));
            //shaders->SetMatrix4("normal_matrix", glm::mat4(1.0f));

            // Ortho Quad Method
            shaders->SetMatrix4("shadow_mapping_bias", glm::mat4(1.0f));
            shaders->SetInt("shadow_mapping_enabled", 0);
            shaders->SetInt("shadow_map", 2);
            shaders->SetFloat2("shadow_map_size", glm::vec2(512.0f, 512.0f));

            shaders->SetMatrix4("projection_matrix", orthographics_projection_matrix);
            shaders->SetMatrix4("view_matrix", glm::mat4(1.0f));
            shaders->SetMatrix4("model_matrix", glm::scale(glm::mat4(1.0f), glm::vec3(float(graphics->GetViewportWidth()), float(graphics->GetViewportHeight()), 1.0f)));
            shaders->SetMatrix4("normal_matrix", glm::mat4(1.0f));

            shaders->SetFloat2("resolution", glm::vec2(float(graphics->GetViewportWidth()), float(graphics->GetViewportHeight())));
            shaders->SetInt("position_map", 0);
            shaders->SetInt("normal_map", 1);
            shaders->SetFloat3("camera_position", camera->GetPosition());
            shaders->SetFloat3("light_position", glm::vec3(0.0f, 0.0f, 0.0f));
            shaders->SetFloat3("light_color", glm::vec3(1.0f, 1.0f, 1.0f));
            shaders->SetFloat3("light_attenuation", glm::vec3(1.0f, 1.0f, 1.0f));
            shaders->SetFloat("light_radius", 0.0f);

            graphics->BindTexture(deferred_buffer->GetTextureId(0), 0);
            graphics->BindTexture(deferred_buffer->GetTextureId(2), 1);

            glDepthFunc(GL_LEQUAL);
            glEnable(GL_BLEND);
            glBlendFunc(GL_ONE, GL_ONE);

            EG::Utility::UnsignedIntDictionary<EG::Game::Object *> *light_objects = scene->GetObjectManager()->GetObjects();
            EG::Utility::UnsignedIntDictionaryKeysIterator light_object_iterator = light_objects->GetKeysBegin();
            while (light_object_iterator != light_objects->GetKeysEnd()){
                EG::Game::Object *light_object = light_objects->Get(*light_object_iterator);
                if (light_object->HasAttributesOfType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_EMISSION_LIGHT)){
                    std::vector<EG::Game::ObjectAttribute *> *light_attributes = light_object->GetAttributesByType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_EMISSION_LIGHT);
                    std::vector<EG::Game::ObjectAttribute *>::iterator light_attribute_iterator = light_attributes->begin();
                    while (light_attribute_iterator != light_attributes->end()){
                        //glPushAttrib(GL_TEXTURE_BIT);
                        EG::Game::ObjectAttributeEmissionLight *light_attribute = static_cast<EG::Game::ObjectAttributeEmissionLight *>(*light_attribute_iterator);
                        EG::Graphics::Light *light = light_attribute->GetLight();
                        CalculateLighting(scene, light);
                        ++light_attribute_iterator;
                        //glPopAttrib();
                    }
                }

                if (light_object->HasAttributesOfType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_EMISSION_PARTICLE_SYSTEM)){
                    std::vector<EG::Game::ObjectAttribute *> *attrs = light_object->GetAttributesByType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_EMISSION_PARTICLE_SYSTEM);
                    std::vector<EG::Game::ObjectAttribute *>::iterator attr_iter = attrs->begin();
                    while (attr_iter != attrs->end()){
                        EG::Game::ObjectAttributeEmissionParticleSystem *pattr = static_cast<EG::Game::ObjectAttributeEmissionParticleSystem *>(*attr_iter);
                        EG::Graphics::ParticleSystem *psys = pattr->GetParticleSystem();
                        std::list<EG::Graphics::Particle *>::iterator piter = psys->GetParticles()->begin();
                        while (piter != psys->GetParticles()->end()){
                            EG::Graphics::Particle *p = (*piter);
                            if (p->HasAttributesOfType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_EMISSION_LIGHT)){
                                std::vector<EG::Game::ObjectAttribute *> *light_attributes = p->GetAttributesByType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_EMISSION_LIGHT);
                                std::vector<EG::Game::ObjectAttribute *>::iterator light_attribute_iterator = light_attributes->begin();
                                while (light_attribute_iterator != light_attributes->end()){
                                    //glPushAttrib(GL_TEXTURE_BIT);
                                    EG::Game::ObjectAttributeEmissionLight *light_attribute = static_cast<EG::Game::ObjectAttributeEmissionLight *>(*light_attribute_iterator);
                                    EG::Graphics::Light *light = light_attribute->GetLight();
                                    CalculateLighting(scene, light);
                                    ++light_attribute_iterator;
                                    //glPopAttrib();
                                }
                            }
                            ++piter;
                        }
                        ++attr_iter;
                    }
                }

                ++light_object_iterator;
            }
            glDisable(GL_BLEND);

            shaders->Unbind();
        }

        void RendererDeferred::ComposeScene(EG::Game::Scene *scene){
            EG::Graphics::Camera *camera = scene->GetCurrentCamera();
            // 2D Output
            if (output_type == EG::Graphics::RendererDeferred::DEFERRED_OUTPUT_PREPASS){
                shaders->Bind("prepass_debug");
                shaders->SetMatrix4("projection_matrix", orthographics_projection_matrix);
                shaders->SetMatrix4("view_matrix", glm::mat4(1.0f));
                shaders->SetInt("out_map", 0);
                shaders->SetMatrix4("model_matrix", glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, (graphics->GetViewportHeight() / 2.0f), 0.0f)), glm::vec3((graphics->GetViewportWidth() / 2.0f), (graphics->GetViewportHeight() / 2.0f), 1.0f)));
                graphics->BindTexture(deferred_buffer->GetTextureId(0), 0); // position, specular
                rectangle->Draw();
                shaders->SetMatrix4("model_matrix", glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3((graphics->GetViewportWidth() / 2.0f), (graphics->GetViewportHeight() / 2.0f), 0.0f)), glm::vec3((graphics->GetViewportWidth() / 2.0f), (graphics->GetViewportHeight() / 2.0f), 1.0f)));
                graphics->BindTexture(deferred_buffer->GetTextureId(1), 0); // color
                rectangle->Draw();
                shaders->SetMatrix4("model_matrix", glm::scale(glm::mat4(1.0f), glm::vec3((graphics->GetViewportWidth() / 2.0f), (graphics->GetViewportHeight() / 2.0f), 1.0f)));
                graphics->BindTexture(deferred_buffer->GetTextureId(2), 0); // normal, height
                rectangle->Draw();
                shaders->SetMatrix4("model_matrix", glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3((graphics->GetViewportWidth() / 2.0f), 0.0f, 0.0f)), glm::vec3((graphics->GetViewportWidth() / 2.0f), (graphics->GetViewportHeight() / 2.0f), 1.0f)));
                graphics->BindTexture(deferred_buffer->GetTextureId(3), 0); // translucent
                rectangle->Draw();
                shaders->Unbind();
            }else if (output_type == EG::Graphics::RendererDeferred::DEFERRED_OUTPUT_BLOOM){
                Bloom();
                shaders->Bind("prepass_debug");
                shaders->SetMatrix4("projection_matrix", orthographics_projection_matrix);
                shaders->SetMatrix4("view_matrix", glm::mat4(1.0f));
                shaders->SetInt("out_map", 0);
                shaders->SetMatrix4("model_matrix", glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, (graphics->GetViewportHeight() / 2.0f), 0.0f)), glm::vec3((graphics->GetViewportWidth() / 2.0f), (graphics->GetViewportHeight() / 2.0f), 1.0f)));
                graphics->BindTexture(light_buffer->GetTextureId(0), 0);
                rectangle->Draw();
                shaders->SetMatrix4("model_matrix", glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3((graphics->GetViewportWidth() / 2.0f), (graphics->GetViewportHeight() / 2.0f), 0.0f)), glm::vec3((graphics->GetViewportWidth() / 2.0f), (graphics->GetViewportHeight() / 2.0f), 1.0f)));
                graphics->BindTexture(bloom_buffer->GetTextureId(0), 0);
                rectangle->Draw();
                shaders->SetMatrix4("model_matrix", glm::scale(glm::mat4(1.0f), glm::vec3((graphics->GetViewportWidth() / 2.0f), (graphics->GetViewportHeight() / 2.0f), 1.0f)));
                graphics->BindTexture(bloom_buffer->GetTextureId(1), 0);
                rectangle->Draw();
                shaders->SetMatrix4("model_matrix", glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3((graphics->GetViewportWidth() / 2.0f), 0.0f, 0.0f)), glm::vec3((graphics->GetViewportWidth() / 2.0f), (graphics->GetViewportHeight() / 2.0f), 1.0f)));
                graphics->BindTexture(bloom_buffer->GetTextureId(2), 0);
                rectangle->Draw();
                shaders->Unbind();
            }else if (output_type == EG::Graphics::RendererDeferred::DEFERRED_OUTPUT_SSAO){
                SSAO();
                shaders->Bind("prepass_debug");
                shaders->SetMatrix4("projection_matrix", orthographics_projection_matrix);
                shaders->SetMatrix4("view_matrix", glm::mat4(1.0f));
                shaders->SetInt("out_map", 0);
                shaders->SetMatrix4("model_matrix", glm::scale(glm::mat4(1.0f), glm::vec3(float(graphics->GetViewportWidth()), float(graphics->GetViewportHeight()), 1.0f)));
                graphics->BindTexture(ssao_buffer->GetTextureId(2), 0);
                rectangle->Draw();
                shaders->Unbind();
            }else if (output_type == EG::Graphics::RendererDeferred::DEFERRED_OUTPUT_HDR){
                Bloom();
                shaders->Bind("prepass_debug");
                shaders->SetMatrix4("projection_matrix", orthographics_projection_matrix);
                shaders->SetMatrix4("view_matrix", glm::mat4(1.0f));
                shaders->SetInt("out_map", 0);
                shaders->SetMatrix4("model_matrix", glm::scale(glm::mat4(1.0f), glm::vec3(float(graphics->GetViewportWidth()), float(graphics->GetViewportHeight()), 1.0f)));
                graphics->BindTexture(hdr_buffer->GetTextureId(0), 0);
                rectangle->Draw();
                shaders->Unbind();
            }else if (output_type == EG::Graphics::RendererDeferred::DEFERRED_OUTPUT_SHADOW){
                EG::Graphics::Light *light = debug_shadow_map_light;
                light->GetColor();

                shaders->Bind("depth_debug");
                shaders->SetMatrix4("projection_matrix", orthographics_projection_matrix);
                shaders->SetMatrix4("view_matrix", glm::mat4(1.0f));
                shaders->SetMatrix4("model_matrix", glm::scale(glm::mat4(1.0f), glm::vec3(float(graphics->GetViewportWidth()), float(graphics->GetViewportHeight()), 1.0f)));
                shaders->SetInt("depth_map", 0);
                graphics->BindTexture(light->GetShadowBuffer()->GetDepthTextureId(), 0);
                rectangle->Draw();
                shaders->Unbind();
            }else if (output_type == EG::Graphics::RendererDeferred::DEFERRED_OUTPUT_NORMAL){
                if (bloom_enabled == 1){
                    Bloom();
                }
                if (ssao_enabled == 1){
                    SSAO();
                }

                graphics->StartOffscreenRender(composition_buffer->GetBufferId(), 0, graphics->GetViewportWidth(), graphics->GetViewportHeight());
                shaders->Bind("composition");
                shaders->SetMatrix4("projection_matrix", orthographics_projection_matrix);
                shaders->SetMatrix4("view_matrix", glm::mat4(1.0f));
                shaders->SetMatrix4("model_matrix", glm::scale(glm::mat4(1.0f), glm::vec3(float(graphics->GetViewportWidth()), float(graphics->GetViewportHeight()), 1.0f)));
                shaders->SetInt("color_map", 0);
                shaders->SetInt("light_map", 1);
                shaders->SetInt("bloom_map", 2);
                shaders->SetInt("ssao_map", 3);
                shaders->SetInt("translucent_map", 4);
                shaders->SetFloat("luminance", luminance);
                shaders->SetFloat("luminance_scale", luminance_scale);
                shaders->SetFloat("bloom_scale", bloom_scale);
                shaders->SetInt("ssao_enabled", ssao_enabled);
                shaders->SetInt("bloom_enabled", bloom_enabled);
                graphics->BindTexture(deferred_buffer->GetTextureId(1), 0);
                graphics->BindTexture(light_buffer->GetTextureId(0), 1);
                graphics->BindTexture(bloom_buffer->GetTextureId(2), 2);
                graphics->BindTexture(ssao_buffer->GetTextureId(2), 3);
                graphics->BindTexture(deferred_buffer->GetTextureId(3), 4);
                rectangle->Draw();
                shaders->Unbind();
                graphics->EndOffscreenRender();

                if (dof_enabled == 1){
                    shaders->Bind("dof");
                    shaders->SetMatrix4("projection_matrix", orthographics_projection_matrix);
                    shaders->SetMatrix4("view_matrix", glm::mat4(1.0f));
                    shaders->SetMatrix4("model_matrix", glm::scale(glm::mat4(1.0f), glm::vec3(float(graphics->GetViewportWidth()), float(graphics->GetViewportHeight()), 1.0f)));
                    shaders->SetInt("depth_map", 0);
                    shaders->SetInt("color_map", 1);
                    shaders->SetFloat2("screensize", glm::vec2(float(graphics->GetViewportWidth()), float(graphics->GetViewportHeight())));
                    shaders->SetFloat2("camerarange", 0.1f, 100.0f);
                    shaders->SetFloat2("middle_texcoord", 0.5f, 0.5f);
                    shaders->SetFloat("blur_clamp", 0.025f);
                    shaders->SetFloat("bias", 0.1f);
                    graphics->BindTexture(deferred_buffer->GetDepthTextureId(), 0);
                    graphics->BindTexture(composition_buffer->GetTextureId(0), 1);
                    rectangle->Draw();
                    shaders->Unbind();
                }else{
                    shaders->Bind("prepass_debug");
                    shaders->SetMatrix4("projection_matrix", orthographics_projection_matrix);
                    shaders->SetMatrix4("view_matrix", glm::mat4(1.0f));
                    shaders->SetMatrix4("model_matrix", glm::scale(glm::mat4(1.0f), glm::vec3(float(graphics->GetViewportWidth()), float(graphics->GetViewportHeight()), 1.0f)));
                    shaders->SetInt("out_map", 0);
                    graphics->BindTexture(composition_buffer->GetTextureId(0), 0);
                    rectangle->Draw();
                    shaders->Unbind();
                }
            }
        }

        void RendererDeferred::Overlays(EG::Game::Scene *scene, EG::Utility::Time *time){
            EG::Graphics::Camera *camera = scene->GetCurrentCamera();
            glm::vec3 position = glm::vec3(5.0f, 5.0f, -0.1f);
            glm::vec2 scale = glm::vec2(1.0f, 1.0f);

            glDisable(GL_DEPTH_TEST);

            glEnable(GL_BLEND);
            glEnable(GL_TEXTURE_2D);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            shaders->Bind("font_rendering");
            shaders->SetMatrix4("projection_matrix", orthographics_projection_matrix);
            shaders->SetMatrix4("view_matrix", glm::mat4(1.0f));
            shaders->SetMatrix4("model_matrix", glm::translate(glm::vec3(10.0f, 10.0f, 0.0f)));

            graphics->SetActiveTexture(0);
            shaders->SetInt("decal", 0);
            shaders->SetFloat4("color", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

            std::stringstream campos;
            campos.precision(3);
            campos << camera->GetPosition().x << ", " << camera->GetPosition().y << ", " << camera->GetPosition().z;
            campos.flush();
            font_manager->Draw(campos.str());

            shaders->SetMatrix4("model_matrix", glm::translate(glm::vec3(10.0f, 24.0f, 0.0f)));
            std::stringstream fps;
            fps.precision(3);
            fps << time->GetFPS();
            fps.flush();
            font_manager->Draw(fps.str());

            shaders->Unbind();

            shaders->Bind("gui_rendering");
            shaders->SetMatrix4("projection_matrix", orthographics_projection_matrix);
            shaders->SetMatrix4("view_matrix", glm::mat4(1.0f));
            shaders->SetMatrix4("model_matrix", glm::scale(float(graphics->GetViewportWidth()), float(graphics->GetViewportHeight()), 1.0f));
            shaders->SetInt("decal", 0);
            shaders->SetFloat4("color", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
            if (gui->GetInitialized()){
                gui->Render();
                graphics->BindTexture(gui->GetTextureId());
                scene->GetMeshManager()->Get("rectangle")->Draw();
            }
            glDisable(GL_BLEND);
            shaders->Unbind();
            glEnable(GL_DEPTH_TEST);
        }

        void RendererDeferred::Bloom(void){
            glDisable(GL_BLEND);
            glm::mat4 model_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(float(graphics->GetViewportWidth()) / 4.0f, float(graphics->GetViewportHeight()) / 4.0f, 1.0f));
            glm::vec2 size = glm::vec2(float(graphics->GetViewportWidth()) / 4.0f, float(graphics->GetViewportHeight()) / 4.0f);

            graphics->StartOffscreenRender(bloom_buffer->GetBufferId(), 0, graphics->GetViewportWidth(), graphics->GetViewportHeight());
            shaders->Bind("convolution");
            graphics->BindTexture(light_buffer->GetTextureId(0), 0);
            shaders->SetInt("map_in", 0);
            shaders->SetInt("translucent_map", 1);
            graphics->BindTexture(deferred_buffer->GetTextureId(3), 1);
            shaders->SetFloat2("size", size);
            shaders->SetMatrix4("projection_matrix", orthographics_projection_matrix);
            shaders->SetMatrix4("view_matrix", glm::mat4(1.0f));
            shaders->SetMatrix4("model_matrix", model_matrix);
            rectangle->Draw();
            shaders->Unbind();
            graphics->EndOffscreenRender();

            graphics->StartOffscreenRender(bloom_buffer->GetBufferId(), 1, graphics->GetViewportWidth(), graphics->GetViewportHeight());
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

            graphics->StartOffscreenRender(bloom_buffer->GetBufferId(), 2, graphics->GetViewportWidth(), graphics->GetViewportHeight());
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
            model_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(16.0f, 16.0f, 1.0f));
            graphics->StartOffscreenRender(hdr_buffer->GetBufferId(), 0, graphics->GetViewportWidth(), graphics->GetViewportHeight());
            shaders->Bind("luminance");
            shaders->SetMatrix4("projection_matrix", orthographics_projection_matrix);
            shaders->SetMatrix4("view_matrix", glm::mat4(1.0f));
            shaders->SetMatrix4("model_matrix", model_matrix);
            shaders->SetInt("light_map", 0);
            shaders->SetInt("bloom_map", 1);
            graphics->BindTexture(light_buffer->GetTextureId(0), 0);
            graphics->BindTexture(bloom_buffer->GetTextureId(2), 1);
            rectangle->Draw();
            shaders->Unbind();
            graphics->EndOffscreenRender();

            graphics->BindTexture(hdr_buffer->GetTextureId(0), 0);
            graphics->GetTextureDataFloats(luminance_buffer, 0);
            graphics->BindTexture(0, 0);
            glm::vec3 luminance_color = glm::vec3(0.0f);
            unsigned int luminance_index = 0;
            luminance_time_multiplier = 0.5f;
            for (unsigned int luminance_x_index = luminance_buffer_sample_min_x; luminance_x_index < luminance_buffer_sample_max_x; luminance_x_index++){
                for (unsigned int luminance_y_index = luminance_buffer_sample_min_y; luminance_y_index < luminance_buffer_sample_max_y; luminance_y_index++){
                    luminance_index = (luminance_y_index * 16 * 4) + (luminance_x_index * 4);
                    luminance_color += glm::vec3(luminance_buffer[luminance_index], luminance_buffer[luminance_index + 1], luminance_buffer[luminance_index + 2]);
                }
            }
            luminance_color /= ((luminance_buffer_sample_max_x - luminance_buffer_sample_min_x) * (luminance_buffer_sample_max_y - luminance_buffer_sample_min_y));
            target_luminance = (luminance_color.x + luminance_color.y + luminance_color.z) / 3.0f;

            if (luminance <= target_luminance + 0.025 && luminance >= target_luminance - 0.025){
                luminance = target_luminance;
            }else if (luminance > target_luminance){
                luminance -= frame_time * luminance_time_multiplier;
            }else if (luminance < target_luminance){
                luminance += frame_time * luminance_time_multiplier;
            }
        }

        void RendererDeferred::SSAO(void){
            glDisable(GL_BLEND);
            glm::mat4 model_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(float(graphics->GetViewportWidth()) / 2.0f, float(graphics->GetViewportHeight()) / 2.0f, 1.0f));
            glm::vec2 size = glm::vec2(float(graphics->GetViewportWidth()) / 2.0f, float(graphics->GetViewportHeight()) / 2.0f);

            graphics->StartOffscreenRender(ssao_buffer->GetBufferId(), 0, graphics->GetViewportWidth(), graphics->GetViewportHeight());
            shaders->Bind("ssao");
            graphics->BindTexture(deferred_buffer->GetDepthTextureId(), 0);
            shaders->SetInt("depth_map", 0);
            shaders->SetFloat2("screensize", size);
            shaders->SetFloat2("camerarange", 0.1f, 100.0f);
            shaders->SetMatrix4("projection_matrix", orthographics_projection_matrix);
            shaders->SetMatrix4("view_matrix", glm::mat4(1.0f));
            shaders->SetMatrix4("model_matrix", model_matrix);
            rectangle->Draw();
            shaders->Unbind();
            graphics->EndOffscreenRender();

            graphics->StartOffscreenRender(ssao_buffer->GetBufferId(), 1, graphics->GetViewportWidth(), graphics->GetViewportHeight());
            shaders->Bind("gaussian_h");
            graphics->BindTexture(ssao_buffer->GetTextureId(0), 0);
            shaders->SetInt("map_in", 0);
            shaders->SetFloat2("size", size);
            shaders->SetMatrix4("projection_matrix", orthographics_projection_matrix);
            shaders->SetMatrix4("view_matrix", glm::mat4(1.0f));
            shaders->SetMatrix4("model_matrix", model_matrix);
            rectangle->Draw();
            shaders->Unbind();
            graphics->EndOffscreenRender();

            graphics->StartOffscreenRender(ssao_buffer->GetBufferId(), 2, graphics->GetViewportWidth(), graphics->GetViewportHeight());
            shaders->Bind("gaussian_v");
            graphics->BindTexture(ssao_buffer->GetTextureId(1), 0);
            shaders->SetInt("map_in", 0);
            shaders->SetFloat2("size", size);
            shaders->SetMatrix4("projection_matrix", orthographics_projection_matrix);
            shaders->SetMatrix4("view_matrix", glm::mat4(1.0f));
            shaders->SetMatrix4("model_matrix", model_matrix);
            rectangle->Draw();
            shaders->Unbind();
            graphics->EndOffscreenRender();
        }

        void RendererDeferred::ShadowMapping(EG::Game::Scene *scene){
            EG::Graphics::Camera *camera = scene->GetCurrentCamera();
            if (shadows_enabled == 1){
                // I wonder... Does the depth of the scene's camera have to be the same as the depth in the projection matrix of the shadow?
                // Move To OpenGLInterface
                glEnable(GL_CULL_FACE); // Should be enabled all of the time?
                glCullFace(GL_FRONT);
                glEnable(GL_POLYGON_OFFSET_FILL);
                //glDisable(GL_TEXTURE_2D);
                //glDisable(GL_LIGHTING);
                //glDisable(GL_BLEND);
                glShadeModel(GL_FLAT);
                glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
                glPolygonOffset(shadow_mapping_offset_0, shadow_mapping_offset_1);
                //

                EG::Utility::UnsignedIntDictionary<EG::Game::Object *> *light_objects = scene->GetObjectManager()->GetObjects();
                EG::Utility::UnsignedIntDictionaryKeysIterator light_object_iterator = light_objects->GetKeysBegin();
                while (light_object_iterator != light_objects->GetKeysEnd()){
                    EG::Game::Object *light_object = light_objects->Get(*light_object_iterator);
                    if (light_object->HasAttributesOfType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_EMISSION_LIGHT)){
                        std::vector<EG::Game::ObjectAttribute *> *light_attributes = light_object->GetAttributesByType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_EMISSION_LIGHT);
                        std::vector<EG::Game::ObjectAttribute *>::iterator light_attribute_iterator = light_attributes->begin();
                        while (light_attribute_iterator != light_attributes->end()){
                            EG::Game::ObjectAttributeEmissionLight *light_attribute = static_cast<EG::Game::ObjectAttributeEmissionLight *>(*light_attribute_iterator);
                            EG::Graphics::Light *light = light_attribute->GetLight();
                            if (light->GetCastsShadows()){
                                debug_shadow_map_light = light;
                                light->Update();
                                graphics->StartOffscreenRender(light->GetShadowBuffer()->GetBufferId(), 0, int(light->GetShadowMapResolution()), int(light->GetShadowMapResolution()));
                                shaders->Bind("shadow_map");

                                shaders->SetMatrix4("projection_matrix", light->GetProjectionMatrix());
                                //glm::mat4 light_view_matrix = glm::gtc::matrix_transform::lookAt(light->GetPosition(), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                                shaders->SetMatrix4("view_matrix", light->GetViewMatrix());
                                //shaders->SetMatrix4("view_matrix", light_view_matrix);
                                //shaders->SetMatrix4("projection_matrix", camera->GetProjectionMatrix());
                                //shaders->SetMatrix4("view_matrix", camera->GetViewMatrix());

                                // TODO: Disable Color Writes, ETC... After it's working that is!
                                EG::Utility::UnsignedIntDictionary<EG::Game::Object *> *objects = scene->GetObjectManager()->GetObjects();
                                EG::Utility::UnsignedIntDictionaryKeysIterator object_iterator = objects->GetKeysBegin();
                                while (object_iterator != objects->GetKeysEnd()){
                                    EG::Game::Object *object = objects->Get(*object_iterator);

                                    // HACK: For now, don't render objects with particle systems
                                    if (object->HasAttributesOfType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_RENDERING_MESH)){
                                        // Transformation
                                        std::vector<EG::Game::ObjectAttribute *> *transformation_attributes = object->GetAttributesByType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_BASIC_TRANSFORMATION);
                                        EG::Game::ObjectAttributeBasicTransformation *transformation_attribute = static_cast<EG::Game::ObjectAttributeBasicTransformation *>(transformation_attributes->at(0));
                                        glm::mat4 transformation = transformation_attribute->GetTransformation();
                                        shaders->SetMatrix4("model_matrix", transformation);

                                        std::vector<EG::Game::ObjectAttribute *> *mesh_attributes = object->GetAttributesByType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_RENDERING_MESH);
                                        std::vector<EG::Game::ObjectAttribute *>::iterator mesh_attribute_iterator = mesh_attributes->begin();
                                        while (mesh_attribute_iterator != mesh_attributes->end()){
                                            EG::Game::ObjectAttributeRenderingMesh *mesh_attribute = static_cast<EG::Game::ObjectAttributeRenderingMesh *>(*mesh_attribute_iterator);
                                            EG::Graphics::RenderingMaterial *material = mesh_attribute->GetMaterial();
                                            if (material->GetLit() && material->GetCastsShadows()){
                                                EG::Graphics::Mesh *mesh = scene->GetMeshManager()->Get(mesh_attribute->GetMeshId());
                                                mesh->Draw();
                                                //std::cout << object->GetObjectName() << std::endl;
                                            }

                                            ++mesh_attribute_iterator;
                                        }
                                    }
                                    if (object->HasAttributesOfType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_EMISSION_PARTICLE_SYSTEM)){
                                        std::vector<EG::Game::ObjectAttribute *> *attrs = object->GetAttributesByType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_EMISSION_PARTICLE_SYSTEM);
                                        std::vector<EG::Game::ObjectAttribute *>::iterator attr_iter = attrs->begin();
                                        while (attr_iter != attrs->end()){
                                            EG::Game::ObjectAttributeEmissionParticleSystem *pattr = static_cast<EG::Game::ObjectAttributeEmissionParticleSystem *>(*attr_iter);
                                            EG::Graphics::ParticleSystem *psys = pattr->GetParticleSystem();
                                            std::list<EG::Graphics::Particle *>::iterator piter = psys->GetParticles()->begin();
                                            while (piter != psys->GetParticles()->end()){
                                                EG::Graphics::Particle *object = (*piter);
                                                if (object->HasAttributesOfType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_RENDERING_MESH)){
                                                    // Transformation
                                                    std::vector<EG::Game::ObjectAttribute *> *transformation_attributes = object->GetAttributesByType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_BASIC_TRANSFORMATION);
                                                    EG::Game::ObjectAttributeBasicTransformation *transformation_attribute = static_cast<EG::Game::ObjectAttributeBasicTransformation *>(transformation_attributes->at(0));
                                                    glm::mat4 transformation = transformation_attribute->GetTransformation();
                                                    shaders->SetMatrix4("model_matrix", transformation);

                                                    std::vector<EG::Game::ObjectAttribute *> *mesh_attributes = object->GetAttributesByType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_RENDERING_MESH);
                                                    std::vector<EG::Game::ObjectAttribute *>::iterator mesh_attribute_iterator = mesh_attributes->begin();
                                                    while (mesh_attribute_iterator != mesh_attributes->end()){
                                                        EG::Game::ObjectAttributeRenderingMesh *mesh_attribute = static_cast<EG::Game::ObjectAttributeRenderingMesh *>(*mesh_attribute_iterator);
                                                        EG::Graphics::RenderingMaterial *material = mesh_attribute->GetMaterial();
                                                        if (material->GetLit() && material->GetCastsShadows()){
                                                            EG::Graphics::Mesh *mesh = scene->GetMeshManager()->Get(mesh_attribute->GetMeshId());
                                                            mesh->Draw();
                                                        }

                                                        ++mesh_attribute_iterator;
                                                    }
                                                }
                                                ++piter;
                                            }
                                            ++attr_iter;
                                        }
                                    }

                                    ++object_iterator;
                                }

                                shaders->Unbind();
                                graphics->EndOffscreenRender();
                            }

                            ++light_attribute_iterator;
                        }
                    }

                    ++light_object_iterator;
                }

                // Move to OpenGLInterface
                glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
                glDisable(GL_POLYGON_OFFSET_FILL);
                glDisable(GL_CULL_FACE);
                glCullFace(GL_BACK);
                glShadeModel(GL_SMOOTH);
                //
            }
        }

        void RendererDeferred::CycleOutputType(void){
            if (output_type == EG::Graphics::RendererDeferred::DEFERRED_OUTPUT_BLOOM){
                output_type = EG::Graphics::RendererDeferred::DEFERRED_OUTPUT_HDR;
            }else if (output_type == EG::Graphics::RendererDeferred::DEFERRED_OUTPUT_HDR){
                output_type = EG::Graphics::RendererDeferred::DEFERRED_OUTPUT_SSAO;
            }else if (output_type == EG::Graphics::RendererDeferred::DEFERRED_OUTPUT_SSAO){
                output_type = EG::Graphics::RendererDeferred::DEFERRED_OUTPUT_SHADOW;
            }else if (output_type == EG::Graphics::RendererDeferred::DEFERRED_OUTPUT_SHADOW){
                output_type = EG::Graphics::RendererDeferred::DEFERRED_OUTPUT_NORMAL;
            }else if (output_type == EG::Graphics::RendererDeferred::DEFERRED_OUTPUT_NORMAL){
                output_type = EG::Graphics::RendererDeferred::DEFERRED_OUTPUT_PREPASS;
            }else if (output_type == EG::Graphics::RendererDeferred::DEFERRED_OUTPUT_PREPASS){
                output_type = EG::Graphics::RendererDeferred::DEFERRED_OUTPUT_BLOOM;
            }
        }

        void RendererDeferred::ToggleSSAO(void){
            if (ssao_enabled == 1){
                ssao_enabled = 0;
            }else{
                ssao_enabled = 1;
            }
        }

        void RendererDeferred::ToggleBloom(void){
            if (bloom_enabled == 1){
                bloom_enabled = 0;
            }else{
                bloom_enabled = 1;
            }
        }

        void RendererDeferred::ToggleNormalMapping(void){
            if (normal_mapping_enabled == 1){
                normal_mapping_enabled = 0;
            }else{
                normal_mapping_enabled = 1;
            }
        }

        void RendererDeferred::ToggleShadowMapping(void){
            if (shadows_enabled == 1){
                shadows_enabled = 0;
            }else{
                shadows_enabled = 1;
            }
        }

        void RendererDeferred::ToggleDOF(void){
            if (dof_enabled == 1){
                dof_enabled = 0;
            }else{
                dof_enabled = 1;
            }
        }
    }
}
