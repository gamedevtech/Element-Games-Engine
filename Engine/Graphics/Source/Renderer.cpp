#include "../Renderer.h"

#include <iostream>
#include <sstream>

#include "../../Game/ObjectRenderingAttribute.h"
#include "../../Game/ObjectBasicAttribute.h"
#include "../../Game/ObjectEmissionAttribute.h"
#include "../../Graphics/MeshManager.h"
#include "../../Graphics/TextureManager.h"
#include "../../Graphics/MeshGenerator.h"
#include "../../Utility/Font.h"
#include "../../Utility/Text.h"

namespace EG{
    namespace Graphics{
        Renderer::Renderer(void){
            initialized = false;

            current_shader_id = "";

            // defaults for values only used in deferred renderer
            ssao_enabled = 0;
            bloom_enabled = 0;
            normal_mapping_enabled = 0;
            shadows_enabled = 0;
            dof_enabled = 0;
            shadow_mapping_bias = glm::mat4(1.0f);
            luminance_scale = 1.0f;
            bloom_scale = 1.0f;
            luminance = 1.0f;
        }

        Renderer::~Renderer(void){
            if (initialized){
                delete shaders;
            }
        }

        void Renderer::Initialize(void){
            shaders = new EG::Graphics::ShaderManager();
            shaders->Add("billboarding", "Shaders/Basic/billboarding.vert", "Shaders/Basic/billboarding.frag");
            shaders->Add("lighting", "Shaders/Basic/lighting.vert", "Shaders/Basic/lighting.frag");
            shaders->Add("textured", "Shaders/Basic/textured.vert", "Shaders/Basic/textured.frag");
            shaders->Add("font_rendering", "Shaders/Basic/font_rendering.vert", "Shaders/Basic/font_rendering.frag");
            shaders->Add("sphere_cube_map_gradient_decal", "Shaders/Basic/sphere_cube_mapped_with_gradient_decal.vert", "Shaders/Basic/sphere_cube_mapped_with_gradient_decal.frag");
            shaders->Add("sphere_cube_map_gradient_decal_with_lighting", "Shaders/Basic/sphere_cube_mapped_with_gradient_decal_with_lighting.vert", "Shaders/Basic/sphere_cube_mapped_with_gradient_decal_with_lighting.frag");
            shaders->Add("planet_atmosphere", "Shaders/Basic/planet_atmosphere.vert", "Shaders/Basic/planet_atmosphere.frag");

            orthographics_projection_matrix = glm::gtc::matrix_transform::ortho(0.0f, float(graphics->GetViewportWidth()), 0.0f, float(graphics->GetViewportHeight()));

            initialized = true;
        }

