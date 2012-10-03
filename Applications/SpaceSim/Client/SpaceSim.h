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
#include "../Common/SpaceSimNetworkActionTypes.h"

class SpaceSim : public EG::Game::Game{
    public:
        SpaceSim(EG::Utility::Window *_window, EG::Game::Scene *_scene);
        ~SpaceSim(void);

        void Update(void);
    private:
        void NetworkUpdates(float frame_time);
        void ProcessNetworkPacket(float frame_time, EG::Network::Packet *packet);

        void InputUpdates(float frame_time);
        void PhysicsUpdates(float frame_time);

        bool polling_lan;
};

class GetLANServersListener : public EG::GUI::WebResourceResponse{
    public:
        virtual std::string Call(std::map<std::string, std::string> args);
        SpaceSim *game;
};

class GetLANServerResultsListener : public EG::GUI::WebResourceResponse{
    public:
        virtual std::string Call(std::map<std::string, std::string> args);
        SpaceSim *game;
};

class QuitGameListener : public EG::GUI::WebResourceResponse{
    public:
        virtual std::string Call(std::map<std::string, std::string> args);
        SpaceSim *game;
};

#endif
