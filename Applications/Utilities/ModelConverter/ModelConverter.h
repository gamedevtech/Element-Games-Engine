#ifndef MODEL_CONVERTER_H
#define MODEL_CONVERTER_H

#include "../../../Engine/Media/ModelLoader.h"
#include "../../../Engine/Game/Object.h"
#include "../../../Engine/Game/Game.h"

// Switch this to GUI.h when it's all done.
#include "../../../Engine/Utility/RocketInterface.h"

class ModelConverter : public EG::Game::Game{
	public:
		ModelConverter(EG::Utility::Window *_window, EG::Game::Scene *_scene);
		~ModelConverter(void);

		void Update(void);
	private:
		//std::string model_path;
		//EG::Media::ModelLoader *model;
		//EG::Game::Object *model_object;
};

class LoadModelEventListener : public EG::Utility::EventListener{
	public:
		bool model_loaded;
		EG::Game::Scene *scene;
		EG::Media::ModelLoader *model;
		EG::Utility::RocketInterface *gui;
		void ProcessEvent(EG::Utility::Event &event);
};

class SetLitEventListener : public EG::Utility::EventListener{
	public:
		EG::Game::Object *object;
		void ProcessEvent(EG::Utility::Event &event);
};

class SetShadowsEventListener : public EG::Utility::EventListener{
	public:
		EG::Game::Object *object;
		void ProcessEvent(EG::Utility::Event &event);
};

class DecalButtonEventListener : public EG::Utility::EventListener{
	public:
		EG::Game::Scene *scene;
		EG::Game::Object *object;
		void ProcessEvent(EG::Utility::Event &event);
};

class NormalButtonEventListener : public EG::Utility::EventListener{
	public:
		EG::Game::Scene *scene;
		EG::Game::Object *object;
		void ProcessEvent(EG::Utility::Event &event);
};

class HeightButtonEventListener : public EG::Utility::EventListener{
	public:
		EG::Game::Scene *scene;
		EG::Game::Object *object;
		void ProcessEvent(EG::Utility::Event &event);
};

class SpecularButtonEventListener : public EG::Utility::EventListener{
	public:
		EG::Game::Scene *scene;
		EG::Game::Object *object;
		void ProcessEvent(EG::Utility::Event &event);
};

#endif
