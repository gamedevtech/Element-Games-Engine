#include <iostream>
#include <string>

#include "SpaceSim.h"

int main(int argc, char **argv) {
    EG::Utility::JSON config_file("config.json");
    EG::Utility::Window *window = new EG::Utility::Window(config_file.GetInt("screen.width"),
                                                          config_file.GetInt("screen.height"),
                                                          32,
                                                          config_file.GetBool("screen.fullscreen"),
                                                          "Element Games Space Sim Client");
    EG::Game::Scene *scene = new EG::Game::Scene();
    SpaceSim *game = new SpaceSim(window, scene);

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
    game->GetScene()->SetCurrentCamera(main_camera);
    // NOTE: End Test Data

    while (game->GetWindow()->IsOpened()){
        game->Update();
        game->Render();
        glFinish();
    }
    window->Close();

    return 0;
}
