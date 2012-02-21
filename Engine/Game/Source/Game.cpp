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
			use_gui = false;
			window = _window;
			scene = _scene;
			input = new EG::Input::Input();
			sfml_interface->SetInput(input);
			time = new EG::Utility::Time();
			graphics->Initialize(window->GetResolutionWidth(), window->GetResolutionHeight());

			// Resolution needs to be from config
			rendering_method = RENDERER_SIMPLE;

			//if (graphics->CheckVersion(3, 1)){
			if (graphics->CheckVersion(4, 1)){
				if (rendering_method == RENDERER_SIMPLE){
					rendering_method = RENDERER_DEFERRED;
				}
			}else{
				rendering_method = RENDERER_SIMPLE;
				graphics->OverrideVersion(2, 1);
			}
			if (rendering_method == RENDERER_SIMPLE){
				renderer = new EG::Graphics::Renderer();
				renderer->Initialize();
			}else if (rendering_method == RENDERER_MULTIPASS){
				renderer = new EG::Graphics::RendererMultipass();
				(static_cast<EG::Graphics::RendererMultipass *>(renderer))->Initialize();
			}else if (rendering_method == RENDERER_DEFERRED){
				renderer = new EG::Graphics::RendererDeferred();
				(static_cast<EG::Graphics::RendererDeferred *>(renderer))->Initialize();
			}
			//gui = new EG::GUI::GUI();

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

			if (use_gui){
				if (input->IsMousePressed(EG::Input::mouse_left)) {
					gui->InjectMouseDown(Awesomium::LEFT_MOUSE_BTN);
				}
				if (input->IsMouseReleased(EG::Input::mouse_left)) {
					gui->InjectMouseUp(Awesomium::LEFT_MOUSE_BTN);
				}
				glm::vec2 mp = input->GetMousePosition();
				gui->InjectMouseMove((unsigned int)(mp.x), (unsigned int)(mp.y));

				std::vector<char> text = input->GetTextEntered();
				std::vector<char>::iterator text_iter = text.begin();
				if (input->IsKeyPressed(EG::Input::back_space)) {
					gui->InjectKeyPress(Awesomium::KeyCodes::AK_BACK);
				}
				if (input->IsKeyPressed(EG::Input::del)){
					gui->InjectKeyPress(Awesomium::KeyCodes::AK_DELETE);
				}
				if (input->IsKeyPressed(EG::Input::tab)){
					gui->InjectKeyPress(Awesomium::KeyCodes::AK_TAB);
				}
				if (input->IsKeyPressed(EG::Input::left)){
					gui->InjectKeyPress(Awesomium::KeyCodes::AK_LEFT);
				}
				if (input->IsKeyPressed(EG::Input::right)){
						gui->InjectKeyPress(Awesomium::KeyCodes::AK_RIGHT);
				}
				if (input->IsKeyPressed(EG::Input::up)){
						gui->InjectKeyPress(Awesomium::KeyCodes::AK_UP);
				}
				if (input->IsKeyPressed(EG::Input::down)){
						gui->InjectKeyPress(Awesomium::KeyCodes::AK_DOWN);
				}
				while (text_iter != text.end()){
					char c = (*text_iter);
					int cint = int(c);
					Awesomium::WebKeyboardEvent key_event;
					key_event.text[0] = c;
					key_event.unmodifiedText[0] = c;
					key_event.type = Awesomium::WebKeyboardEvent::TYPE_CHAR;
					key_event.virtualKeyCode = c;
					key_event.nativeKeyCode = c;
					gui->InjectKeyPress(key_event);
					++text_iter;
				}

				gui->Update();
			}

			input->Update();

			EG::Utility::UnsignedIntDictionaryKeysIterator object_iter = scene->GetObjectManager()->GetObjects()->GetKeysBegin();
			while (object_iter != scene->GetObjectManager()->GetObjects()->GetKeysEnd()){
				unsigned int object_id = (*object_iter);
				EG::Game::Object *object = scene->GetObjectManager()->GetObject(object_id);

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
	}
}
