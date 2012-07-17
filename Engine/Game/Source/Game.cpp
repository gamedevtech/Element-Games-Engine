#include "../Game.h"
#include "../ObjectBasicAttribute.h"
#include "../ObjectControlAttribute.h"
#include "../ObjectEmissionAttribute.h"
#include "../ObjectRenderingAttribute.h"
#include "../../Media/AssimpInterface.h"
#include "../../Graphics/RendererDeferred.h"
#include "../../Graphics/RendererMultipass.h"
#include "../../Utility/UnsignedIntDictionary.h"

namespace EG{
    namespace Game{
        Game::Game(EG::Utility::Window *_window, EG::Game::Scene *_scene){
            window = _window;
            scene = _scene;
            input = new EG::Input::Input();
            sfml_interface->SetInput(input);
            time = new EG::Utility::Time();
            graphics->Initialize(window->GetResolutionWidth(), window->GetResolutionHeight());

            if (!(graphics->CheckVersion(3, 0))) {
                window->Close();
            }

            // Resolution needs to be from config
            bool deferred = true;
            if (deferred) {
                rendering_method = RENDERER_DEFERRED;
                renderer = new EG::Graphics::RendererDeferred();
                (static_cast<EG::Graphics::RendererDeferred *>(renderer))->Initialize();
            } else {
                rendering_method = RENDERER_SIMPLE;
                renderer = new EG::Graphics::Renderer();
                renderer->Initialize();
            }

            physics = new EG::Dynamics::Physics();

            time->Update();
        }

        Game::~Game(void){
            //
        }

        void Game::Update(void){
            //
        }

        void Game::Render(void){
            PostUpdates();
            if (rendering_method == RENDERER_SIMPLE){
                renderer->Render(scene, time);
            }else if (rendering_method == RENDERER_MULTIPASS){
                (static_cast<EG::Graphics::RendererMultipass *>(renderer))->Render(scene);
            }else if (rendering_method == RENDERER_DEFERRED){
                (static_cast<EG::Graphics::RendererDeferred *>(renderer))->Render(scene, time);
            }
            window->Display();
            PreUpdates();
        }

        void Game::PreUpdates(void){
            time->Update();
            window->Update();
        }

