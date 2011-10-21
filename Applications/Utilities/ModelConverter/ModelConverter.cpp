#include "ModelConverter.h"

#include "../../../Engine/Graphics/RendererMultipass.h"
#include "../../../Engine/Graphics/RendererDeferred.h"
#include "../../../Engine/Game/ObjectBasicAttribute.h"
#include "../../../Engine/Game/ObjectEmissionAttribute.h"
#include "../../../Engine/Game/ObjectRenderingAttribute.h"
#include "../../../Engine/Utility/StringMethods.h"

ModelConverter::ModelConverter(EG::Utility::Window *_window, EG::Game::Scene *_scene) : Game(_window, _scene){
	gui = new EG::Utility::RocketInterface("Assets/GUIs/model_converter.rml", time, renderer->GetShaderManager(), input);
	use_gui = true;
	renderer->SetGUI(gui);
	LoadModelEventListener *load_model_event_listener = new LoadModelEventListener();
	load_model_event_listener->model_loaded = false;
	load_model_event_listener->scene = scene;
	gui->RegisterEventListener("click", "load_model_button", load_model_event_listener);
}

ModelConverter::~ModelConverter(void){
	//
}

void ModelConverter::Update(void){
	float movement_speed = time->GetFrameTime() * 2.0f;
	if (input->IsMouseDown(EG::Input::mouse_right)){
		renderer->GetCamera()->RotateByMouse(input->GetMouseDelta() * 2.0f);
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

void LoadModelEventListener::ProcessEvent(Rocket::Core::Event &event){
	Rocket::Core::Element *path_element = document->GetElementById("model_path");
	const Rocket::Core::Variant *path_variant = path_element->GetAttribute("value");
	if (path_variant){
		std::string model_path = (path_variant->Get<Rocket::Core::String>()).CString();
		model_path = EG::Utility::StringMethods::RemoveSpecialCharactersFromPathString(model_path);
		model = new EG::Media::ModelLoader(scene);
		model_loaded = model->Load(model_path);
		if (model_loaded){
			EG::Game::Object *model_object = new EG::Game::Object("SpaceShip");
			model_object->AddAttribute(new EG::Game::ObjectAttributeBasicTransformation(glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.0f, 0.0f)), glm::vec3(0.02f, 0.02f, 0.02f))));
			model->GetMaterial(0)->SetLit(true);
			model->GetMaterial(0)->SetCastsShadows(true);
			model_object->AddAttribute(new EG::Game::ObjectAttributeRenderingMesh(model->GetMesh(0), model->GetMaterial(0)));
			scene->GetObjectManager()->AddObject(model_object);
			document->GetElementById("load_model_inputs")->SetInnerRML((model_path + ":").c_str());

			// Add new controls to RML
			std::string model_options_rml = "<br />Is Lit: <input type=\"checkbox\" id=\"lit\" />";
			document->GetElementById("model_options")->SetInnerRML(model_options_rml.c_str());

			if (model->GetMaterial(0)->GetLit()){
				std::string checked_value = "";
				//document->GetElementById("lit")->GetAttribute<std::string>("checked", checked_value);
				checked_value = document->GetElementById("lit")->GetAttribute("type")->Get<std::string>();
				std::cout << "Checked: " << checked_value << std::endl;
				document->GetElementById("lit")->SetAttribute("checked", "checked");
				//document->GetElementById("lit")->GetAttribute<std::string>("checked", checked_value);
				checked_value = document->GetElementById("lit")->GetAttribute("type")->Get<std::string>();
				std::cout << "Checked: " << checked_value << std::endl;
			}

			/*Rocket::Core::Element *lit_break = new Rocket::Core::Element("br");
			Rocket::Core::Element *lit_checkbox_label = new Rocket::Core::Element("p");
			lit_checkbox_label->SetInnerRML("Is Lit: ");
			Rocket::Core::Element *lit_checkbox = new Rocket::Core::Element("input");
			lit_checkbox->SetAttribute("type", "checkbox");
			document->GetElementById("model_options")->AppendChild(lit_break);
			document->GetElementById("model_options")->AppendChild(lit_checkbox_label);
			document->GetElementById("model_options")->AppendChild(lit_checkbox);*/
		}else{
			document->GetElementById("loading_output_error")->SetInnerRML("Model Doesn't Exist Apparently... note: paths must be from where you ran this executable!");
		}
	}
	
}
