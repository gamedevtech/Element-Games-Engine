#include "ModelConverter.h"

#include "../../../Engine/Graphics/RendererMultipass.h"
#include "../../../Engine/Graphics/RendererDeferred.h"
#include "../../../Engine/Game/ObjectBasicAttribute.h"
#include "../../../Engine/Game/ObjectEmissionAttribute.h"
#include "../../../Engine/Game/ObjectRenderingAttribute.h"

ModelConverter::ModelConverter(EG::Utility::Window *_window, EG::Game::Scene *_scene) : Game(_window, _scene){
	model_path = "Assets/Models/spaceship.3ds";

	model = new EG::Media::ModelLoader(scene);
	model->Load(model_path);
	model_object = new EG::Game::Object("SpaceShip");
	model_object->AddAttribute(new EG::Game::ObjectAttributeBasicTransformation(glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.2f, 0.0f)), glm::vec3(0.01f, 0.01f, 0.01f))));
	model->GetMaterial(0)->SetLit(true);
	model->GetMaterial(0)->SetCastsShadows(true);
	model_object->AddAttribute(new EG::Game::ObjectAttributeRenderingMesh(model->GetMesh(0), model->GetMaterial(0)));
	scene->GetObjectManager()->AddObject(model_object);
}

ModelConverter::~ModelConverter(void){
	//
}

void ModelConverter::Update(void){
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
