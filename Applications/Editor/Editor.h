#ifndef MODEL_CONVERTER_H
#define MODEL_CONVERTER_H

#include "../../../Engine/Media/ModelLoader.h"
#include "../../../Engine/Game/Object.h"
#include "../../../Engine/Game/Game.h"

class Sandbox : public EG::Game::Game{
    public:
        Sandbox(EG::Utility::Window *_window, EG::Game::Scene *_scene);
        ~Sandbox(void);

        void Update(void);
    private:
        //
};

#endif
