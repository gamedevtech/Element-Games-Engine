#include "ModelConverter.h"

#include "../../../Engine/Graphics/RendererMultipass.h"
#include "../../../Engine/Graphics/RendererDeferred.h"
#include "../../../Engine/Game/ObjectBasicAttribute.h"
#include "../../../Engine/Game/ObjectEmissionAttribute.h"
#include "../../../Engine/Game/ObjectRenderingAttribute.h"
#include "../../../Engine/Utility/StringMethods.h"
#include "../../../Engine/Media/ObjectWriter.h"

ModelConverter::ModelConverter(EG::Utility::Window *_window, EG::Game::Scene *_scene) : Game(_window, _scene){
	gui = new EG::GUI::GUI("Assets/GUIs/ModelConverter", "index.html");
	//gui->ExecuteScript("window.location = 'http://slashdot.org';");
	use_gui = true;
	renderer->SetGUI(gui);
	/*LoadModelEventListener *load_model_event_listener = new LoadModelEventListener();
	load_model_event_listener->model_loaded = false;
	load_model_event_listener->scene = scene;
	load_model_event_listener->gui = gui;
	gui->RegisterEventListener("click", "load_model_button", load_model_event_listener);*/
}

ModelConverter::~ModelConverter(void){
	//
}

void ModelConverter::Update(void){
	float movement_speed = time->GetFrameTime() * 2.0f;
	if (input->IsMouseDown(EG::Input::mouse_right)){
		scene->GetCurrentCamera()->RotateByMouse(input->GetMouseDelta() * 2.0f);
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
/*
void LoadModelEventListener::ProcessEvent(EG::Utility::Event &event){
	Rocket::Core::Element *path_element = document->GetElementById("model_path");
	const Rocket::Core::Variant *path_variant = path_element->GetAttribute("value");
	if (path_variant){
		std::string model_path = (path_variant->Get<Rocket::Core::String>()).CString();
		model_path = EG::Utility::StringMethods::RemoveSpecialCharactersFromPathString(model_path);
		model = new EG::Media::ModelLoader(scene);
		model_loaded = model->Load(model_path);
		if (model_loaded){
			EG::Game::Object *model_object = new EG::Game::Object(model_path);
			model_object->AddAttribute(new EG::Game::ObjectAttributeBasicTransformation(glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.0f, 0.0f)), glm::vec3(0.02f, 0.02f, 0.02f))));
			model->GetMaterial(0)->SetLit(true);
			model->GetMaterial(0)->SetCastsShadows(true);
			model_object->AddAttribute(new EG::Game::ObjectAttributeRenderingMesh(model->GetMesh(0), model->GetMaterial(0)));
			scene->GetObjectManager()->AddObject(model_object);
			document->GetElementById("load_model_inputs")->SetInnerRML((model_path + ":").c_str());

			// Add new controls to RML
			std::string model_options_rml = "Object Name: <input type=\"text\" id=\"object_name\" /><br /><br />Is Lit: <input type=\"checkbox\" id=\"lit\" />";
			model_options_rml += "&nbsp;&nbsp;&nbsp;&nbsp;Casts Shadows: <input type=\"checkbox\" id=\"shadows\" /><br />";
			model_options_rml += "<br />Decal: <input type=\"text\" id=\"decal\" />&nbsp;<button id=\"decal_button\">Set</button><br />";
			model_options_rml += "<br />Normal: <input type=\"text\" id=\"normal\" />&nbsp;<button id=\"normal_button\">Set</button><br />";
			model_options_rml += "<br />Height: <input type=\"text\" id=\"height\" />&nbsp;<button id=\"height_button\">Set</button><br />";
			model_options_rml += "<br />Specular: <input type=\"text\" id=\"specular\" />&nbsp;<button id=\"specular_button\">Set</button><br /><br /><br />";
			model_options_rml += "<br />Images Output Path: <input type=\"text\" id=\"images_output_path\" value=\"Assets/Textures/\"/><br />Model Output Path: <input type=\"text\" id=\"model_output_path\" value=\"Assets/Models/\" />";
			model_options_rml += "<br />Save As: <input type=\"text\" id=\"file_out\" />&nbsp;<button id=\"save_button\">Save</button>";
			document->GetElementById("model_options")->SetInnerRML(model_options_rml.c_str());

			// Values
			document->GetElementById("object_name")->SetAttribute("value", model_object->GetObjectName().c_str());
			if (model->GetMaterial(0)->GetLit()){
				document->GetElementById("lit")->SetAttribute("checked", "checked");
			}
			if (model->GetMaterial(0)->GetCastsShadows()){
				document->GetElementById("shadows")->SetAttribute("checked", "checked");
			}
			if (model->GetMaterial(0)->HasTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_DECAL)){
				std::string file_path = scene->GetTextureManager()->GetTexture(model->GetMaterial(0)->GetTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_DECAL))->GetFilePath();
				document->GetElementById("decal")->SetAttribute("value", file_path.c_str());
			}
			if (model->GetMaterial(0)->HasTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_NORMAL)){
				std::string file_path = scene->GetTextureManager()->GetTexture(model->GetMaterial(0)->GetTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_NORMAL))->GetFilePath();
				document->GetElementById("normal")->SetAttribute("value", file_path.c_str());
			}
			if (model->GetMaterial(0)->HasTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_HEIGHT)){
				std::string file_path = scene->GetTextureManager()->GetTexture(model->GetMaterial(0)->GetTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_HEIGHT))->GetFilePath();
				document->GetElementById("height")->SetAttribute("value", file_path.c_str());
			}
			if (model->GetMaterial(0)->HasTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_SPECULAR)){
				std::string file_path = scene->GetTextureManager()->GetTexture(model->GetMaterial(0)->GetTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_SPECULAR))->GetFilePath();
				document->GetElementById("specular")->SetAttribute("value", file_path.c_str());
			}

			// Event Listeners
			SetLitEventListener *lit_event_listener = new SetLitEventListener();
			lit_event_listener->object = model_object;
			gui->RegisterEventListener("click", "lit", lit_event_listener);

			SetShadowsEventListener *shadows_event_listener = new SetShadowsEventListener();
			shadows_event_listener->object = model_object;
			gui->RegisterEventListener("click", "shadows", shadows_event_listener);

			DecalButtonEventListener *decal_button_event_listener = new DecalButtonEventListener();
			decal_button_event_listener->object = model_object;
			decal_button_event_listener->scene = scene;
			gui->RegisterEventListener("click", "decal_button", decal_button_event_listener);

			NormalButtonEventListener *normal_button_event_listener = new NormalButtonEventListener();
			normal_button_event_listener->object = model_object;
			normal_button_event_listener->scene = scene;
			gui->RegisterEventListener("click", "normal_button", normal_button_event_listener);

			HeightButtonEventListener *height_button_event_listener = new HeightButtonEventListener();
			height_button_event_listener->object = model_object;
			height_button_event_listener->scene = scene;
			gui->RegisterEventListener("click", "height_button", height_button_event_listener);

			SpecularButtonEventListener *specular_button_event_listener = new SpecularButtonEventListener();
			specular_button_event_listener->object = model_object;
			specular_button_event_listener->scene = scene;
			gui->RegisterEventListener("click", "specular_button", specular_button_event_listener);

			SaveFileButtonEventListener *save_event_listener = new SaveFileButtonEventListener();
			save_event_listener->object = model_object;
			save_event_listener->scene = scene;
			gui->RegisterEventListener("click", "save_button", save_event_listener);
		}else{
			document->GetElementById("loading_output_error")->SetInnerRML("Model Doesn't Exist Apparently... note: paths must be from where you ran this executable!");
		}
	}
}

void SetLitEventListener::ProcessEvent(EG::Utility::Event &event){
	// Meshes
	std::vector<EG::Game::ObjectAttribute *> *mesh_attributes = object->GetAttributesByType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_RENDERING_MESH);
	std::vector<EG::Game::ObjectAttribute *>::iterator mesh_attribute_iterator = mesh_attributes->begin();
	while (mesh_attribute_iterator != mesh_attributes->end()){
		EG::Game::ObjectAttributeRenderingMesh *mesh_attribute = static_cast<EG::Game::ObjectAttributeRenderingMesh *>(*mesh_attribute_iterator);
		EG::Graphics::RenderingMaterial *material = mesh_attribute->GetMaterial();

		bool lit_status = material->GetLit();
		material->SetLit(!lit_status);
		++mesh_attribute_iterator;
	}
}

void SetShadowsEventListener::ProcessEvent(EG::Utility::Event &event){
	// Meshes
	std::vector<EG::Game::ObjectAttribute *> *mesh_attributes = object->GetAttributesByType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_RENDERING_MESH);
	std::vector<EG::Game::ObjectAttribute *>::iterator mesh_attribute_iterator = mesh_attributes->begin();
	while (mesh_attribute_iterator != mesh_attributes->end()){
		EG::Game::ObjectAttributeRenderingMesh *mesh_attribute = static_cast<EG::Game::ObjectAttributeRenderingMesh *>(*mesh_attribute_iterator);
		EG::Graphics::RenderingMaterial *material = mesh_attribute->GetMaterial();

		bool shadows_status = material->GetCastsShadows();
		material->SetCastsShadows(!shadows_status);
		++mesh_attribute_iterator;
	}
}

void DecalButtonEventListener::ProcessEvent(EG::Utility::Event &event){
	// See if texture is already loaded... if not, load it
	//std::cout << "DONGLE" << std::endl;
	std::string new_texture_path = (document->GetElementById("decal")->GetAttribute("value")->Get<Rocket::Core::String>()).CString();
	new_texture_path = EG::Utility::StringMethods::RemoveSpecialCharactersFromPathString(new_texture_path);
	//std::cout << "DONGLE2" << std::endl;

	if (!(scene->GetTextureManager()->HasTexture(new_texture_path))){
		EG::Graphics::Texture *texture = new EG::Graphics::Texture(new_texture_path);
		scene->GetTextureManager()->AddTexture(new_texture_path, texture);
	}

	// Meshes
	std::vector<EG::Game::ObjectAttribute *> *mesh_attributes = object->GetAttributesByType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_RENDERING_MESH);
	std::vector<EG::Game::ObjectAttribute *>::iterator mesh_attribute_iterator = mesh_attributes->begin();
	while (mesh_attribute_iterator != mesh_attributes->end()){
		EG::Game::ObjectAttributeRenderingMesh *mesh_attribute = static_cast<EG::Game::ObjectAttributeRenderingMesh *>(*mesh_attribute_iterator);
		EG::Graphics::RenderingMaterial *material = mesh_attribute->GetMaterial();

		material->SetTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_DECAL, new_texture_path);
		++mesh_attribute_iterator;
	}
}

void NormalButtonEventListener::ProcessEvent(EG::Utility::Event &event){
	// See if texture is already loaded... if not, load it
	std::string new_texture_path = (document->GetElementById("normal")->GetAttribute("value")->Get<Rocket::Core::String>()).CString();
	new_texture_path = EG::Utility::StringMethods::RemoveSpecialCharactersFromPathString(new_texture_path);

	if (!(scene->GetTextureManager()->HasTexture(new_texture_path))){
		EG::Graphics::Texture *texture = new EG::Graphics::Texture(new_texture_path);
		scene->GetTextureManager()->AddTexture(new_texture_path, texture);
	}

	// Meshes
	std::vector<EG::Game::ObjectAttribute *> *mesh_attributes = object->GetAttributesByType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_RENDERING_MESH);
	std::vector<EG::Game::ObjectAttribute *>::iterator mesh_attribute_iterator = mesh_attributes->begin();
	while (mesh_attribute_iterator != mesh_attributes->end()){
		EG::Game::ObjectAttributeRenderingMesh *mesh_attribute = static_cast<EG::Game::ObjectAttributeRenderingMesh *>(*mesh_attribute_iterator);
		EG::Graphics::RenderingMaterial *material = mesh_attribute->GetMaterial();

		material->SetTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_NORMAL, new_texture_path);
		++mesh_attribute_iterator;
	}
}

void HeightButtonEventListener::ProcessEvent(EG::Utility::Event &event){
	// See if texture is already loaded... if not, load it
	std::string new_texture_path = (document->GetElementById("height")->GetAttribute("value")->Get<Rocket::Core::String>()).CString();
	new_texture_path = EG::Utility::StringMethods::RemoveSpecialCharactersFromPathString(new_texture_path);

	if (!(scene->GetTextureManager()->HasTexture(new_texture_path))){
		EG::Graphics::Texture *texture = new EG::Graphics::Texture(new_texture_path);
		scene->GetTextureManager()->AddTexture(new_texture_path, texture);
	}

	// Meshes
	std::vector<EG::Game::ObjectAttribute *> *mesh_attributes = object->GetAttributesByType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_RENDERING_MESH);
	std::vector<EG::Game::ObjectAttribute *>::iterator mesh_attribute_iterator = mesh_attributes->begin();
	while (mesh_attribute_iterator != mesh_attributes->end()){
		EG::Game::ObjectAttributeRenderingMesh *mesh_attribute = static_cast<EG::Game::ObjectAttributeRenderingMesh *>(*mesh_attribute_iterator);
		EG::Graphics::RenderingMaterial *material = mesh_attribute->GetMaterial();

		material->SetTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_HEIGHT, new_texture_path);
		++mesh_attribute_iterator;
	}
}

void SpecularButtonEventListener::ProcessEvent(EG::Utility::Event &event){
	// See if texture is already loaded... if not, load it
	std::string new_texture_path = (document->GetElementById("specular")->GetAttribute("value")->Get<Rocket::Core::String>()).CString();
	new_texture_path = EG::Utility::StringMethods::RemoveSpecialCharactersFromPathString(new_texture_path);

	if (!(scene->GetTextureManager()->HasTexture(new_texture_path))){
		EG::Graphics::Texture *texture = new EG::Graphics::Texture(new_texture_path);
		scene->GetTextureManager()->AddTexture(new_texture_path, texture);
	}

	// Meshes
	std::vector<EG::Game::ObjectAttribute *> *mesh_attributes = object->GetAttributesByType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_RENDERING_MESH);
	std::vector<EG::Game::ObjectAttribute *>::iterator mesh_attribute_iterator = mesh_attributes->begin();
	while (mesh_attribute_iterator != mesh_attributes->end()){
		EG::Game::ObjectAttributeRenderingMesh *mesh_attribute = static_cast<EG::Game::ObjectAttributeRenderingMesh *>(*mesh_attribute_iterator);
		EG::Graphics::RenderingMaterial *material = mesh_attribute->GetMaterial();

		material->SetTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_SPECULAR, new_texture_path);
		++mesh_attribute_iterator;
	}
}

void SaveFileButtonEventListener::ProcessEvent(EG::Utility::Event &event){
	std::string file_path = (document->GetElementById("file_out")->GetAttribute("value")->Get<Rocket::Core::String>()).CString();
	file_path = EG::Utility::StringMethods::RemoveSpecialCharactersFromPathString(file_path);

	EG::Media::ObjectWriter *writer = new EG::Media::ObjectWriter(object, scene, document->GetElementById("object_name")->GetAttribute("value")->Get<Rocket::Core::String>().CString(), document->GetElementById("images_output_path")->GetAttribute("value")->Get<Rocket::Core::String>().CString(), document->GetElementById("model_output_path")->GetAttribute("value")->Get<Rocket::Core::String>().CString());
	writer->Write(file_path);
	document->GetElementById("model_options")->SetInnerRML(("Model Saved To: " + file_path).c_str());
}
*/
