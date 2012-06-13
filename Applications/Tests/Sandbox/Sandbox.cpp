#include "Sandbox.h"
#include "../../../Engine/Game/ObjectBasicAttribute.h"
#include "../../../Engine/Game/ObjectControlAttribute.h"
#include "../../../Engine/Game/ObjectEmissionAttribute.h"
#include "../../../Engine/Game/ObjectRenderingAttribute.h"
#include "../../../Engine/Graphics/Renderer.h"
#include "../../../Engine/Graphics/RendererDeferred.h"

Sandbox::Sandbox(EG::Utility::Window *_window, EG::Game::Scene *_scene) : Game(_window, _scene){
    //
}

Sandbox::~Sandbox(void){
    //
}

void Sandbox::Update(void){
    float movement_speed = time->GetFrameTime() * 2.0f;
    if (input->IsMouseDown(EG::Input::mouse_right)){
        scene->GetCurrentCamera()->RotateByMouse(input->GetMouseDelta());
    }
    if (input->IsKeyDown(EG::Input::q)){
        scene->GetCurrentCamera()->Rotate(glm::vec3(0.0f, 0.0f, -movement_speed));
    }
    if (input->IsKeyDown(EG::Input::e)){
        scene->GetCurrentCamera()->Rotate(glm::vec3(0.0f, 0.0f, movement_speed));
    }
    if (input->IsKeyDown(EG::Input::w)){
        scene->GetCurrentCamera()->Move(glm::vec3(0.0f, 0.0f, -movement_speed));
    }
    if (input->IsKeyDown(EG::Input::s)){
        scene->GetCurrentCamera()->Move(glm::vec3(0.0f, 0.0f, movement_speed));
    }
    if (input->IsKeyDown(EG::Input::a)){
        scene->GetCurrentCamera()->Move(glm::vec3(-movement_speed, 0.0f, 0.0f));
    }
    if (input->IsKeyDown(EG::Input::d)){
        scene->GetCurrentCamera()->Move(glm::vec3(movement_speed, 0.0f, 0.0f));
    }
    if (input->IsKeyDown(EG::Input::space)){
        scene->GetCurrentCamera()->Move(glm::vec3(0.0f, movement_speed, 0.0f));
    }
    if (input->IsKeyDown(EG::Input::c)){
        scene->GetCurrentCamera()->Move(glm::vec3(0.0f, -movement_speed, 0.0f));
    }
    if (input->IsKeyDown(EG::Input::i)){
        EG::Game::Object *ship = scene->GetObjectManager()->GetObjectByName("test_model.ego");
        std::vector<EG::Game::ObjectAttribute *> *attrs = ship->GetAttributesByType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_CONTROL_RIGID_BODY);
        std::vector<EG::Game::ObjectAttribute *>::iterator attr_iter = attrs->begin();
        while (attr_iter != attrs->end()){
            EG::Game::ObjectAttributeControlRigidBody *body_attr = static_cast<EG::Game::ObjectAttributeControlRigidBody *>(*attr_iter);
            EG::Dynamics::RigidBody *ship_body = body_attr->GetBody();
            ship_body->ApplyImpulse(glm::vec3(-5.0f * time->GetFrameTime(), 0.0f, 0.0f));
            ++attr_iter;
        }
    }
    if (input->IsKeyDown(EG::Input::k)){
        EG::Game::Object *ship = scene->GetObjectManager()->GetObjectByName("test_model.ego");
        std::vector<EG::Game::ObjectAttribute *> *attrs = ship->GetAttributesByType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_CONTROL_RIGID_BODY);
        std::vector<EG::Game::ObjectAttribute *>::iterator attr_iter = attrs->begin();
        while (attr_iter != attrs->end()){
            EG::Game::ObjectAttributeControlRigidBody *body_attr = static_cast<EG::Game::ObjectAttributeControlRigidBody *>(*attr_iter);
            EG::Dynamics::RigidBody *ship_body = body_attr->GetBody();
            ship_body->ApplyImpulse(glm::vec3(5.0f * time->GetFrameTime(), 0.0f, 0.0f));
            ++attr_iter;
        }
    }
    if (input->IsKeyDown(EG::Input::u)){
        EG::Game::Object *ship = scene->GetObjectManager()->GetObjectByName("test_model.ego");
        std::vector<EG::Game::ObjectAttribute *> *attrs = ship->GetAttributesByType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_CONTROL_RIGID_BODY);
        std::vector<EG::Game::ObjectAttribute *>::iterator attr_iter = attrs->begin();
        while (attr_iter != attrs->end()){
            EG::Game::ObjectAttributeControlRigidBody *body_attr = static_cast<EG::Game::ObjectAttributeControlRigidBody *>(*attr_iter);
            EG::Dynamics::RigidBody *ship_body = body_attr->GetBody();
            ship_body->ApplyImpulse(glm::vec3(0.0f, 5.0f * time->GetFrameTime(), 0.0f));
            ++attr_iter;
        }
    }
    if (input->IsKeyDown(EG::Input::j)){
        EG::Game::Object *ship = scene->GetObjectManager()->GetObjectByName("test_model.ego");
        std::vector<EG::Game::ObjectAttribute *> *attrs = ship->GetAttributesByType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_CONTROL_RIGID_BODY);
        std::vector<EG::Game::ObjectAttribute *>::iterator attr_iter = attrs->begin();
        while (attr_iter != attrs->end()){
            EG::Game::ObjectAttributeControlRigidBody *body_attr = static_cast<EG::Game::ObjectAttributeControlRigidBody *>(*attr_iter);
            EG::Dynamics::RigidBody *ship_body = body_attr->GetBody();
            ship_body->ApplyImpulse(glm::vec3(0.0f, -5.0f * time->GetFrameTime(), 0.0f));
            ++attr_iter;
        }
    }
    if (input->IsKeyPressed(EG::Input::t)){
        if (rendering_method == RENDERER_DEFERRED){
            (static_cast<EG::Graphics::RendererDeferred *>(renderer))->CycleOutputType();
        }
    }
    if (input->IsKeyPressed(EG::Input::b)){
        if (rendering_method == RENDERER_DEFERRED){
            (static_cast<EG::Graphics::RendererDeferred *>(renderer))->ToggleBloom();
        }
    }
    if (input->IsKeyPressed(EG::Input::o)){
        if (rendering_method == RENDERER_DEFERRED){
            (static_cast<EG::Graphics::RendererDeferred *>(renderer))->ToggleSSAO();
        }
    }
    if (input->IsKeyPressed(EG::Input::n)){
        if (rendering_method == RENDERER_DEFERRED){
            (static_cast<EG::Graphics::RendererDeferred *>(renderer))->ToggleNormalMapping();
        }
    }
    if (input->IsKeyPressed(EG::Input::m)){
        if (rendering_method == RENDERER_DEFERRED){
            (static_cast<EG::Graphics::RendererDeferred *>(renderer))->ToggleShadowMapping();
        }
    }
    if (input->IsKeyPressed(EG::Input::f)){
        if (rendering_method == RENDERER_DEFERRED){
            (static_cast<EG::Graphics::RendererDeferred *>(renderer))->ToggleDOF();
        }
    }

    EG::Game::Object *ship = scene->GetObjectManager()->GetObjectByName("test_model.ego");
    if (ship){
        std::vector<EG::Game::ObjectAttribute *> *attrs = ship->GetAttributesByType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_BASIC_TRANSFORMATION);
        std::vector<EG::Game::ObjectAttribute *>::iterator attr_iter = attrs->begin();
        glm::vec4 pos = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
        while (attr_iter != attrs->end()){
            EG::Game::ObjectAttributeBasicTransformation *trans_attr = static_cast<EG::Game::ObjectAttributeBasicTransformation *>(*attr_iter);
            glm::mat4 offset = glm::translate(3.0f, 1.5f, 0.0f);
            glm::mat4 trans = offset * trans_attr->GetTransformation();
            pos = trans * pos;
            ++attr_iter;
        }
        attrs = ship->GetAttributesByType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_RENDERING_CAMERA);
        attr_iter = attrs->begin();
        while (attr_iter != attrs->end()){
            EG::Game::ObjectAttributeRenderingCamera *cam_attr = static_cast<EG::Game::ObjectAttributeRenderingCamera *>(*attr_iter);
            EG::Graphics::Camera *c = cam_attr->GetCamera();
            c->SetPosition(glm::vec3(pos.x, pos.y, pos.z));
            ++attr_iter;
        }
    }

    physics->Update(time->GetFrameTime());
}
