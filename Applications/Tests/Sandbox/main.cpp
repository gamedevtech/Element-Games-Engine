#include <iostream>
#include <string>

#include "../../../Engine/Game/Game.h"
#include "../../../Engine/Utility/Window.h"
#include "../../../Engine/Graphics/GraphicsSubsystem.h"
#include "../../../Engine/Graphics/Renderer.h"
#include "../../../Engine/Math/Math.h"
#include "../../../Engine/Math/Noise.h"
#include "../../../Engine/Input/Input.h"
#include "../../../Engine/Game/Scene.h"
#include "../../../Engine/Game/ObjectManager.h"
#include "../../../Engine/Game/ObjectBasicAttribute.h"
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
#include "../../../Engine/Graphics/Particle.h"

EG::Media::ModelLoader *model;
int main(int argc, char **argv){
    // This is just a temporary organization to test classes as they get created!
    EG::Utility::JSON config_file("config.json");
    EG::Utility::Window *window = new EG::Utility::Window(config_file.GetInt("screen.width"), config_file.GetInt("screen.height"), 32, config_file.GetBool("screen.fullscreen"), "Element Games Sandbox");
    EG::Game::Scene *scene = new EG::Game::Scene();
    EG::Game::Game *game = new EG::Game::Game(window, scene);

    EG::Graphics::RenderingMaterial *material;
    EG::Graphics::Mesh *sphere = EG::Graphics::GenerateCubeSphere(4);
    scene->GetMeshManager()->Add("planet_sphere", sphere);

    // NOTE: Test Data
	float width = 256;
    float height = 256;
    EG::Math::Noise *noise_generator = new EG::Math::Noise(0, 16, 1.75f);
    float **heights = EG::Math::GenerateCubeSphereHeightMap(width, height, noise_generator, "Assets/Textures/generated_planet_height_map.png");
    glm::vec4 **normals = EG::Math::GenerateCubeSphereNormalHeightMap(width, height, heights, "Assets/Textures/generated_planet_normal_map.png");
    EG::Math::ColorGradientSet *gradients = new EG::Math::ColorGradientSet();
    gradients->AddColorGradient(-1.0f, -0.3f, glm::vec4(0.0f, 0.11f, 0.45f, 1.0f), glm::vec4(0.0f, 0.25f, 1.0f, 1.0f));
    gradients->AddColorGradient(-0.3f, -0.2f, glm::vec4(1.0f, 1.0f, 0.35f, 1.0f), glm::vec4(0.64f, 0.85f, 0.0f, 1.0f));
    gradients->AddColorGradient(-0.2f, 0.0f, glm::vec4(0.64f, 0.85f, 0.0f, 1.0f), glm::vec4(0.14f, 0.55f, 0.1f, 1.0f));
    gradients->AddColorGradient(0.0f, 0.15f, glm::vec4(0.14f, 0.55f, 0.1f, 1.0f), glm::vec4(0.45f, 0.37f, 0.0f, 1.0f));
    gradients->AddColorGradient(0.15f, 0.35f, glm::vec4(0.45f, 0.37f, 0.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    gradients->AddColorGradient(0.35f, 1.0f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    glm::vec4 *colors = EG::Math::GenerateGradientMap(width, height, 4, gradients, "Assets/Textures/generated_planet_decal_map.png");
    for (unsigned int i = 0; i < 6; i++){
        delete []normals[i];
        delete []heights[i];
        //delete []colors[i];
    }
    delete []normals;
    delete []heights;
    delete []colors;
    delete noise_generator;

    // Planet Sorta
    EG::Game::Object *object = new EG::Game::Object("Planet");
    object->AddAttribute(new EG::Game::ObjectAttributeBasicTransformation(glm::translate(glm::mat4(1.0f), glm::vec3(-5.0f, 5.0f, -5.0f))));

    std::string images[6];
    for (unsigned int i = 0; i < 6; i++){
        std::stringstream temp;
        temp << "Assets/Textures/generated_planet_height_map_" << i << ".png";
        images[i] = temp.str();
    }
    EG::Graphics::CubeMap *cube_map = new EG::Graphics::CubeMap(images[CUBE_RIGHT], images[CUBE_LEFT], images[CUBE_TOP], images[CUBE_BOTTOM], images[CUBE_BACK], images[CUBE_FRONT]);
    scene->GetTextureManager()->AddCubeMap("planet_heights", cube_map);

    for (unsigned int i = 0; i < 6; i++){
        std::stringstream temp;
        temp << "Assets/Textures/generated_planet_normal_map_" << i << ".png";
        //temp << "Assets/Textures/default_normal.png";
        images[i] = temp.str();
    }
    cube_map = new EG::Graphics::CubeMap(images[CUBE_RIGHT], images[CUBE_LEFT], images[CUBE_TOP], images[CUBE_BOTTOM], images[CUBE_BACK], images[CUBE_FRONT]);
    scene->GetTextureManager()->AddCubeMap("planet_normals", cube_map);

    EG::Graphics::Texture *decal_gradient = new EG::Graphics::Texture("Assets/Textures/generated_planet_decal_map.png");
    scene->GetTextureManager()->AddTexture("planet_decal_gradient", decal_gradient);
	material = new EG::Graphics::RenderingMaterial();
    material->SetCubeMap(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_HEIGHT, "planet_heights");
    material->SetCubeMap(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_NORMAL, "planet_normals");
    material->SetTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_DECAL, "planet_decal_gradient");
    material->SetLit(true);
    material->SetShaderOverride(EG::Graphics::RenderingMaterial::RENDERER_BASIC, EG::Graphics::RenderingMaterial::RENDERING_PHASE_TEXTURED_SHADER, "sphere_cube_map_gradient_decal");
    material->SetShaderOverride(EG::Graphics::RenderingMaterial::RENDERER_BASIC, EG::Graphics::RenderingMaterial::RENDERING_PHASE_LIGHTING_SHADER, "sphere_cube_map_gradient_decal_with_lighting");
    material->SetShaderOverride(EG::Graphics::RenderingMaterial::RENDERER_MULTIPASS, EG::Graphics::RenderingMaterial::RENDERING_PHASE_TEXTURED_SHADER, "sphere_cube_map_gradient_decal");
    material->SetShaderOverride(EG::Graphics::RenderingMaterial::RENDERER_MULTIPASS, EG::Graphics::RenderingMaterial::RENDERING_PHASE_LIGHTING_SHADER, "sphere_cube_map_gradient_decal_with_lighting");
    material->SetShaderOverride(EG::Graphics::RenderingMaterial::RENDERER_DEFERRED, EG::Graphics::RenderingMaterial::RENDERING_PHASE_PREPASS_SHADER, "sphere_cube_mapped_gradient_decal_prepass");
	object->AddAttribute(new EG::Game::ObjectAttributeRenderingMesh("planet_sphere", material));

    // Test Cube2
    EG::Graphics::Mesh *cube = EG::Graphics::GenerateCube();
    scene->GetMeshManager()->Add("cube", cube);
    EG::Game::Object *object2 = new EG::Game::Object("TestObject2");
    EG::Graphics::Texture *texture = new EG::Graphics::Texture("Assets/Textures/concrete.jpg");
    scene->GetTextureManager()->AddTexture("concrete_decal", texture);
    texture = new EG::Graphics::Texture("Assets/Textures/concrete_normal.jpg");
    scene->GetTextureManager()->AddTexture("concrete_normal", texture);
    object2->AddAttribute(new EG::Game::ObjectAttributeBasicTransformation(glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(-5.0f, -0.101f, -5.0f)), glm::vec3(10.0f, 0.1f, 10.0f))));
    //EG::Graphics::RenderingMaterial *material = new EG::Graphics::RenderingMaterial();
    material = new EG::Graphics::RenderingMaterial();
    material->SetLit(true);
    material->SetCastsShadows(true);
    material->SetTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_DECAL, "concrete_decal");
    material->SetTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_NORMAL, "concrete_normal");
    object2->AddAttribute(new EG::Game::ObjectAttributeRenderingMesh("cube", material));

    // Sky Sphere
    EG::Graphics::Mesh *regular_sphere = EG::Graphics::GenerateSphere(4, 4);
    scene->GetMeshManager()->Add("sphere", regular_sphere);
    EG::Game::Object *object3 = new EG::Game::Object("SkySphere");
    texture = new EG::Graphics::Texture("Assets/Textures/starfield.jpg");
    scene->GetTextureManager()->AddTexture("starfield_decal", texture);
    object3->AddAttribute(new EG::Game::ObjectAttributeBasicTransformation(glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)), glm::vec3(50.0f, 50.0f, 50.0f))));
    material = new EG::Graphics::RenderingMaterial();
    material->SetLit(false);
    material->SetCastsShadows(false);
    material->SetTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_DECAL, "starfield_decal");
    object3->AddAttribute(new EG::Game::ObjectAttributeRenderingMesh("sphere", material));

    // Light 0
    EG::Game::Object *light_object = new EG::Game::Object("RedLight");
    glm::mat4 light_geometry_transform = glm::mat4(1.0f);
    glm::vec3 light_position = glm::vec3(-4.0f, 5.0f, 5.0f);
    light_geometry_transform = glm::translate(light_geometry_transform, light_position);
    light_geometry_transform = glm::scale(light_geometry_transform, glm::vec3(0.1f, 0.1f, 0.1f));
    light_object->AddAttribute(new EG::Game::ObjectAttributeBasicTransformation(light_geometry_transform));
    EG::Graphics::Light *light = new EG::Graphics::Light();
    light->SetPosition(light_position);
    light->SetDirection(-light_position);
    light->SetColor(glm::vec3(1.0f, 0.0f, 0.0f));
    //light->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
    light->SetAttenuation(glm::vec3(0.8f, 0.00125f, 0.0000001f));
    light->SetRadius(100.0f);
    light->SetCastsShadows(true);
    light_object->AddAttribute(new EG::Game::ObjectAttributeEmissionLight(light));
    material = new EG::Graphics::RenderingMaterial();
    material->SetLit(false);
    material->SetColor(glm::vec4(1.0f, 0.0f, 0.0f, 0.5f));
    light_object->AddAttribute(new EG::Game::ObjectAttributeRenderingMesh("sphere", material));

    // Light 1
    EG::Game::Object *light_object2 = new EG::Game::Object("GreenLight");
    glm::vec3 light_position2 = glm::vec3(4.0f, 5.0f, 5.0f);
    glm::mat4 light_geometry_transform2 = glm::mat4(1.0f);
    light_geometry_transform2 = glm::translate(light_geometry_transform2, light_position2);
    light_geometry_transform2 = glm::scale(light_geometry_transform2, glm::vec3(0.1f, 0.1f, 0.1f));
    light_object2->AddAttribute(new EG::Game::ObjectAttributeBasicTransformation(light_geometry_transform2));
    EG::Graphics::Light *light2 = new EG::Graphics::Light();
    light2->SetPosition(light_position2);
    light2->SetDirection(-light_position2);
    light2->SetColor(glm::vec3(0.0f, 1.0f, 0.0f));
    //light2->SetColor(glm::vec3(0.0f, 0.8f, 0.2f));
    light2->SetAttenuation(glm::vec3(0.8f, 0.00125f, 0.0000001f));
    light2->SetRadius(100.0f);
    light2->SetCastsShadows(true);
    light_object2->AddAttribute(new EG::Game::ObjectAttributeEmissionLight(light2));
    material = new EG::Graphics::RenderingMaterial();
    material->SetLit(false);
    material->SetColor(glm::vec4(0.0f, 1.0f, 0.0f, 0.5f));
    light_object2->AddAttribute(new EG::Game::ObjectAttributeRenderingMesh("sphere", material));

    // Light 2
    EG::Game::Object *light_object3 = new EG::Game::Object("BlueLight");
    glm::vec3 light_position3 = glm::vec3(0.0f, 5.0f, -5.0f);
    glm::mat4 light_geometry_transform3 = glm::mat4(1.0f);
    light_geometry_transform3 = glm::translate(light_geometry_transform3, light_position3);
    light_geometry_transform3 = glm::scale(light_geometry_transform3, glm::vec3(0.1f, 0.1f, 0.1f));
    light_object3->AddAttribute(new EG::Game::ObjectAttributeBasicTransformation(light_geometry_transform3));
    EG::Graphics::Light *light3 = new EG::Graphics::Light();
    light3->SetPosition(light_position3);
    light3->SetDirection(-light_position3);
    light3->SetColor(glm::vec3(0.0f, 0.0f, 1.0f));
    //light3->SetColor(glm::vec3(1.0f, 0.0f, 1.0f));
    light3->SetAttenuation(glm::vec3(0.8f, 0.00125f, 0.0000001f));
    light3->SetRadius(100.0f);
    light3->SetCastsShadows(true);
    light_object3->AddAttribute(new EG::Game::ObjectAttributeEmissionLight(light3));
    material = new EG::Graphics::RenderingMaterial();
    material->SetLit(false);
    material->SetColor(glm::vec4(0.0f, 0.0f, 1.0f, 0.5f));
    light_object3->AddAttribute(new EG::Game::ObjectAttributeRenderingMesh("sphere", material));

    // NOTE: Object Reader Test
    EG::Media::ObjectReader reader;
    reader.Read("Assets/Models/test_model.ego", scene);
    EG::Game::Object *read_object = reader.GetLoadedObject();
    // END TEST

    // Add Objects
    EG::Game::ObjectManager *objects = game->GetScene()->GetObjectManager();
	objects->AddObject(object);
    objects->AddObject(read_object);
    objects->AddObject(object2);
    objects->AddObject(object3);
    //objects->AddObject(object4);
    objects->AddObject(light_object);
    objects->AddObject(light_object2);
    objects->AddObject(light_object3);
    //objects->AddObject(dummy_light_object);
    // NOTE: End Test Data

    while (game->GetWindow()->IsOpened()){
        game->Update();
        game->Render();
    }
    window->Close();
    std::cout << "Exiting..." << std::endl;

    return 0;
}
