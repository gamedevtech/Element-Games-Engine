#include <iostream>
#include <string>

#include "Editor.h"
#include "../../Engine/Utility/Window.h"
#include "../../Engine/Graphics/GraphicsSubsystem.h"
#include "../../Engine/Graphics/Renderer.h"
#include "../../Engine/Math/Math.h"
#include "../../Engine/Math/Noise.h"
#include "../../Engine/Input/Input.h"
#include "../../Engine/Game/Scene.h"
#include "../../Engine/Game/ObjectManager.h"
#include "../../Engine/Game/ObjectBasicAttribute.h"
#include "../../Engine/Game/ObjectControlAttribute.h"
#include "../../Engine/Game/ObjectEmissionAttribute.h"
#include "../../Engine/Game/ObjectRenderingAttribute.h"
#include "../../Engine/Graphics/Mesh.h"
#include "../../Engine/Graphics/MeshGenerator.h"
#include "../../Engine/Graphics/TextureManager.h"
#include "../../Engine/Graphics/Texture.h"
#include "../../Engine/Graphics/RenderingMaterial.h"
#include "../../Engine/Utility/Time.h"
#include "../../Engine/Utility/JSON.h"
#include "../../Engine/Media/ModelLoader.h"
#include "../../Engine/Media/ObjectReader.h"
#include "../../Engine/Media/ObjectWriter.h"
#include "../../Engine/Graphics/Particle.h"

EG::Media::ModelLoader *model;
int main(int argc, char **argv){
    EG::Utility::JSON config_file("config.json");
    EG::Utility::Window *window = new EG::Utility::Window(config_file.GetInt("screen.width"), config_file.GetInt("screen.height"), 32, config_file.GetBool("screen.fullscreen"), "Element Games Sandbox");
    EG::Game::Scene *scene = new EG::Game::Scene();
    Editor *game = new Editor(window, scene);

    EG::Graphics::RenderingMaterial *material;
    EG::Graphics::Mesh *sphere = EG::Graphics::GenerateCubeSphere(4);
    scene->GetMeshManager()->Add("sphere", sphere);
    scene->GetMeshManager()->Add("rectangle", EG::Graphics::GenerateQuad());

    // Test Cube2
    EG::Graphics::Mesh *cube = EG::Graphics::GenerateCube();
    scene->GetMeshManager()->Add("cube", cube);
    EG::Game::Object *object2 = new EG::Game::Object("Ground");
    EG::Graphics::Texture *texture = new EG::Graphics::Texture("Assets/Textures/concrete.jpg");
    scene->GetTextureManager()->AddTexture("concrete_decal", texture);
    texture = new EG::Graphics::Texture("Assets/Textures/concrete_normal.jpg");
    scene->GetTextureManager()->AddTexture("concrete_normal", texture);
    glm::mat4 scale = glm::scale(10.0f, 0.1f, 10.0f);
    glm::mat4 translate = glm::translate(-5.0f, -0.101f, -5.0f);
    glm::mat4 plane_transform = translate * scale;
    object2->AddAttribute(new EG::Game::ObjectAttributeBasicTransformation(plane_transform));
    material = new EG::Graphics::RenderingMaterial();
    material->SetLit(true);
    material->SetCastsShadows(true);
    material->SetTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_DECAL, "concrete_decal");
    material->SetTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_NORMAL, "concrete_normal");
    object2->AddAttribute(new EG::Game::ObjectAttributeRenderingMesh("cube", material));
    EG::Dynamics::CollisionBox *plane_box = new EG::Dynamics::CollisionBox(0.0f, glm::vec3(5.0f, 0.05f, 5.0f));
    EG::Dynamics::RigidBody *plane_body = new EG::Dynamics::RigidBody(plane_box, translate, glm::vec3(10.0f, 0.1f, 10.0f));
    object2->AddAttribute(new EG::Game::ObjectAttributeControlRigidBody(plane_body));
    //object2->AddScript(new EG::Game::ObjectScript("Assets/Scripts/hello_world.lua"));

    // Selection Box
    EG::Game::Object *selection_box = new EG::Game::Object("SelectionBox");
    texture = new EG::Graphics::Texture("Assets/Textures/outline.png");
    scene->GetTextureManager()->AddTexture("outline_texture", texture);
    selection_box->AddAttribute(new EG::Game::ObjectAttributeBasicTransformation(/*glm::scale(glm::vec3(0.01f, 0.01f, 0.01f))*/glm::mat4(1.0f)));
    material = new EG::Graphics::RenderingMaterial();
    material->SetTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_DECAL, "outline_texture");
    material->SetLit(false);
    material->SetCastsShadows(false);
    material->SetTranslucent(true);
    material->SetBlendingMode(EG::Graphics::RenderingMaterial::BLEND_ALPHA);
    material->SetColor(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
    selection_box->AddAttribute(new EG::Game::ObjectAttributeRenderingMesh("cube", material));
    game->SetSelectionBox(selection_box);

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

    // Particle System
    scene->GetMeshManager()->Add("quad", EG::Graphics::GenerateQuad());

    EG::Graphics::Camera *main_camera = new EG::Graphics::Camera(45.0f, glm::ivec2(graphics->GetViewportWidth(), graphics->GetViewportHeight()), glm::vec2(0.001f, 100.0f));
    main_camera->ComputeProjectionMatrix();
    main_camera->SetPosition(glm::vec3(-0.7f, 2.5f, 5.0f));
    main_camera->RotateByMouse(glm::vec2(0.0f, -200.0f));
    main_camera->Update();
    main_camera->SetCameraType(EG::Graphics::Camera::CAMERA_FPS);
    EG::Game::Object *camera_object = new EG::Game::Object("camera");
    camera_object->AddAttribute(new EG::Game::ObjectAttributeRenderingCamera(main_camera));

    // Add Objects
    EG::Game::ObjectManager *objects = game->GetScene()->GetObjectManager();
    objects->AddObject(object2);
    objects->AddObject(light_object);
    objects->AddObject(camera_object);
    objects->AddObject(selection_box);
    game->GetScene()->SetCurrentCamera(main_camera);
    // NOTE: End Test Data

    while (game->GetWindow()->IsOpened()){
        game->Update();
        game->Render();
        glFinish();
    }
    window->Close();
    std::cout << "Exiting..." << std::endl;

    return 0;
}
