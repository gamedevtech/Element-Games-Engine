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
    private:
        //
};

#endif
