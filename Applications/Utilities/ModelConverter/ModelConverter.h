#ifndef MODEL_CONVERTER_H
#define MODEL_CONVERTER_H

#include "../../../Engine/Media/ModelLoader.h"
#include "../../../Engine/Game/Object.h"
#include "../../../Engine/Game/Game.h"
#include "../../../Engine/Utility/GUI.h"

class ModelConverter : public EG::Game::Game{
	public:
		ModelConverter(EG::Utility::Window *_window, EG::Game::Scene *_scene);
		~ModelConverter(void);

		void Update(void);
};

class LoadModelEventListener : public EG::GUI::WebResourceResponse{
	public:
		virtual std::string Call(std::map<std::string, std::string> args);
		bool model_loaded;
		EG::Game::Scene *scene;
		EG::Media::ModelLoader *model;
		EG::Game::Object *model_object;
};

class SetLitCallback : public EG::GUI::WebResourceResponse{
	public:
		virtual std::string Call(std::map<std::string, std::string> args);
		EG::Game::Object *object;
};

class SetShadowsCallback : public EG::GUI::WebResourceResponse{
	public:
		virtual std::string Call(std::map<std::string, std::string> args);
		EG::Game::Object *object;
};

class SetDecalCallback : public EG::GUI::WebResourceResponse{
	public:
		virtual std::string Call(std::map<std::string, std::string> args);
		EG::Game::Object *object;
		EG::Game::Scene *scene;
};
class SetNormalCallback : public EG::GUI::WebResourceResponse{
	public:
		virtual std::string Call(std::map<std::string, std::string> args);
		EG::Game::Object *object;
		EG::Game::Scene *scene;
};
class SetHeightCallback : public EG::GUI::WebResourceResponse{
	public:
		virtual std::string Call(std::map<std::string, std::string> args);
		EG::Game::Object *object;
		EG::Game::Scene *scene;
};
class SetSpecularCallback : public EG::GUI::WebResourceResponse{
	public:
		virtual std::string Call(std::map<std::string, std::string> args);
		EG::Game::Object *object;
		EG::Game::Scene *scene;
};

class SaveCallback : public EG::GUI::WebResourceResponse{
	public:
		virtual std::string Call(std::map<std::string, std::string> args);
		EG::Game::Object *object;
		EG::Game::Scene *scene;
};

#endif

