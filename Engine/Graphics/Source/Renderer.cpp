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
        TestEmitter::TestEmitter(void) : EG::Graphics::ParticleEmitter(50.0f){}
        TestEmitter::~TestEmitter(void){}
        void TestEmitter::CreateParticle(EG::Graphics::Particle *p){
            p->SetAttribute("frame_count", 0.0f);
            p->SetAttribute("alpha", 0.75f);
            p->SetAttribute("x", EG::Math::Utility::RandomFloat(-0.0025f, 0.0025f));
            p->SetAttribute("y", EG::Math::Utility::RandomFloat(0.0f, 0.0025f));
            p->SetAttribute("z", EG::Math::Utility::RandomFloat(-0.0025f, 0.0025f));
            EG::Graphics::RenderingMaterial *material = new EG::Graphics::RenderingMaterial();
            material->SetCastsShadows(false);
            material->SetDiffuse(1.0f);
            material->SetAmbient(1.0f);
            material->SetSpecular(1.0f);
            material->SetColor(glm::vec4(0.8f, 0.2f, 0.0f, 0.75f));
            material->SetLit(false);
            material->SetTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_DECAL, "particle");
            material->SetShaderOverride(EG::Graphics::RenderingMaterial::RENDERER_BASIC, EG::Graphics::RenderingMaterial::RENDERING_PHASE_TEXTURED_SHADER, "billboarding");
            EG::Game::ObjectAttributeBasicTransformation *transformation = new EG::Game::ObjectAttributeBasicTransformation(glm::scale(glm::mat4(1.0f), glm::vec3(0.2f, 0.2f, 0.2f)));
            p->AddAttribute(transformation);
            p->AddAttribute(new EG::Game::ObjectAttributeRenderingMesh("quad", material));
        }
        TestController::TestController(void){}
        TestController::~TestController(void){}
        void TestController::ControlParticle(EG::Graphics::Particle *p, float frame_time){
            float fc = p->GetAttribute("frame_count");
            fc += 1.0f;
            if (fc > 300.0f){
                p->SetAlive(false);
            }
            p->SetAttribute("frame_count", fc);

            std::vector<EG::Game::ObjectAttribute *> *attributes = p->GetAttributesByType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_BASIC_TRANSFORMATION);
            EG::Game::ObjectAttributeBasicTransformation *transformation = static_cast<EG::Game::ObjectAttributeBasicTransformation *>((*attributes)[0]);
            glm::mat4 t = transformation->GetTransformation();
            t = glm::translate(t, glm::vec3(p->GetAttribute("x"), 0.005f + p->GetAttribute("y"), p->GetAttribute("z")));
            transformation->SetTransformation(t);

            attributes = p->GetAttributesByType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_RENDERING_MESH);
            EG::Game::ObjectAttributeRenderingMesh *mesh_attr = static_cast<EG::Game::ObjectAttributeRenderingMesh *>((*attributes)[0]);
            glm::vec4 color = mesh_attr->GetMaterial()->GetColor();

            if (fc > 260.0f){
                float alpha_reduction_factor = (300.0f - fc) / 40.0f;
                mesh_attr->GetMaterial()->SetColor(glm::vec4(color.x, color.y, color.z, color.w * alpha_reduction_factor));
            }
        }

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
            shaders->Add("billboarding", "Shaders/Basic/billboarding.vert", "Shaders/Basic/billboarding.frag");
            shaders->Add("lighting", "Shaders/Basic/lighting.vert", "Shaders/Basic/lighting.frag");
            shaders->Add("textured", "Shaders/Basic/textured.vert", "Shaders/Basic/textured.frag");
            shaders->Add("font_rendering", "Shaders/Basic/font_rendering.vert", "Shaders/Basic/font_rendering.frag");
            shaders->Add("sphere_cube_map_gradient_decal", "Shaders/Basic/sphere_cube_mapped_with_gradient_decal.vert", "Shaders/Basic/sphere_cube_mapped_with_gradient_decal.frag");
            shaders->Add("sphere_cube_map_gradient_decal_with_lighting", "Shaders/Basic/sphere_cube_mapped_with_gradient_decal_with_lighting.vert", "Shaders/Basic/sphere_cube_mapped_with_gradient_decal_with_lighting.frag");

            camera = new EG::Graphics::Camera(45.0f, glm::ivec2(graphics->GetViewportWidth(), graphics->GetViewportHeight()), glm::vec2(0.1f, 100.0f));
            camera->ComputeProjectionMatrix();
            camera->SetPosition(glm::vec3(-0.7f, 2.5f, 5.0f));
            camera->RotateByMouse(glm::vec2(0.0f, -200.0f));
            camera->Update();
            camera->SetCameraType(EG::Graphics::Camera::CAMERA_FPS);

            orthographics_projection_matrix = glm::gtc::matrix_transform::ortho(0.0f, float(graphics->GetViewportWidth()), 0.0f, float(graphics->GetViewportHeight()));

            /* Test Data */
            TestEmitter *emitter = new TestEmitter();
            TestController *controller = new TestController();
            test_particles = new EG::Graphics::ParticleSystem(controller, emitter);

            initialized = true;
        }

        void Renderer::SetGUI(EG::Utility::RocketInterface *_gui){
            gui = _gui;
            gui_set = true;
        }

        void Renderer::RenderLitObject(EG::Game::Scene *scene, EG::Graphics::Light *light, EG::Game::Object *object){
            if (!(scene->GetMeshManager()->Get("quad"))){
                scene->GetMeshManager()->Add("quad", EG::Graphics::GenerateQuad());
                scene->GetTextureManager()->AddTexture("particle", new EG::Graphics::Texture("Assets/Textures/particle.png"));
            }
            // Meshes
            glm::vec3 lp = light->GetPosition();
            glm::vec4 light_position = glm::vec4(lp.x, lp.y, lp.z, 1.0f);
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
                        shaders->SetFloat3("camera_position", camera->GetPosition());
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
                        shaders->SetFloat3("camera_position", camera->GetPosition());
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
        }

        void Renderer::RenderObject(EG::Game::Scene *scene, EG::Game::Object *object){
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

            EG::Graphics::Light blank_light;
            blank_light.SetAttenuation(glm::vec3(1.0f, 1.0f, 1.0f));
            blank_light.SetCastsShadows(false);
            blank_light.SetColor(glm::vec3(0.0f, 0.0f, 0.0f));
            blank_light.SetRadius(0.0000001f);

            test_particles->Update(frame_time);
            std::list<EG::Graphics::Particle *> *particles = test_particles->GetParticles();
            std::list<EG::Graphics::Particle *>::iterator piter = particles->begin();
            unsigned int counter = 0;
            while (piter != particles->end()){
                EG::Graphics::Particle *p = (*piter);
                RenderLitObject(scene, &blank_light, p);
                counter += 1;
                ++piter;
            }

            // Render Objects
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

                        glm::vec3 light_color = light->GetColor();
                        // Matrices seem inappropriate now... hmm
                        glm::vec3 lp = light->GetPosition();
                        glm::vec4 light_position = glm::vec4(lp.x, lp.y, lp.z, 1.0f);
                        light_position.w = 1.0f;
                        //glm::vec4 light_view = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
                        shaders->SetFloat3("light_color", light_color);
                        shaders->SetFloat4("light_position", light_position);
                        shaders->SetFloat3("light_attenuation", light->GetAttenuation());
                        shaders->SetFloat("light_radius", light->GetRadius());

                        // Render Objects
                        test_particles->Update(frame_time);
                        std::list<EG::Graphics::Particle *> *particles = test_particles->GetParticles();
                        std::list<EG::Graphics::Particle *>::iterator piter = particles->begin();
                        unsigned int counter = 0;
                        while (piter != particles->end()){
                            EG::Graphics::Particle *p = (*piter);
                            RenderLitObject(scene, light, p);
                            counter += 1;
                            ++piter;
                        }

                        object_iterator = objects->GetKeysBegin();
                        while (object_iterator != objects->GetKeysEnd()){
                            EG::Game::Object *object = objects->Get(*object_iterator);
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
            shaders->Bind("textured");
            shaders->SetMatrix4("projection_matrix", camera->GetProjectionMatrix());
            shaders->SetMatrix4("view_matrix", camera->GetViewMatrix());
            shaders->SetInt("decal", 0);
            shaders->SetInt("height", 1);
            object_iterator = objects->GetKeysBegin();

            while (object_iterator != objects->GetKeysEnd()){
                EG::Game::Object *object = objects->Get(*object_iterator);
                RenderObject(scene, object);
                ++object_iterator;
            }

            glEnable(GL_BLEND);
            glDisable(GL_DEPTH_TEST);
            glEnable(GL_TEXTURE_2D);
            //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            particles = test_particles->GetParticles();
            piter = particles->begin();
            counter = 0;
            while (piter != particles->end()){
                EG::Graphics::Particle *p = (*piter);
                RenderObject(scene, p);
                counter += 1;
                ++piter;
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

            if (gui_set){
                gui->Draw();
            }
            glDisable(GL_BLEND);
            glEnable(GL_DEPTH_TEST);
            shaders->Unbind();
        }
    }
}
