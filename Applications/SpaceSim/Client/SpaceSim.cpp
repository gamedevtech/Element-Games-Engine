#include "SpaceSim.h"

SpaceSim::SpaceSim(EG::Utility::Window *_window, EG::Game::Scene *_scene) : Game(_window, _scene) {
    //network->Connect("127.0.0.1", "admin", "admin");
    gui->Initialize("Assets/GUIs/SpaceSimClient", "index.html");
    polling_lan = false;

    GetLANServersListener *lan_server_listener = new GetLANServersListener();
    lan_server_listener->game = this;
    gui->AddResponseHandler("lan_servers", lan_server_listener);

    GetLANServerResultsListener *lan_server_results_listener = new GetLANServerResultsListener();
    lan_server_results_listener->game = this;
    gui->AddResponseHandler("lan_results", lan_server_results_listener);

    QuitGameListener *quit_game_listener = new QuitGameListener();
    quit_game_listener->game = this;
    gui->AddResponseHandler("quit", quit_game_listener);

    ConnectServerListener *connect_server_listener = new ConnectServerListener();
    connect_server_listener->game = this;
    gui->AddResponseHandler("connect", connect_server_listener);
}

SpaceSim::~SpaceSim(void) {
    //
}

EG::Game::Object *SpaceSim::CreateNewPlayerObject(glm::vec3 pos) {
	EG::Game::Object *out = new EG::Game::Object();
	EG::Graphics::RenderingMaterial *material = new EG::Graphics::RenderingMaterial();
	material->SetTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_DECAL, "concrete_decal");
	material->SetTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_NORMAL, "concrete_normal");
	EG::Game::ObjectAttributeRenderingMesh *mesh_attr = new EG::Game::ObjectAttributeRenderingMesh("sphere", material);
	out->AddAttribute(mesh_attr);
	glm::mat4 trans = glm::translate(pos);
	out->AddAttribute(new EG::Game::ObjectAttributeBasicTransformation(trans));
	scene->GetObjectManager()->AddObject(out);
	return out;
}

void SpaceSim::UpdatePlayerObject(EG::Game::Object *obj, glm::vec3 pos) {
	EG::Game::ObjectAttributeBasicTransformation * trans = (static_cast<EG::Game::ObjectAttributeBasicTransformation *>((*(obj->GetAttributesByType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_BASIC_TRANSFORMATION)))[0]));
	trans->SetTransformation(glm::translate(pos));
}

void SpaceSim::ProcessNetworkPacket(float frame_time, EG::Network::Packet* packet) {
    sf::Packet *sfpacket = packet->GetPacket();
    unsigned int action_type_id;
    *(sfpacket) >> action_type_id;
    if (action_type_id == NETWORK_ACTION_MESSAGE_RELAY) {
        unsigned int client_id, from_client_id;
        std::string message;
        *(sfpacket) >> client_id >> from_client_id >> message;
        std::stringstream stream;
        stream << "Received Broadcast Message From " << client_id << ":" << from_client_id << " of " << message << std::endl;
        console->Print(stream.str());
    } else if (action_type_id == NETWORK_ACTION_MOVEMENT_BROADCAST_RELAY) {
		unsigned int server_client_id, player_client_id;
		glm::vec3 pos;
		*(sfpacket) >> server_client_id >> player_client_id >> pos.x >> pos.y >> pos.z;
		EG::Game::Object *player_object;
		if (players.count(player_client_id) < 1) {
			player_object = CreateNewPlayerObject(pos);
			players[player_client_id] = player_object;
		} else {
			UpdatePlayerObject(players[player_client_id], pos);
		}
	}
}

void SpaceSim::NetworkUpdates(float frame_time) {
    bool got_packet = false;
    EG::Network::Packet *packet = network->ReceivePacket(got_packet);
    while (got_packet) {
        ProcessNetworkPacket(frame_time, packet);
        packet = network->ReceivePacket(got_packet);
    }

    if (polling_lan) {
        if (!(network->IsPollingLAN())) {
            // Done Polling
        }
    }
}

