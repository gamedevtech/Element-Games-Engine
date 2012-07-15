#include <iostream>
#include <string>

#include "Sandbox.h"
#include "../../../Engine/Utility/Window.h"
#include "../../../Engine/Graphics/GraphicsSubsystem.h"
#include "../../../Engine/Graphics/Renderer.h"
#include "../../../Engine/Math/Math.h"
#include "../../../Engine/Math/Noise.h"
#include "../../../Engine/Input/Input.h"
#include "../../../Engine/Game/Scene.h"
#include "../../../Engine/Game/ObjectManager.h"
#include "../../../Engine/Game/ObjectBasicAttribute.h"
#include "../../../Engine/Game/ObjectControlAttribute.h"
#include "../../../Engine/Game/ObjectEmissionAttribute.h"
#include "../../../Engine/Game/ObjectRenderingAttribute.h"
#include "../../../Engine/Graphics/Mesh.h"
#include "../../../Engine/Graphics/MeshGenerator.h"
#include "../../../Engine/Graphics/TextureManager.h"
#include "../../../Engine/Graphics/Texture.h"
#include "../../../Engine/Graphics/RenderingMaterial.h"
#include "../../../Engine/Utility/Time.h"
#include "../../../Engine/Utility/JSON.h"
#include "../../../Engine/Media/ModelLoader.h"
#include "../../../Engine/Media/ObjectReader.h"
#include "../../../Engine/Media/ObjectWriter.h"
#include "../../../Engine/Graphics/Particle.h"