        void Game::PostUpdates(void){
            physics->Update(time->GetFrameTime());

            if (gui->GetInitialized()){
                if (input->IsMousePressed(EG::Input::mouse_left)) {
                    gui->InjectMouseDown(AWE_MB_LEFT);
                }
                if (input->IsMouseReleased(EG::Input::mouse_left)) {
                    gui->InjectMouseUp(AWE_MB_LEFT);
                }
                glm::vec2 mp = input->GetMousePosition();
                gui->InjectMouseMove((unsigned int)(mp.x), (unsigned int)(mp.y));

                std::vector<char> text = input->GetTextEntered();
                std::vector<char>::iterator text_iter = text.begin();
                if (input->IsKeyPressed(EG::Input::back_space)) {
                    gui->InjectKeyPress('\b');
                    std::cout << "Backspace" << std::endl;
                }
                if (input->IsKeyPressed(EG::Input::del)){
                    gui->InjectKeyPress(char(127));
                }
                if (input->IsKeyPressed(EG::Input::tab)){
                    gui->InjectKeyPress('\t');
                }
                if (input->IsKeyPressed(EG::Input::left)){
                    //gui->InjectKeyPress(Awesomium::KeyCodes::AK_LEFT);
                }
                if (input->IsKeyPressed(EG::Input::right)){
                    //gui->InjectKeyPress(Awesomium::KeyCodes::AK_RIGHT);
                }
                if (input->IsKeyPressed(EG::Input::up)){
                    //gui->InjectKeyPress(Awesomium::KeyCodes::AK_UP);
                }
                if (input->IsKeyPressed(EG::Input::down)){
                    //gui->InjectKeyPress(Awesomium::KeyCodes::AK_DOWN);
                }
                while (text_iter != text.end()){
                    char c = (*text_iter);
                    int cint = int(c);
                    gui->InjectKeyPress(cint);
                    ++text_iter;
                }

                gui->Update();
            }

            input->Update();

            EG::Utility::UnsignedIntDictionaryKeysIterator object_iter = scene->GetObjectManager()->GetObjects()->GetKeysBegin();
            while (object_iter != scene->GetObjectManager()->GetObjects()->GetKeysEnd()){
                unsigned int object_id = (*object_iter);
                EG::Game::Object *object = scene->GetObjectManager()->GetObjectById(object_id);

                if (object->HasAttributesOfType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_EMISSION_PARTICLE_SYSTEM)){
                    std::vector<EG::Game::ObjectAttribute *> *attrs = object->GetAttributesByType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_EMISSION_PARTICLE_SYSTEM);
                    std::vector<EG::Game::ObjectAttribute *>::iterator attr_iter = attrs->begin();
                    while (attr_iter != attrs->end()){
                        EG::Game::ObjectAttributeEmissionParticleSystem *pattr = static_cast<EG::Game::ObjectAttributeEmissionParticleSystem *>(*attr_iter);

                        EG::Graphics::ParticleSystem *psys = pattr->GetParticleSystem();
                        std::list<EG::Graphics::Particle *> *particles = psys->GetParticles();
                        std::list<EG::Graphics::Particle *>::iterator piter = particles->begin();
                        while (piter != particles->end()){
                            EG::Graphics::Particle *p = (*piter);
                            bool has_physics = false;
                            glm::mat4 motion_state;
                            if (p->HasAttributesOfType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_CONTROL_RIGID_BODY)){
                                std::vector<EG::Game::ObjectAttribute *> *attrs = p->GetAttributesByType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_CONTROL_RIGID_BODY);
                                std::vector<EG::Game::ObjectAttribute *>::iterator attr_iter = attrs->begin();
                                while (attr_iter != attrs->end()){
                                    EG::Game::ObjectAttributeControlRigidBody *rigid_body_attr = static_cast<EG::Game::ObjectAttributeControlRigidBody *>(*attr_iter);
                                    if (!(rigid_body_attr->GetConnected())){
                                        physics->AddRigidBody(rigid_body_attr->GetBody());
                                        rigid_body_attr->SetConnected(true);
                                    }

                                    if (!(p->Alive())){
                                        physics->RemoveRigidBody(rigid_body_attr->GetBody());
                                    }

                                    // TODO: Get Motion State From Physics
                                    EG::Dynamics::RigidBody *body = rigid_body_attr->GetBody();
                                    motion_state = body->GetMotionState();
                                    has_physics = true;

                                    ++attr_iter;
                                }
                            }

                            if (has_physics && p->HasAttributesOfType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_BASIC_TRANSFORMATION)){
                                std::vector<EG::Game::ObjectAttribute *> *attrs = p->GetAttributesByType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_BASIC_TRANSFORMATION);
                                std::vector<EG::Game::ObjectAttribute *>::iterator attr_iter = attrs->begin();
                                while (attr_iter != attrs->end()){
                                    EG::Game::ObjectAttributeBasicTransformation *trans_attr = static_cast<EG::Game::ObjectAttributeBasicTransformation *>(*attr_iter);
                                    glm::mat4 t = trans_attr->GetTransformation();
                                    glm::vec3 p(t[3][0], t[3][1], t[3][2]);
                                    glm::vec3 c = scene->GetCurrentCamera()->GetPosition();
                                    float distance = glm::distance(p, c);
                                    trans_attr->SetTransformation(motion_state);
                                    ++attr_iter;
                                }
                            }
                            ++piter;
                        }

                        pattr->GetParticleSystem()->Update(time->GetFrameTime());
                        ++attr_iter;
                    }
                }

                if (object->HasAttributesOfType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_CONTROL_ANIMATION)) {
                    EG::Dynamics::AnimationState *animations = (static_cast<EG::Game::ObjectAttributeControlAnimationState *>(object->GetAttributesByType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_CONTROL_ANIMATION)->at(0)))->GetAnimationState();
                    animations->Update(time->GetFrameTime());
                }

                bool has_physics = false;
                glm::mat4 motion_state;
                if (object->HasAttributesOfType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_CONTROL_RIGID_BODY)){
                    std::vector<EG::Game::ObjectAttribute *> *attrs = object->GetAttributesByType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_CONTROL_RIGID_BODY);
                    std::vector<EG::Game::ObjectAttribute *>::iterator attr_iter = attrs->begin();
                    while (attr_iter != attrs->end()){
                        EG::Game::ObjectAttributeControlRigidBody *rigid_body_attr = static_cast<EG::Game::ObjectAttributeControlRigidBody *>(*attr_iter);
                        if (!(rigid_body_attr->GetConnected())){
                            physics->AddRigidBody(rigid_body_attr->GetBody());
                            rigid_body_attr->SetConnected(true);
                        }

                        // TODO: Get Motion State From Physics
                        EG::Dynamics::RigidBody *body = rigid_body_attr->GetBody();
                        motion_state = body->GetMotionState();
                        has_physics = true;

                        ++attr_iter;
                    }
                }

                if (has_physics && object->HasAttributesOfType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_BASIC_TRANSFORMATION)){
                    std::vector<EG::Game::ObjectAttribute *> *attrs = object->GetAttributesByType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_BASIC_TRANSFORMATION);
                    std::vector<EG::Game::ObjectAttribute *>::iterator attr_iter = attrs->begin();
                    while (attr_iter != attrs->end()){
                        EG::Game::ObjectAttributeBasicTransformation *trans_attr = static_cast<EG::Game::ObjectAttributeBasicTransformation *>(*attr_iter);
                        trans_attr->SetTransformation(motion_state);
                        ++attr_iter;
                    }
                }

                std::vector<EG::Game::ObjectScript *> *scripts = object->GetScripts();
                std::vector<EG::Game::ObjectScript *>::iterator script_iter = scripts->begin();
                while (script_iter != scripts->end()) {
                    EG::Game::ObjectScript *script = (*script_iter);
                    EG::Game::ObjectScript::object = object;
                    script->Run();
                    ++script_iter;
                }

                ++object_iter;
            }
        }

        EG::Utility::Window *Game::GetWindow(void){
            return window;
        }

        EG::Game::Scene *Game::GetScene(void){
            return scene;
        }

        EG::Graphics::Renderer *Game::GetRenderer(void){
            return renderer;
        }

        EG::Input::Input *Game::GetInput(void){
            return input;
        }

        EG::Utility::Time *Game::GetTime(void){
            return time;
        }
    }
}