        void Renderer::RenderLitObject(EG::Game::Scene *scene, EG::Graphics::Light *light, EG::Game::Object *object){
            EG::Graphics::Camera *camera = scene->GetCurrentCamera();
            // Meshes
            glm::vec3 lp = light->GetPosition();
            glm::vec4 light_position = glm::vec4(lp.x, lp.y, lp.z, 1.0f);
            if (object->HasAttributesOfType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_RENDERING_MESH)){
                std::vector<EG::Game::ObjectAttribute *> *mesh_attributes = object->GetAttributesByType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_RENDERING_MESH);
                std::vector<EG::Game::ObjectAttribute *>::iterator mesh_attribute_iterator = mesh_attributes->begin();
                while (mesh_attribute_iterator != mesh_attributes->end()){
                    EG::Game::ObjectAttributeRenderingMesh *mesh_attribute = static_cast<EG::Game::ObjectAttributeRenderingMesh *>(*mesh_attribute_iterator);
                    EG::Graphics::RenderingMaterial *material = mesh_attribute->GetMaterial();
                    if (material->GetLit()){
                        bool custom_shader = false;

                        // Transformation
                        std::vector<EG::Game::ObjectAttribute *> *transformation_attributes = object->GetAttributesByType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_BASIC_TRANSFORMATION);
                        EG::Game::ObjectAttributeBasicTransformation *transformation_attribute = static_cast<EG::Game::ObjectAttributeBasicTransformation *>(transformation_attributes->at(0));
                        glm::mat4 transformation = transformation_attribute->GetTransformation();

                        if (material->HasShader(EG::Graphics::RenderingMaterial::RENDERER_BASIC, EG::Graphics::RenderingMaterial::RENDERING_PHASE_LIGHTING_SHADER)){
                            custom_shader = true;
                            shaders->Unbind();
                            std::string shader_id = material->GetShader(EG::Graphics::RenderingMaterial::RENDERER_BASIC, EG::Graphics::RenderingMaterial::RENDERING_PHASE_LIGHTING_SHADER);
                            current_shader_id = shader_id;
                            shaders->Bind(current_shader_id);
                            BindShaderBeginUniforms(current_shader_id, scene, light);
                        } else {
                            if (current_shader_id != "lighting") {
                                current_shader_id = "lighting";
                                shaders->Unbind();
                                shaders->Bind(current_shader_id);
                                BindShaderBeginUniforms(current_shader_id, scene, light); // TODO: REUSE THIS WHERE THE SHADER IS INITIALLY BOUND TOO!
                            }
                        }

                        BindCustomShaderUniforms(object, current_shader_id);
                        BindEngineShaderUniforms(object, current_shader_id, "Lighting", transformation, material, scene);

                        EG::Graphics::Mesh *mesh = scene->GetMeshManager()->Get(mesh_attribute->GetMeshId());
                        if (mesh){
                            mesh->Draw();
                        }
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
                        RenderLitObject(scene, light, *piter);
                        ++piter;
                    }
                    ++attr_iter;
                }
            }
        }

        void Renderer::RenderObject(EG::Game::Scene *scene, EG::Game::Object *object){
            EG::Graphics::Camera *camera = scene->GetCurrentCamera();
            // Meshes
            if (object->HasAttributesOfType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_RENDERING_MESH)){
                std::vector<EG::Game::ObjectAttribute *> *mesh_attributes = object->GetAttributesByType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_RENDERING_MESH);
                std::vector<EG::Game::ObjectAttribute *>::iterator mesh_attribute_iterator = mesh_attributes->begin();
                while (mesh_attribute_iterator != mesh_attributes->end()){
                    EG::Game::ObjectAttributeRenderingMesh *mesh_attribute = static_cast<EG::Game::ObjectAttributeRenderingMesh *>(*mesh_attribute_iterator);
                    EG::Graphics::RenderingMaterial *material = mesh_attribute->GetMaterial();

                    graphics->SetBlendingMode(material->GetBlendingMode());

                    // Transformation
                    std::vector<EG::Game::ObjectAttribute *> *transformation_attributes = object->GetAttributesByType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_BASIC_TRANSFORMATION);
                    EG::Game::ObjectAttributeBasicTransformation *transformation_attribute = static_cast<EG::Game::ObjectAttributeBasicTransformation *>(transformation_attributes->at(0));
                    glm::mat4 transformation = transformation_attribute->GetTransformation();

                    if (!material->GetLit()){
                        bool custom_shader = false;
                        bool billboarding_shader = false;
                        if (material->HasShader(EG::Graphics::RenderingMaterial::RENDERER_BASIC, EG::Graphics::RenderingMaterial::RENDERING_PHASE_TEXTURED_SHADER)){
                            std::string shader_id = material->GetShader(EG::Graphics::RenderingMaterial::RENDERER_BASIC, EG::Graphics::RenderingMaterial::RENDERING_PHASE_TEXTURED_SHADER);
                            if (shader_id != current_shader_id) {
                                current_shader_id = shader_id;
                                custom_shader = true;
                                shaders->Unbind();
                                shaders->Bind(current_shader_id);
                                BindShaderBeginUniforms(current_shader_id, scene, NULL); // TODO: REUSE THIS WHERE THE SHADER IS INITIALLY BOUND TOO!
                            }

                            // TODO: Make this go away!
                            if (material->GetShader(EG::Graphics::RenderingMaterial::RENDERER_BASIC, EG::Graphics::RenderingMaterial::RENDERING_PHASE_TEXTURED_SHADER) == "billboarding"){
                                //glDisable(GL_DEPTH_TEST);
                                glDepthMask(GL_FALSE);
                                billboarding_shader = true;
                            }
                        } else {
                            if (current_shader_id != "textured") {
                                current_shader_id = "textured";
                                shaders->Unbind();
                                shaders->Bind(current_shader_id);
                                BindShaderBeginUniforms(current_shader_id, scene, NULL); // TODO: REUSE THIS WHERE THE SHADER IS INITIALLY BOUND TOO!
                            }
                        }
                        BindCustomShaderUniforms(object, current_shader_id);
                        BindEngineShaderUniforms(object, current_shader_id, "Textured", transformation, material, scene);

                        EG::Graphics::Mesh *mesh = scene->GetMeshManager()->Get(mesh_attribute->GetMeshId());
                        if (mesh){
                            mesh->Draw();
                        }

                        if (billboarding_shader){
                            glDepthMask(GL_TRUE);
                        }
                        graphics->SetBlendingMode();
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
                        RenderObject(scene, *piter);
                        ++piter;
                    }
                    ++attr_iter;
                }
            }
        }

        void Renderer::BindShaderBeginUniforms(std::string shader_id, EG::Game::Scene *scene, EG::Graphics::Light *light){
            std::vector<EG::Graphics::ShaderManager::EngineUniforms> *uniforms = shaders->GetEngineUniforms(shader_id);
            if (uniforms == NULL){
                return;
            }

            EG::Graphics::Camera *camera = scene->GetCurrentCamera();
            std::vector<EG::Graphics::ShaderManager::EngineUniforms>::iterator uniform_iter = uniforms->begin();
            //std::cout << phase << ": " << shader_id << std::endl;
            while (uniform_iter != uniforms->end()){
                EG::Graphics::ShaderManager::EngineUniforms variable = (*uniform_iter);
                if (variable == EG::Graphics::ShaderManager::ENGINE_CAMERA_MATRIX) {
                    shaders->SetMatrix4("camera_matrix", camera->GetViewMatrix());
                }else if (variable == EG::Graphics::ShaderManager::ENGINE_VIEW_MATRIX) {
                    shaders->SetMatrix4("view_matrix", camera->GetViewMatrix());
                }else if (variable == EG::Graphics::ShaderManager::ENGINE_PROJECTION_MATRIX) {
                    shaders->SetMatrix4("projection_matrix", camera->GetProjectionMatrix());
                }else if (variable == EG::Graphics::ShaderManager::ENGINE_PERSPECTIVE_MATRIX) {
                    shaders->SetMatrix4("perspective_matrix", camera->GetProjectionMatrix());
                }else if (variable == EG::Graphics::ShaderManager::ENGINE_ORTHO_MATRIX) {
                    shaders->SetMatrix4("ortho_matrix", orthographics_projection_matrix);
                }else if (variable == EG::Graphics::ShaderManager::ENGINE_CAMERA_POSITION) {
                    shaders->SetFloat3("camera_position", camera->GetPosition());
                }else if (variable == EG::Graphics::ShaderManager::ENGINE_LIGHT_ATTENUATION) {
                    shaders->SetFloat3("light_attenuation", light->GetAttenuation());
                }else if (variable == EG::Graphics::ShaderManager::ENGINE_LIGHT_COLOR) {
                    shaders->SetFloat3("light_color", light->GetColor());
                }else if (variable == EG::Graphics::ShaderManager::ENGINE_LIGHT_POSITION) {
                    glm::vec3 lp = light->GetPosition();
                    glm::vec4 light_position = glm::vec4(lp.x, lp.y, lp.z, 1.0f);
                    shaders->SetFloat4("light_position", light_position);
                }else if (variable == EG::Graphics::ShaderManager::ENGINE_LIGHT_RADIUS) {
                    shaders->SetFloat("light_radius", light->GetRadius());
                }else if (variable == EG::Graphics::ShaderManager::ENGINE_NORMAL_MAPPING_ENABLED) {
                    shaders->SetInt("normal_mapping_enabled", normal_mapping_enabled);
                }else if (variable == EG::Graphics::ShaderManager::ENGINE_SHADOW_MAPPING_ENABLED) {
                    shaders->SetInt("shadow_mapping_enabled", shadows_enabled);
                }else if (variable == EG::Graphics::ShaderManager::ENGINE_SSAO_ENABLED) {
                    shaders->SetInt("ssao_enabled", ssao_enabled);
                }else if (variable == EG::Graphics::ShaderManager::ENGINE_BLOOM_ENABLED) {
                    shaders->SetInt("bloom_enabled", bloom_enabled);
                }else if (variable == EG::Graphics::ShaderManager::ENGINE_SHADOW_MAPPING_BIAS) {
                    shaders->SetMatrix4("shadow_mapping_bias", shadow_mapping_bias);
                }else if (variable == EG::Graphics::ShaderManager::ENGINE_LUMINANCE) {
                    shaders->SetInt("luminance", luminance);
                }else if (variable == EG::Graphics::ShaderManager::ENGINE_LUMINANCE_SCALE) {
                    shaders->SetInt("luminance_scale", luminance_scale);
                }else if (variable == EG::Graphics::ShaderManager::ENGINE_BLOOM_SCALE) {
                    shaders->SetInt("bloom_scale", bloom_scale);
                }
                ++uniform_iter;
            }
        }

        // phase = ["obj", "lit", "2d"]
        // associated data = lights or frame buffers or whatever is needed
        void Renderer::BindEngineShaderUniforms(EG::Game::Object *object, std::string shader_id, std::string phase, glm::mat4 transform, EG::Graphics::RenderingMaterial *material, EG::Game::Scene *scene){
            std::vector<EG::Graphics::ShaderManager::EngineUniforms> *uniforms = shaders->GetEngineUniforms(shader_id);
            if (uniforms == NULL){
                return;
            }

            std::vector<EG::Graphics::ShaderManager::EngineUniforms>::iterator uniform_iter = uniforms->begin();
            while (uniform_iter != uniforms->end()){
                EG::Graphics::ShaderManager::EngineUniforms variable = (*uniform_iter);
                if (variable == EG::Graphics::ShaderManager::ENGINE_MATERIAL_COLOR) {
                    shaders->SetFloat4("material_color", material->GetColor());
                }else if (variable == EG::Graphics::ShaderManager::ENGINE_MATERIAL_SPECULARITY) {
                    shaders->SetFloat("material_specularity", material->GetSpecular());
                }else if (variable == EG::Graphics::ShaderManager::ENGINE_MODEL_MATRIX) {
                    shaders->SetMatrix4("model_matrix", transform);
                }else if (variable == EG::Graphics::ShaderManager::ENGINE_NORMAL_MATRIX) {
                    glm::mat4 normal_matrix = EG::Math::Utility::GenerateNormalMatrix(transform);
                    shaders->SetMatrix4("normal_matrix", normal_matrix);
                }else if (variable == EG::Graphics::ShaderManager::ENGINE_DECAL_MAP) {
                    shaders->SetInt("decal_map", 0);
                    if (material->HasTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_DECAL)){
                        graphics->BindTexture(scene->GetTextureManager()->GetTexture(material->GetTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_DECAL))->GetId());
                    }else if (material->HasCubeMap(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_HEIGHT)){
                        graphics->BindCubeMap(scene->GetTextureManager()->GetCubeMap(material->GetTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_DECAL))->GetId());
                    }else{
                        graphics->BindTexture(scene->GetTextureManager()->GetTexture("default_decal")->GetId());
                    }
                }else if (variable == EG::Graphics::ShaderManager::ENGINE_NORMAL_MAP) {
                    shaders->SetInt("normal_map", 1);
                    if (material->HasTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_NORMAL)){
                        graphics->BindTexture(scene->GetTextureManager()->GetTexture(material->GetTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_NORMAL))->GetId(), 1);
                    }else if (material->HasCubeMap(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_NORMAL)){
                        graphics->BindCubeMap(scene->GetTextureManager()->GetCubeMap(material->GetCubeMap(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_NORMAL))->GetId(), 1);
                    }else{
                        graphics->BindTexture(scene->GetTextureManager()->GetTexture("default_normal")->GetId(), 1);
                    }
                }else if (variable == EG::Graphics::ShaderManager::ENGINE_HEIGHT_MAP) {
                    shaders->SetInt("height_map", 2);
                    if (material->HasTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_HEIGHT)){
                        graphics->BindTexture(scene->GetTextureManager()->GetTexture(material->GetTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_HEIGHT))->GetId(), 2);
                    }else if (material->HasCubeMap(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_HEIGHT)){
                        graphics->BindCubeMap(scene->GetTextureManager()->GetCubeMap(material->GetCubeMap(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_HEIGHT))->GetId(), 2);
                    }else{
                        graphics->BindTexture(scene->GetTextureManager()->GetTexture("default_height")->GetId(), 2);
                    }
                }else if (variable == EG::Graphics::ShaderManager::ENGINE_SPECULAR_MAP) {
                    shaders->SetInt("height_map", 3);
                    if (material->HasTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_SPECULAR)){
                        graphics->BindTexture(scene->GetTextureManager()->GetTexture(material->GetTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_SPECULAR))->GetId(), 3);
                    }else if (material->HasCubeMap(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_SPECULAR)){
                        graphics->BindCubeMap(scene->GetTextureManager()->GetCubeMap(material->GetCubeMap(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_SPECULAR))->GetId(), 3);
                    }else{
                        graphics->BindTexture(scene->GetTextureManager()->GetTexture("default_specular")->GetId(), 3);
                    }
                }else if (variable == EG::Graphics::ShaderManager::ENGINE_RECEIVES_LIGHTING) {
                    int r = 0;
                    if (material->GetLit()) {
                        r = 1;
                    }
                    shaders->SetInt("receives_lighting", r);
                }
                ++uniform_iter;
            }
        }

        /*
        ENGINE_SHADOW_MAPPING_SIZE = 18, // NOTE: Not used yet it seems!
        ENGINE_POSITION_BUFFER = 19,
        ENGINE_NORMAL_BUFFER = 20,
        ENGINE_TRANSPARENT_BUFFER = 21,
        ENGINE_LIGHT_BUFFER = 22,
        ENGINE_COLOR_BUFFER = 23,
        ENGINE_BLOOM_BUFFER = 24,
        ENGINE_SSAO_BUFFER = 25,
        ENGINE_DEPTH_MAP = 31,
        ENGINE_RESOLUTION = 32,
        ENGINE_NEAR_FAR = 33,
        */

        void Renderer::BindCustomShaderUniforms(EG::Game::Object *object, std::string shader_id){
            std::vector<std::pair<std::string, EG::Graphics::ShaderManager::ShaderUniformTypes> > *uniforms = shaders->GetObjectUniforms(shader_id);
            if (uniforms == NULL) {
                return;
            }
            std::cout << shader_id << ": " << object->GetObjectName() << std::endl;
            std::vector<std::pair<std::string, EG::Graphics::ShaderManager::ShaderUniformTypes> >::iterator uniform_iter = uniforms->begin();
            std::vector<EG::Game::ObjectAttribute *> *attrs;
            std::vector<EG::Game::ObjectAttribute *>::iterator attrs_iter;
            while (uniform_iter != uniforms->end()){
                std::pair<std::string, EG::Graphics::ShaderManager::ShaderUniformTypes> variable = (*uniform_iter);
                if (variable.second == EG::Graphics::ShaderManager::UNIFORM_INT) {
                    // Seems this should be stored in a better format.
                    attrs = object->GetAttributesByType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_BASIC_INTEGER);
                    attrs_iter = attrs->begin();
                    bool found = false;
                    EG::Game::ObjectAttributeBasicInteger *needed_int;
                    while (attrs_iter != attrs->end() && !found) {
                        EG::Game::ObjectAttributeBasicInteger *int_attr = static_cast<EG::Game::ObjectAttributeBasicInteger *>((*attrs_iter));
                        if (int_attr->GetKey() == variable.first) {
                            needed_int = int_attr;
                            found = true;
                        }
                        ++attrs_iter;
                    }
                    if (found) {
                        shaders->SetInt(variable.first.c_str(), needed_int->GetValue());
                    } else {
                        std::cout << "Couldn't find attribute (" << variable.second << ") in integer attributes of object " << object->GetObjectName() << std::endl;
                    }
                } else if (variable.second == EG::Graphics::ShaderManager::UNIFORM_FLOAT) {
                    // Seems this should be stored in a better format.
                    attrs = object->GetAttributesByType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_BASIC_FLOAT);
                    attrs_iter = attrs->begin();
                    bool found = false;
                    EG::Game::ObjectAttributeBasicFloat *needed_float;
                    while (attrs_iter != attrs->end() && !found) {
                        EG::Game::ObjectAttributeBasicFloat *float_attr = static_cast<EG::Game::ObjectAttributeBasicFloat *>((*attrs_iter));
                        if (float_attr->GetKey() == variable.first) {
                            needed_float = float_attr;
                            found = true;
                        }
                        ++attrs_iter;
                    }
                    if (found) {
                        shaders->SetFloat(variable.first.c_str(), needed_float->GetValue());
                    } else {
                        std::cout << "Couldn't find attribute (" << variable.second << ") in float attributes of object " << object->GetObjectName() << std::endl;
                    }
                }
                ++uniform_iter;
            }
        }

        void Renderer::Render(EG::Game::Scene *scene, EG::Utility::Time *time){
            float frame_time = time->GetFrameTime();
            graphics->BeginFrame();
            EG::Graphics::Camera *camera = scene->GetCurrentCamera();
            camera->Update();

            glDepthFunc(GL_LEQUAL);
            glEnable(GL_BLEND);
            glBlendFunc(GL_ONE, GL_ONE);

            current_shader_id = "lighting";
            shaders->Bind(current_shader_id);

            EG::Graphics::Light blank_light;
            blank_light.SetAttenuation(glm::vec3(1.0f, 1.0f, 1.0f));
            blank_light.SetCastsShadows(false);
            blank_light.SetColor(glm::vec3(0.0f, 0.0f, 0.0f));
            blank_light.SetRadius(0.0000001f);
            BindShaderBeginUniforms(current_shader_id, scene, &blank_light);

            // Render Objects
            // TODO: Sort by shader, by depth?
            EG::Utility::UnsignedIntDictionary<EG::Game::Object *> *objects = scene->GetObjectManager()->GetObjects();
            EG::Utility::UnsignedIntDictionaryKeysIterator object_iterator = objects->GetKeysBegin();
            while (object_iterator != objects->GetKeysEnd()){
                EG::Game::Object *object = objects->Get(*object_iterator);
                RenderLitObject(scene, &blank_light, object);
                ++object_iterator;
            }

            EG::Utility::UnsignedIntDictionary<EG::Game::Object *> *light_objects = scene->GetObjectManager()->GetObjects();
            EG::Utility::UnsignedIntDictionaryKeysIterator light_object_iterator = objects->GetKeysBegin();
            while (light_object_iterator != light_objects->GetKeysEnd()){
                EG::Game::Object *light_object = light_objects->Get(*light_object_iterator);
                if (light_object->HasAttributesOfType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_EMISSION_LIGHT)){
                    std::vector<EG::Game::ObjectAttribute *> *light_attributes = light_object->GetAttributesByType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_EMISSION_LIGHT);
                    std::vector<EG::Game::ObjectAttribute *>::iterator light_attribute_iterator = light_attributes->begin();
                    while (light_attribute_iterator != light_attributes->end()){
                        EG::Game::ObjectAttributeEmissionLight *light_attribute = static_cast<EG::Game::ObjectAttributeEmissionLight *>(*light_attribute_iterator);
                        EG::Graphics::Light *light = light_attribute->GetLight();

                        object_iterator = objects->GetKeysBegin();
                        while (object_iterator != objects->GetKeysEnd()){
                            EG::Game::Object *object = objects->Get(*object_iterator);
                            BindShaderBeginUniforms(current_shader_id, scene, light);
                            RenderLitObject(scene, light, object);
                            ++object_iterator;
                        }
                        ++light_attribute_iterator;
                    }
                }
                ++light_object_iterator;
            }
            shaders->Unbind();

            // Non Lit Objects (like lights)
            current_shader_id = "textured";
            shaders->Bind(current_shader_id);
            BindShaderBeginUniforms(current_shader_id, scene, NULL);
            object_iterator = objects->GetKeysBegin();

            while (object_iterator != objects->GetKeysEnd()){
                EG::Game::Object *object = objects->Get(*object_iterator);
                RenderObject(scene, object);
                ++object_iterator;
            }

            shaders->Unbind();

            graphics->EndFrame();

            // 2D Overlays
            glDisable(GL_DEPTH_TEST);
            shaders->Bind("font_rendering");
            shaders->SetMatrix4("projection_matrix", orthographics_projection_matrix);
            shaders->SetMatrix4("view_matrix", glm::mat4(1.0f));
            shaders->SetMatrix4("model_matrix", glm::gtx::transform::scale(float(graphics->GetViewportWidth()), float(graphics->GetViewportHeight()), 1.0f));
            shaders->SetInt("decal_map", 0);
            shaders->SetInt("use_decal", 1);
            shaders->SetFloat4("material_color", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
            glEnable(GL_BLEND);
            glEnable(GL_TEXTURE_2D);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            if (gui->GetInitialized()){
                gui->Render();
                graphics->BindTexture(gui->GetTextureId(), 0);
                scene->GetMeshManager()->Get("rectangle")->Draw();
            }

            glDisable(GL_BLEND);
            glEnable(GL_DEPTH_TEST);
            shaders->Unbind();

            shaders->Bind("font_rendering");
            shaders->SetMatrix4("projection_matrix", orthographics_projection_matrix);
            shaders->SetMatrix4("view_matrix", glm::mat4(1.0f));
            shaders->SetMatrix4("model_matrix", glm::gtx::transform::translate(glm::vec3(10.0f, 10.0f, 0.0f)));

            graphics->SetActiveTexture(0);
            shaders->SetInt("decal_map", 0);
            shaders->SetFloat4("material_color", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

            std::stringstream campos;
            campos.precision(3);
            campos << camera->GetPosition().x << ", " << camera->GetPosition().y << ", " << camera->GetPosition().z;
            campos.flush();
            font_manager->Draw(campos.str());

            shaders->SetMatrix4("model_matrix", glm::gtx::transform::translate(glm::vec3(10.0f, 24.0f, 0.0f)));
            std::stringstream fps;
            fps.precision(3);
            fps << time->GetFPS();
            fps.flush();
            font_manager->Draw(fps.str());

            shaders->Unbind();

            // LOGO
            glDisable(GL_DEPTH_TEST);
            glEnable(GL_BLEND);
            glEnable(GL_TEXTURE_2D);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            shaders->Bind("textured");
            glm::mat4 scale_mat = glm::scale(glm::mat4(1.0f), glm::vec3(128.0f, 128.0f, 128.0f));
            glm::mat4 trans_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 32.0f, 0.0f));
            shaders->SetMatrix4("model_matrix", trans_mat * scale_mat);
            shaders->SetMatrix4("projection_matrix", orthographics_projection_matrix);
            shaders->SetMatrix4("view_matrix", glm::mat4(1.0f));
            shaders->SetInt("decal_map", 0);
            shaders->SetInt("height_map", 1);
            shaders->SetFloat4("material_color", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
            graphics->BindTexture(scene->GetTextureManager()->GetTexture("eglogo")->GetId(), 0);
            scene->GetMeshManager()->Get("rectangle")->Draw();
            shaders->Unbind();
            glDisable(GL_BLEND);
            glEnable(GL_DEPTH_TEST);
            // END LOGO
        }
    }
}