EG::Media::ModelLoader *model;
int main(int argc, char **argv){
    enum CollisionTypes{
        COLLIDES_OBJECT = BIT(1),
        COLLIDES_PARTICLES = BIT(2)
    };
    int particles_collides = COLLIDES_OBJECT;
    int objects_collides = COLLIDES_OBJECT | COLLIDES_PARTICLES;
    // This is just a temporary organization to test classes as they get created!
    EG::Utility::JSON config_file("config.json");
    EG::Utility::Window *window = new EG::Utility::Window(config_file.GetInt("screen.width"), config_file.GetInt("screen.height"), 32, config_file.GetBool("screen.fullscreen"), "Element Games Sandbox");
    EG::Game::Scene *scene = new EG::Game::Scene();
    Sandbox *game = new Sandbox(window, scene);

    EG::Graphics::RenderingMaterial *material;
    EG::Graphics::Mesh *sphere = EG::Graphics::GenerateCubeSphere(16);
    scene->GetMeshManager()->Add("planet_sphere", sphere);

    scene->GetMeshManager()->Add("rectangle", EG::Graphics::GenerateQuad());

    // NOTE: Test Data
    float width = 512;
    float height = 512;
    EG::Math::Noise *noise_generator = new EG::Math::Noise(0, 16, 1.75f);
    float **heights = EG::Math::GenerateCubeSphereHeightMap(width, height, noise_generator, "Assets/Textures/generated_planet_height_map.png");
    EG::Math::GenerateCubeSphereNormalHeightMap(width, height, heights, "Assets/Textures/generated_planet_normal_map.png");
    EG::Math::ColorGradientSet *gradients = new EG::Math::ColorGradientSet();
    gradients->AddColorGradient(-1.0f, -0.4f, glm::vec4(0.0705f, 0.1333f, 0.2745f, 1.0f), glm::vec4(0.0705f, 0.1333, 0.2745f, 1.0f));
    gradients->AddColorGradient(-0.4f, -0.2f, glm::vec4(0.6588f, 0.5019f, 0.6313f, 1.0f), glm::vec4(0.3137f, 0.2745f, 0.2784f, 1.0f));
    gradients->AddColorGradient(-0.2f, 0.0f, glm::vec4(0.3137f, 0.2745f, 0.2784f, 1.0f), glm::vec4(0.1725f, 0.2235f, 0.1568f, 1.0f));
    gradients->AddColorGradient(0.0f, 0.15f, glm::vec4(0.1725f, 0.2235f, 0.1568f, 1.0f), glm::vec4(0.4f, 0.3176f, 0.3450f, 1.0f));
    gradients->AddColorGradient(0.15f, 0.35f, glm::vec4(0.4f, 0.3176f, 0.3450f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    gradients->AddColorGradient(0.35f, 1.0f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    glm::vec4 *colors = EG::Math::GenerateGradientMap(width, height, 4, gradients, "Assets/Textures/generated_planet_decal_map.png");
    std::vector<std::pair<glm::vec2, float> > specular_values;
    specular_values.push_back(std::pair<glm::vec2, float>(glm::vec2(-1.0f, -0.4f), 1.0f));
    specular_values.push_back(std::pair<glm::vec2, float>(glm::vec2(-0.4f, 0.35f), 0.0f));
    specular_values.push_back(std::pair<glm::vec2, float>(glm::vec2(0.35f, 1.0f), 0.5f));
    EG::Math::GenerateCubeSphereSpecularMap(width, height, heights, specular_values, "Assets/Textures/generated_planet_specular_map.png");
    for (unsigned int i = 0; i < 6; i++){
        delete []heights[i];
    }
    delete []heights;
    delete []colors;
    delete noise_generator;

    // Planet Sorta
    EG::Game::Object *object = new EG::Game::Object("Planet");
    glm::mat4 planet_transform = glm::translate(-5.0f, 5.0f, -5.0f);
    object->AddAttribute(new EG::Game::ObjectAttributeBasicTransformation(planet_transform));

    std::string images[6];
    for (unsigned int i = 0; i < 6; i++){
        std::stringstream temp;
        temp << "Assets/Textures/generated_planet_height_map_" << i << ".png";
        images[i] = temp.str();
    }
    EG::Graphics::CubeMap *cube_map = new EG::Graphics::CubeMap(images[CUBE_RIGHT], images[CUBE_LEFT], images[CUBE_BOTTOM], images[CUBE_TOP], images[CUBE_BACK], images[CUBE_FRONT]);
    scene->GetTextureManager()->AddCubeMap("planet_heights", cube_map);

    for (unsigned int i = 0; i < 6; i++){
        std::stringstream temp;
        temp << "Assets/Textures/generated_planet_normal_map_" << i << ".png";
        //temp << "Assets/Textures/default_normal.png";
        images[i] = temp.str();
    }
    cube_map = new EG::Graphics::CubeMap(images[CUBE_RIGHT], images[CUBE_LEFT], images[CUBE_BOTTOM], images[CUBE_TOP], images[CUBE_BACK], images[CUBE_FRONT]);
    scene->GetTextureManager()->AddCubeMap("planet_normals", cube_map);

    for (unsigned int i = 0; i < 6; i++){
        std::stringstream temp;
        temp << "Assets/Textures/generated_planet_specular_map_" << i << ".png";
        //temp << "Assets/Textures/default_normal.png";
        images[i] = temp.str();
    }
    cube_map = new EG::Graphics::CubeMap(images[CUBE_RIGHT], images[CUBE_LEFT], images[CUBE_BOTTOM], images[CUBE_TOP], images[CUBE_BACK], images[CUBE_FRONT]);
    scene->GetTextureManager()->AddCubeMap("planet_specular", cube_map);

    EG::Graphics::Texture *decal_gradient = new EG::Graphics::Texture("Assets/Textures/generated_planet_decal_map.png");
    scene->GetTextureManager()->AddTexture("planet_decal_gradient", decal_gradient);
    std::cout << "Dongle" << std::endl;
    material = new EG::Graphics::RenderingMaterial();
    material->SetCubeMap(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_HEIGHT, "planet_heights");
    material->SetCubeMap(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_NORMAL, "planet_normals");
    material->SetCubeMap(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_SPECULAR, "planet_specular");
    material->SetTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_DECAL, "planet_decal_gradient");
    material->SetLit(true);
    material->SetShaderOverride(EG::Graphics::RenderingMaterial::RENDERER_BASIC, EG::Graphics::RenderingMaterial::RENDERING_PHASE_TEXTURED_SHADER, "sphere_cube_map_gradient_decal");
    material->SetShaderOverride(EG::Graphics::RenderingMaterial::RENDERER_BASIC, EG::Graphics::RenderingMaterial::RENDERING_PHASE_LIGHTING_SHADER, "sphere_cube_map_gradient_decal_with_lighting");
    material->SetShaderOverride(EG::Graphics::RenderingMaterial::RENDERER_MULTIPASS, EG::Graphics::RenderingMaterial::RENDERING_PHASE_TEXTURED_SHADER, "sphere_cube_map_gradient_decal");
    material->SetShaderOverride(EG::Graphics::RenderingMaterial::RENDERER_MULTIPASS, EG::Graphics::RenderingMaterial::RENDERING_PHASE_LIGHTING_SHADER, "sphere_cube_map_gradient_decal_with_lighting");
    material->SetShaderOverride(EG::Graphics::RenderingMaterial::RENDERER_DEFERRED, EG::Graphics::RenderingMaterial::RENDERING_PHASE_PREPASS_SHADER, "sphere_cube_mapped_gradient_decal_prepass");
    object->AddAttribute(new EG::Game::ObjectAttributeRenderingMesh("planet_sphere", material));

    EG::Game::Object *pa = new EG::Game::Object("PlanetAtmosphere");
    planet_transform = planet_transform * glm::scale(1.15f, 1.15f, 1.15f);
    pa->AddAttribute(new EG::Game::ObjectAttributeBasicTransformation(planet_transform));
    material = new EG::Graphics::RenderingMaterial();
    material->SetLit(true);
    material->SetTranslucent(true);
    material->SetCastsShadows(false);
    material->SetColor(glm::vec4(0.25f, 0.25f, 0.5f, 0.5f));
    material->SetBlendingMode(EG::Graphics::RenderingMaterial::BLEND_ONE_ONE_MINUS_SRC_ALPHA);
    EG::Graphics::Texture *atmosphere_gradient = new EG::Graphics::Texture("Assets/Textures/atmosphere_gradient2.png");
    scene->GetTextureManager()->AddTexture("atmosphere_gradient", atmosphere_gradient);
    material->SetTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_DECAL, "atmosphere_gradient");
    material->SetShaderOverride(EG::Graphics::RenderingMaterial::RENDERER_DEFERRED, EG::Graphics::RenderingMaterial::RENDERING_PHASE_LIGHTING_SHADER, "planet_atmosphere");
    material->SetCastsShadows(false);
    material->SetBlendingMode(EG::Graphics::RenderingMaterial::BLEND_ALPHA);
    pa->AddAttribute(new EG::Game::ObjectAttributeRenderingMesh("planet_sphere", material));
    pa->AddAttribute(new EG::Game::ObjectAttributeBasicFloat("inner_radius", 1.0f));
    pa->AddAttribute(new EG::Game::ObjectAttributeBasicFloat("outer_radius", 1.15f));

    // Test Cube2
    EG::Graphics::Mesh *cube = EG::Graphics::GenerateCube();
    scene->GetMeshManager()->Add("cube", cube);
    EG::Game::Object *object2 = new EG::Game::Object("TestObject2");
    EG::Graphics::Texture *texture = new EG::Graphics::Texture("Assets/Textures/concrete.jpg"); // CRASHES HERE WIN32
    scene->GetTextureManager()->AddTexture("concrete_decal", texture);
    texture = new EG::Graphics::Texture("Assets/Textures/concrete_normal.jpg");
    scene->GetTextureManager()->AddTexture("concrete_normal", texture);
    glm::mat4 scale = glm::scale(10.0f, 0.1f, 10.0f);
    glm::mat4 translate = glm::translate(-5.0f, -0.101f, -5.0f);
    glm::mat4 plane_transform = translate * scale;
    object2->AddAttribute(new EG::Game::ObjectAttributeBasicTransformation(plane_transform));
    //EG::Graphics::RenderingMaterial *material = new EG::Graphics::RenderingMaterial();
    material = new EG::Graphics::RenderingMaterial();
    material->SetLit(true);
    material->SetCastsShadows(true);
    material->SetTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_DECAL, "concrete_decal");
    material->SetTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_NORMAL, "concrete_normal");
    object2->AddAttribute(new EG::Game::ObjectAttributeRenderingMesh("cube", material));
    EG::Dynamics::CollisionBox *plane_box = new EG::Dynamics::CollisionBox(0.0f, glm::vec3(5.0f, 0.05f, 5.0f));
    EG::Dynamics::RigidBody *plane_body = new EG::Dynamics::RigidBody(plane_box, translate, glm::vec3(10.0f, 0.1f, 10.0f));
    plane_body->SetCollisionFiltering(COLLIDES_OBJECT, objects_collides);
    object2->AddAttribute(new EG::Game::ObjectAttributeControlRigidBody(plane_body));

    // Sky Sphere
    EG::Graphics::Mesh *regular_sphere = EG::Graphics::GenerateSphere(4, 4);
    scene->GetMeshManager()->Add("sphere", regular_sphere);
    EG::Game::Object *object3 = new EG::Game::Object("SkySphere");
    texture = new EG::Graphics::Texture("Assets/Textures/starfield.jpg");
    scene->GetTextureManager()->AddTexture("starfield_decal", texture);
    scale = glm::scale(50.0f, 50.0f, 50.0f);
    object3->AddAttribute(new EG::Game::ObjectAttributeBasicTransformation(scale));
    material = new EG::Graphics::RenderingMaterial();
    material->SetLit(false);
    material->SetCastsShadows(false);
    material->SetTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_DECAL, "starfield_decal");
    object3->AddAttribute(new EG::Game::ObjectAttributeRenderingMesh("sphere", material));

    // Light 0
    EG::Game::Object *light_object = new EG::Game::Object("RedLight");
    glm::vec3 light_position = glm::vec3(0.0f, 6.0f, -6.0f);
    translate = glm::translate(light_position);
    scale = glm::scale(0.1f, 0.1f, 0.1f);
    light_object->AddAttribute(new EG::Game::ObjectAttributeBasicTransformation(translate * scale));
    EG::Graphics::Light *light = new EG::Graphics::Light();
    light->SetPosition(light_position);
    light->SetDirection(-light_position);
    light->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
    //light->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
    light->SetAttenuation(glm::vec3(0.8f, 0.00125f, 0.0000001f));
    light->SetRadius(100.0f);
    light->SetCastsShadows(true);
    light_object->AddAttribute(new EG::Game::ObjectAttributeEmissionLight(light));
    material = new EG::Graphics::RenderingMaterial();
    material->SetLit(false);
    material->SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 0.5f));
    light_object->AddAttribute(new EG::Game::ObjectAttributeRenderingMesh("sphere", material));

