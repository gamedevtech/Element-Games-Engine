#ifndef MODEL_CONVERTER_H
#define MODEL_CONVERTER_H

#include "../../Engine/Media/ModelLoader.h"
#include "../../Engine/Game/Object.h"
#include "../../Engine/Game/Game.h"

class Editor : public EG::Game::Game{
    public:
        Editor(EG::Utility::Window *_window, EG::Game::Scene *_scene);
        ~Editor(void);

        void Update(void);
        bool pick_object;
        void SetSelectionBox(EG::Game::Object *_selection_box);
    private:
        EG::Game::Object *selection_box;
        bool object_selected;
        unsigned int selected_object_id;
        void PickObject(glm::vec2 mouse_position);
};

class GetVideoSettingsListener : public EG::GUI::WebResourceResponse{
    public:
        virtual std::string Call(std::map<std::string, std::string> args);
        EG::Game::Scene *scene;
        EG::Graphics::Renderer *renderer;
};

class SetVideoSettingsListener : public EG::GUI::WebResourceResponse{
    public:
        virtual std::string Call(std::map<std::string, std::string> args);
        EG::Game::Scene *scene;
        EG::Graphics::Renderer *renderer;
};

class ReadObjectsListener : public EG::GUI::WebResourceResponse{
    public:
        virtual std::string Call(std::map<std::string, std::string> args);
        EG::Game::Scene *scene;
};

class SaveMaterialListener : public EG::GUI::WebResourceResponse{
    public:
        virtual std::string Call(std::map<std::string, std::string> args);
        EG::Game::Scene *scene;
};

class FileBrowserListener : public EG::GUI::WebResourceResponse {
    public:
        virtual std::string Call(std::map<std::string, std::string> args);
};

class BodyClickListener : public EG::GUI::WebResourceResponse {
    public:
        virtual std::string Call(std::map<std::string, std::string> args);
        EG::Game::Game *game;
};

class LoadModelEventListener : public EG::GUI::WebResourceResponse{
    public:
        virtual std::string Call(std::map<std::string, std::string> args);
        bool model_loaded;
        EG::Game::Scene *scene;
        EG::Media::ModelLoader *model;
        EG::Game::Object *model_object;
};

#endif
