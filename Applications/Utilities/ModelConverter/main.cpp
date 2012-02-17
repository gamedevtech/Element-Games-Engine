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
#include "../../../Engine/Utility/GUI.h"
#include "ModelConverter.h"

EG::Media::ModelLoader *model;
int main(int argc, char **argv){
    // This is just a temporary organization to test classes as they get created!
    EG::Utility::JSON config_file("config.json");
    EG::Utility::Window *window = new EG::Utility::Window(config_file.GetInt("screen.width"), config_file.GetInt("screen.height"), 32, config_file.GetBool("screen.fullscreen"), "Element Games Sandbox");
    EG::Game::Scene *scene = new EG::Game::Scene();
    ModelConverter *game = new ModelConverter(window, scene);

    // Ground Plane
    EG::Graphics::Mesh *cube_mesh = EG::Graphics::GenerateCube();
    scene->GetMeshManager()->Add("cube", cube_mesh);
    EG::Game::Object *ground_plane = new EG::Game::Object("GroundPlane");
    EG::Graphics::Texture *texture = new EG::Graphics::Texture("Assets/Textures/concrete.jpg");
    scene->GetTextureManager()->AddTexture("concrete_decal", texture);
    texture = new EG::Graphics::Texture("Assets/Textures/concrete_normal.jpg");
    scene->GetTextureManager()->AddTexture("concrete_normal", texture);
    ground_plane->AddAttribute(new EG::Game::ObjectAttributeBasicTransformation(glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(-5.0f, -0.101f, -5.0f)), glm::vec3(10.0f, 0.1f, 10.0f))));
    EG::Graphics::RenderingMaterial *material = new EG::Graphics::RenderingMaterial();
    material->SetLit(true);
    material->SetCastsShadows(true);
    material->SetTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_DECAL, "concrete_decal");
    material->SetTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_NORMAL, "concrete_normal");
    ground_plane->AddAttribute(new EG::Game::ObjectAttributeRenderingMesh("cube", material));

    // Sky Sphere
    EG::Graphics::Mesh *sphere = EG::Graphics::GenerateSphere(4, 4);
    scene->GetMeshManager()->Add("sphere", sphere);
    EG::Game::Object *sky_sphere = new EG::Game::Object("SkySphere");
    texture = new EG::Graphics::Texture("Assets/Textures/sky.png");
    scene->GetTextureManager()->AddTexture("starfield_decal", texture);
    sky_sphere->AddAttribute(new EG::Game::ObjectAttributeBasicTransformation(glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)), glm::vec3(50.0f, 50.0f, 50.0f))));
    material = new EG::Graphics::RenderingMaterial();
    material->SetLit(false);
    material->SetCastsShadows(false);
    material->SetTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_DECAL, "starfield_decal");
    sky_sphere->AddAttribute(new EG::Game::ObjectAttributeRenderingMesh("sphere", material));

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
    light2->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
    light2->SetAttenuation(glm::vec3(0.8f, 0.00125f, 0.0000001f));
    light2->SetRadius(100.0f);
    light2->SetCastsShadows(true);
    light_object2->AddAttribute(new EG::Game::ObjectAttributeEmissionLight(light2));
    material = new EG::Graphics::RenderingMaterial();
    material->SetLit(false);
    material->SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 0.5f));
    light_object2->AddAttribute(new EG::Game::ObjectAttributeRenderingMesh("sphere", material));

    EG::Game::Object *camera_object = new EG::Game::Object("camera");
    EG::Graphics::Camera *main_camera = new EG::Graphics::Camera(45.0f, glm::ivec2(graphics->GetViewportWidth(), graphics->GetViewportHeight()), glm::vec2(0.1f, 100.0f));
    main_camera->ComputeProjectionMatrix();
    main_camera->SetPosition(glm::vec3(-0.7f, 2.5f, 5.0f));
    main_camera->RotateByMouse(glm::vec2(0.0f, -200.0f));
    main_camera->Update();
    main_camera->SetCameraType(EG::Graphics::Camera::CAMERA_FPS);
    camera_object->AddAttribute(new EG::Game::ObjectAttributeRenderingCamera(main_camera));
    scene->SetCurrentCamera(main_camera);

    // Add Objects
    EG::Game::ObjectManager *objects = game->GetScene()->GetObjectManager();
    objects->AddObject(ground_plane);
    objects->AddObject(light_object2);
    objects->AddObject(camera_object);

	// TEST DATA //
	EG::GUI::GUI *gui = new EG::GUI::GUI("http://www.google.com");
	///////////////

    while (game->GetWindow()->IsOpened()){
        game->Update();
		gui->Update();
        game->Render();
		gui->Render();
    }
    window->Close();
    std::cout << "Exiting..." << std::endl;

    return 0;
}