void SpaceSim::InputUpdates(float frame_time) {
    float movement_speed = frame_time * 2.0f;
	bool moved = false;
    if (input->IsKeyDown(EG::Input::v)) {
        movement_speed /= 100.0f;
    }
    if (input->IsMouseDown(EG::Input::mouse_right)){
        scene->GetCurrentCamera()->RotateByMouse(input->GetMouseDelta());
    }
    if (input->IsKeyDown(EG::Input::q)){
        scene->GetCurrentCamera()->Rotate(glm::vec3(0.0f, 0.0f, -movement_speed));
		moved = true;
    }
    if (input->IsKeyDown(EG::Input::e)){
        scene->GetCurrentCamera()->Rotate(glm::vec3(0.0f, 0.0f, movement_speed));
		moved = true;
    }
    if (input->IsKeyDown(EG::Input::w)){
        scene->GetCurrentCamera()->Move(glm::vec3(0.0f, 0.0f, -movement_speed));
		moved = true;
    }
    if (input->IsKeyDown(EG::Input::s)){
        scene->GetCurrentCamera()->Move(glm::vec3(0.0f, 0.0f, movement_speed));
		moved = true;
    }
    if (input->IsKeyDown(EG::Input::a)){
        scene->GetCurrentCamera()->Move(glm::vec3(-movement_speed, 0.0f, 0.0f));
		moved = true;
    }
    if (input->IsKeyDown(EG::Input::d)){
        scene->GetCurrentCamera()->Move(glm::vec3(movement_speed, 0.0f, 0.0f));
		moved = true;
    }
    if (input->IsKeyDown(EG::Input::space)){
        scene->GetCurrentCamera()->Move(glm::vec3(0.0f, movement_speed, 0.0f));
		moved = true;
    }
    if (input->IsKeyDown(EG::Input::c)){
        scene->GetCurrentCamera()->Move(glm::vec3(0.0f, -movement_speed, 0.0f));
		moved = true;
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
	if (moved) {
		EG::Network::Packet *packet = new EG::Network::Packet();
		glm::vec3 p = scene->GetCurrentCamera()->GetPosition();
		*(packet->GetPacket()) << NETWORK_ACTION_MOVEMENT_BROADCAST << network->GetClientId() << p.x << p.y << p.z;
		network->SendPacket(packet, true);
	}
    if (input->IsKeyPressed(EG::Input::k)) {
        EG::Network::Packet *packet = new EG::Network::Packet();
        std::string message = "What's up server!";
        *(packet->GetPacket()) << NETWORK_ACTION_MESSAGE_BROADCAST << network->GetClientId() << message;
        network->SendPacket(packet);
        console->Print("Sending Broadcast Message");
    }
    if (input->IsKeyPressed(EG::Input::p)) {
        network->PollLAN();
        polling_lan = true;
    }
}

void SpaceSim::PhysicsUpdates(float frame_time) {
    physics->Update(frame_time);
}

void SpaceSim::Update(void) {
    float frame_time = time->GetFrameTime();
    NetworkUpdates(frame_time);
    InputUpdates(frame_time);
    PhysicsUpdates(frame_time);
}

std::string QuitGameListener::Call(std::map<std::string, std::string> args) {
    game->GetWindow()->Close();
    return "{\"status\": true}";
}

std::string GetLANServersListener::Call(std::map<std::string, std::string> args) {
    game->GetNetwork()->PollLAN();
    console->Print("Started Polling");
    return "{\"status\": true}";
}

std::string GetLANServerResultsListener::Call(std::map<std::string, std::string> args) {
    std::vector<sf::IpAddress> *ips = game->GetNetwork()->GetServerIpAddresses();
    std::vector<sf::IpAddress>::iterator ips_iter = ips->begin();
    std::string out = "[";
    while (ips_iter != ips->end()) {
        sf::IpAddress ip = (*ips_iter);
        out += "{\"id\": \"" + ip.toString() + "\"}";
        ++ips_iter;
    }
    out += "]";
    return out.c_str();
}

std::string ConnectServerListener::Call(std::map<std::string, std::string> args) {
    std::cout << "(" << args["ip"] << ")" << std::endl;
    game->GetNetwork()->Connect(args["ip"], "admin", "admin");
    return "{\"status\": true}";
}
