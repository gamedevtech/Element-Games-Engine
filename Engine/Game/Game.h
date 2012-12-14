#ifndef EG_GAME_GAME_H
#define EG_GAME_GAME_H

#include "Scene.h"
#include "../Graphics/Renderer.h"
#include "../Utility/Window.h"
#include "../Utility/Time.h"
#include "../Utility/GUI.h"
#include "../Utility/Console.h"
#include "../Dynamics/Physics.h"
#include "../Networking/Networking.h"

namespace EG{
    namespace Game{
        class Game{
            public:
                enum RenderingMethod{
                    RENDERER_SIMPLE, RENDERER_MULTIPASS, RENDERER_DEFERRED
                };

                Game(EG::Utility::Window *_window, EG::Game::Scene *_scene);
                ~Game(void);

                virtual void Update(void);
                void PreUpdates(void);
                void PostUpdates(void);
                void Render(void);

                EG::Game::Scene *GetScene(void);
                EG::Utility::Window *GetWindow(void);
                EG::Graphics::Renderer *GetRenderer(void);
                EG::Input::Input *GetInput(void);
                EG::Utility::Time *GetTime(void);
                EG::Network::Network *GetNetwork(void);
            protected:
                EG::Utility::Window *window;
                EG::Graphics::Renderer *renderer;
                EG::Game::Scene *scene;
                EG::Input::Input *input;
                EG::Utility::Time *time;
                EG::Dynamics::Physics *physics;
                EG::Network::Network *network;
                RenderingMethod rendering_method;
        };
    }
}

#endif