//     // Light 1
//     EG::Game::Object *light_object2 = new EG::Game::Object("GreenLight");
//     glm::vec3 light_position2 = glm::vec3(4.0f, 5.0f, 5.0f);
//     translate = glm::translate(light_position2);
//     light_object2->AddAttribute(new EG::Game::ObjectAttributeBasicTransformation(translate * scale));
//     EG::Graphics::Light *light2 = new EG::Graphics::Light();
//     light2->SetPosition(light_position2);
//     light2->SetDirection(-light_position2);
//     light2->SetColor(glm::vec3(0.0f, 1.0f, 0.0f));
//     //light2->SetColor(glm::vec3(0.0f, 0.8f, 0.2f));
//     light2->SetAttenuation(glm::vec3(0.8f, 0.00125f, 0.0000001f));
//     light2->SetRadius(100.0f);
//     light2->SetCastsShadows(true);
//     light_object2->AddAttribute(new EG::Game::ObjectAttributeEmissionLight(light2));
//     material = new EG::Graphics::RenderingMaterial();
//     material->SetLit(false);
//     material->SetColor(glm::vec4(0.0f, 1.0f, 0.0f, 0.5f));
//     light_object2->AddAttribute(new EG::Game::ObjectAttributeRenderingMesh("sphere", material));
//
//     // Light 2
//     EG::Game::Object *light_object3 = new EG::Game::Object("BlueLight");
//     glm::vec3 light_position3 = glm::vec3(0.0f, 5.0f, -5.0f);
//     translate = glm::translate(light_position3);
//     light_object3->AddAttribute(new EG::Game::ObjectAttributeBasicTransformation(translate * scale));
//     EG::Graphics::Light *light3 = new EG::Graphics::Light();
//     light3->SetPosition(light_position3);
//     light3->SetDirection(-light_position3);
//     light3->SetColor(glm::vec3(0.0f, 0.0f, 1.0f));
//     //light3->SetColor(glm::vec3(1.0f, 0.0f, 1.0f));
//     light3->SetAttenuation(glm::vec3(0.8f, 0.00125f, 0.0000001f));
//     light3->SetRadius(100.0f);
//     light3->SetCastsShadows(true);
//     light_object3->AddAttribute(new EG::Game::ObjectAttributeEmissionLight(light3));
//     material = new EG::Graphics::RenderingMaterial();
//     material->SetLit(false);
//     material->SetColor(glm::vec4(0.0f, 0.0f, 1.0f, 0.5f));
//     light_object3->AddAttribute(new EG::Game::ObjectAttributeRenderingMesh("sphere", material));
//     std::cout << "After Light 2" << std::endl;

    // Particle System
    scene->GetMeshManager()->Add("quad", EG::Graphics::GenerateQuad());
    scene->GetTextureManager()->AddTexture("particle", new EG::Graphics::Texture("Assets/Textures/nebula_particle.png"));
    //scene->GetTextureManager()->AddTexture("particle", new EG::Graphics::Texture("Assets/Textures/smoke_particle.png"));
    EG::Game::Object *particle_system = new EG::Game::Object("ParticleSystem");

    class TestEmitterDef : public EG::Graphics::ParticleEmitter{
    public:
        TestEmitterDef(void) : EG::Graphics::ParticleEmitter(10.0f){
            emitted = false;
        }
        ~TestEmitterDef(void){ }
        void CreateParticle(EG::Graphics::Particle *p){
            enum CollisionTypes{
                COLLIDES_OBJECT = BIT(1),
                COLLIDES_PARTICLES = BIT(2)
            };
            int particles_collides = COLLIDES_OBJECT;
            int objects_collides = COLLIDES_OBJECT | COLLIDES_PARTICLES;

            p->SetAttribute("frame_count", 0.0f);
            p->SetAttribute("alpha", 0.75f);
            p->SetAttribute("x", EG::Math::Utility::RandomFloat(-0.25f, 0.25f));
            p->SetAttribute("y", EG::Math::Utility::RandomFloat(0.0f, 0.25f));
            p->SetAttribute("z", EG::Math::Utility::RandomFloat(-0.25f, 0.25f));
            EG::Graphics::RenderingMaterial *material = new EG::Graphics::RenderingMaterial();
            material->SetCastsShadows(false);
            material->SetDiffuse(1.0f);
            material->SetAmbient(1.0f);
            material->SetSpecular(1.0f);
            material->SetColor(glm::vec4(0.1f, 0.3f, 0.7f, 0.75f));
            material->SetLit(false);
            material->SetTranslucent(true);
            material->SetBlendingMode(EG::Graphics::RenderingMaterial::BLEND_ALPHA_PARTICLE);
            material->SetTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_DECAL, "particle");
            material->SetShaderOverride(EG::Graphics::RenderingMaterial::RENDERER_DEFERRED, EG::Graphics::RenderingMaterial::RENDERING_PHASE_PREPASS_SHADER, "billboarding");
            material->SetShaderOverride(EG::Graphics::RenderingMaterial::RENDERER_BASIC, EG::Graphics::RenderingMaterial::RENDERING_PHASE_TEXTURED_SHADER, "billboarding");

            glm::vec3 position = glm::vec3(EG::Math::Utility::RandomFloat(-1.0f, 1.0f), EG::Math::Utility::RandomFloat(-1.0f, 1.0f) + 3.0f, EG::Math::Utility::RandomFloat(-1.0f, 1.0f));
            //glm::vec3 position(0.0f, 0.5f, 0.0f);
            glm::vec3 psize(1.2f, 1.2f, 1.2f);
            glm::mat4 pscale = glm::scale(psize);
            glm::mat4 ptranslate = glm::translate(position);
            EG::Game::ObjectAttributeBasicTransformation *transformation = new EG::Game::ObjectAttributeBasicTransformation(ptranslate * pscale);

            p->AddAttribute(transformation);
            p->AddAttribute(new EG::Game::ObjectAttributeRenderingMesh("quad", material));

            /*if (EG::Math::Utility::RandomUnsigned(500) >= 350){
                EG::Graphics::Light *light = new EG::Graphics::Light();
                light->SetPosition(position);
                light->SetDirection(-glm::vec3(0.2f, 0.2f, 0.2f));
                light->SetColor(glm::vec3(0.3f, 0.1f, 0.0f));
                light->SetAttenuation(glm::vec3(0.8f, 0.00125f, 0.0000001f));
                light->SetRadius(1.75f);
                light->SetCastsShadows(false);
                p->AddAttribute(new EG::Game::ObjectAttributeEmissionLight(light));
            }*/

            EG::Dynamics::CollisionSphere *psphere = new EG::Dynamics::CollisionSphere(0.0f, psize.x);
            EG::Dynamics::RigidBody *pbody = new EG::Dynamics::RigidBody(psphere, ptranslate, psize);
            pbody->SetCollisionFiltering(COLLIDES_PARTICLES, particles_collides);
            //pbody->ApplyImpulse(glm::vec3(EG::Math::Utility::RandomFloat(-0.5f, 0.5f), 2.0f, EG::Math::Utility::RandomFloat(-0.5f, 0.5f)));
            p->AddAttribute(new EG::Game::ObjectAttributeControlRigidBody(pbody));
        }
        void Emit(std::list<EG::Graphics::Particle *> *particles, float frame_time){
            if (!emitted){
                for (unsigned int i = 0; i < 50; i++){
                    EG::Graphics::Particle *new_particle = new EG::Graphics::Particle();
                    CreateParticle(new_particle);
                    particles->push_back(new_particle);
                }
                emitted = true;
            }
        }
    private:
        bool emitted;
    };

    class TestControllerDef : public EG::Graphics::ParticleController{
    public:
        TestControllerDef(void){ }
        ~TestControllerDef(void){ }
        void ControlParticle(EG::Graphics::Particle *p, float frame_time){
            /*float fc = p->GetAttribute("frame_count");
            fc += frame_time;
            if (fc > 3.0f){
                p->SetAlive(false);
            }
            p->SetAttribute("frame_count", fc);

            std::vector<EG::Game::ObjectAttribute *> *attributes = p->GetAttributesByType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_BASIC_TRANSFORMATION);
            EG::Game::ObjectAttributeBasicTransformation *transformation = static_cast<EG::Game::ObjectAttributeBasicTransformation *>((*attributes)[0]);
            glm::mat4 t = transformation->GetTransformation();
            //t = glm::translate(t, glm::vec3(frame_time * p->GetAttribute("x"), frame_time * (1.0f + p->GetAttribute("y")), frame_time * p->GetAttribute("z")));
            //transformation->SetTransformation(t);

            attributes = p->GetAttributesByType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_RENDERING_MESH);
            EG::Game::ObjectAttributeRenderingMesh *mesh_attr = static_cast<EG::Game::ObjectAttributeRenderingMesh *>((*attributes)[0]);
            glm::vec4 color = mesh_attr->GetMaterial()->GetColor();

            if (p->HasAttributesOfType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_EMISSION_LIGHT)){
                attributes = p->GetAttributesByType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_EMISSION_LIGHT);
                std::vector<EG::Game::ObjectAttribute *>::iterator attr_iter = attributes->begin();
                while (attr_iter != attributes->end()){
                    EG::Game::ObjectAttributeEmissionLight *light_attr = static_cast<EG::Game::ObjectAttributeEmissionLight *>(*attr_iter);
                    EG::Graphics::Light *l = light_attr->GetLight();
                    float *m_data = glm::value_ptr(t);
                    l->SetPosition(glm::vec3(m_data[12], m_data[13] + 1.25f, m_data[14]));
                    ++attr_iter;
                }
            }

            if (fc > 2.0f){
                float alpha_reduction_factor = (3.0f - fc) / 1.0f;
                mesh_attr->GetMaterial()->SetColor(glm::vec4(color.x, color.y, color.z, color.w * alpha_reduction_factor));
            }*/
        }
    private:
    };
    particle_system->AddAttribute(new EG::Game::ObjectAttributeEmissionParticleSystem(new EG::Graphics::ParticleSystem(new TestControllerDef(), new TestEmitterDef())));

    // NOTE: Object Reader Test
    EG::Media::ObjectReader reader;
    reader.Read("Assets/Models/test_model.ego", scene);
    EG::Game::Object *read_object = reader.GetLoadedObject();
    EG::Dynamics::CollisionBox *collision_shape = new EG::Dynamics::CollisionBox(10.0f, glm::vec3(1.0f, 0.5f, 1.0f));
    glm::mat4 ship_trans = glm::translate(0.0f, 2.0f, 0.0f);
    EG::Dynamics::RigidBody *rigid_body = new EG::Dynamics::RigidBody(collision_shape, ship_trans, glm::vec3(0.02f, 0.02f, 0.02f));
    rigid_body->SetCollisionFiltering(COLLIDES_OBJECT, COLLIDES_OBJECT);
    read_object->AddAttribute(new EG::Game::ObjectAttributeControlRigidBody(rigid_body));
    std::cout << "Loaded Model: " << read_object->GetObjectName() << std::endl;
    // END TEST

    EG::Graphics::Camera *main_camera = new EG::Graphics::Camera(45.0f, glm::ivec2(graphics->GetViewportWidth(), graphics->GetViewportHeight()), glm::vec2(0.001f, 100.0f));
    main_camera->ComputeProjectionMatrix();
    main_camera->SetPosition(glm::vec3(-0.7f, 2.5f, 5.0f));
    main_camera->RotateByMouse(glm::vec2(0.0f, -200.0f));
    main_camera->Update();
    main_camera->SetCameraType(EG::Graphics::Camera::CAMERA_FPS);
    //EG::Game::Object *camera_object = new EG::Game::Object("camera");
    //camera_object->AddAttribute(new EG::Game::ObjectAttributeRenderingCamera(main_camera));
    read_object->AddAttribute(new EG::Game::ObjectAttributeRenderingCamera(main_camera));

    // Add Objects
    EG::Game::ObjectManager *objects = game->GetScene()->GetObjectManager();
    objects->AddObject(object);
    objects->AddObject(pa);
    objects->AddObject(read_object);
    objects->AddObject(object2);
    objects->AddObject(object3);
    //objects->AddObject(object4);
    objects->AddObject(light_object);
//     objects->AddObject(light_object2);
//     objects->AddObject(light_object3);
    objects->AddObject(particle_system);
    //objects->AddObject(camera_object);
    game->GetScene()->SetCurrentCamera(main_camera);
    //objects->AddObject(dummy_light_object);
    // NOTE: End Test Data

    while (game->GetWindow()->IsOpened()){
        //std::cout << "Begin Loop Frame" << std::endl;
        game->Update();
        //std::cout << "Game Updated" << std::endl;
        game->Render();
        //std::cout << "Game Rendered" << std::endl;
        glFinish();
    }
    window->Close();
    std::cout << "Exiting..." << std::endl;

    return 0;
}
