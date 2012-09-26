#ifndef SPACE_SIM_H
#define SPACE_SIM_H

#include "../../../Engine/Game/Object.h"
#include "../../../Engine/Game/ObjectBasicAttribute.h"
#include "../../../Engine/Game/ObjectControlAttribute.h"
#include "../../../Engine/Game/ObjectEmissionAttribute.h"
#include "../../../Engine/Game/ObjectRenderingAttribute.h"
#include "../../../Engine/Game/Game.h"
#include "../../../Engine/Graphics/Renderer.h"
#include "../../../Engine/Graphics/RendererDeferred.h"
#include "../../../Engine/Utility/JSON.h"
#include "../../../Engine/Graphics/MeshGenerator.h"

class SpaceSim : public EG::Game::Game{
    public:
        SpaceSim(EG::Utility::Window *_window, EG::Game::Scene *_scene);
        ~SpaceSim(void);

        void Update(void);
    private:
        //
};

#endif
