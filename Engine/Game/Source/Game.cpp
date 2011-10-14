#include "../Game.h"
#include "../../Graphics/RendererMultipass.h"
#include "../../Graphics/RendererDeferred.h"
#include "../../Media/AssimpInterface.h"

namespace EG{
	namespace Game{
		Game::Game(EG::Utility::Window *_window, EG::Game::Scene *_scene){
			window = _window;
			scene = _scene;
			input = new EG::Input::Input();
			sfml_interface->SetInput(input);
			time = new EG::Utility::Time();
			graphics->Initialize(window->GetResolutionWidth(), window->GetResolutionHeight());

			// Resolution needs to be from config
			rendering_method = RENDERER_SIMPLE;

			if (graphics->CheckVersion(3)){
				if (rendering_method == RENDERER_SIMPLE){
					rendering_method = RENDERER_DEFERRED;
				}
			}else{
				rendering_method = RENDERER_SIMPLE;
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
			gui = new EG::Utility::GUI();

			time->Update();
		}

		Game::~Game(void){
			//
		}

		void Game::Update(void){
			float movement_speed = time->GetFrameTime() * 2.0f;
			if (input->IsMouseDown(EG::Input::mouse_right)){
				renderer->GetCamera()->RotateByMouse(input->GetMouseDelta());
			}
			if (input->IsKeyDown(EG::Input::q)){
				renderer->GetCamera()->Rotate(glm::vec3(0.0f, 0.0f, -movement_speed));
			}
			if (input->IsKeyDown(EG::Input::e)){
				renderer->GetCamera()->Rotate(glm::vec3(0.0f, 0.0f, movement_speed));
			}
			if (input->IsKeyDown(EG::Input::w)){
				renderer->GetCamera()->Move(glm::vec3(0.0f, 0.0f, -movement_speed));
			}
			if (input->IsKeyDown(EG::Input::s)){
				renderer->GetCamera()->Move(glm::vec3(0.0f, 0.0f, movement_speed));
			}
			if (input->IsKeyDown(EG::Input::a)){
				renderer->GetCamera()->Move(glm::vec3(-movement_speed, 0.0f, 0.0f));
			}
			if (input->IsKeyDown(EG::Input::d)){
				renderer->GetCamera()->Move(glm::vec3(movement_speed, 0.0f, 0.0f));
			}
			if (input->IsKeyDown(EG::Input::space)){
				renderer->GetCamera()->Move(glm::vec3(0.0f, movement_speed, 0.0f));
			}
			if (input->IsKeyDown(EG::Input::c)){
				renderer->GetCamera()->Move(glm::vec3(0.0f, -movement_speed, 0.0f));
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
		}

		void Game::Render(void){
			PostUpdates();
			if (rendering_method == RENDERER_SIMPLE){
				renderer->Render(scene, time->GetFrameTime());
			}else if (rendering_method == RENDERER_MULTIPASS){
				(static_cast<EG::Graphics::RendererMultipass *>(renderer))->Render(scene);
			}else if (rendering_method == RENDERER_DEFERRED){
				(static_cast<EG::Graphics::RendererDeferred *>(renderer))->Render(scene, time->GetFrameTime());
			}
			window->Display();
			PreUpdates();
		}

		void Game::PreUpdates(void){
			time->Update();
			window->Update();
		}

		void Game::PostUpdates(void){
			input->Update();
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
