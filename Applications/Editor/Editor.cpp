#include "Editor.h"
#include "../../Engine/Game/ObjectBasicAttribute.h"
#include "../../Engine/Game/ObjectControlAttribute.h"
#include "../../Engine/Game/ObjectEmissionAttribute.h"
#include "../../Engine/Game/ObjectRenderingAttribute.h"
#include "../../Engine/Graphics/Renderer.h"
#include "../../Engine/Graphics/RendererDeferred.h"
#include "../../Engine/Utility/StringMethods.h"
#include "../../Engine/Utility/JSON.h"

#include <boost/filesystem.hpp>
#include <stdio.h>
#include <stdlib.h>

std::string GetVideoSettingsListener::Call(std::map<std::string, std::string> args) {
    std::string out;
    out += "{";

    std::string bloom_enabled = "false";
    if (renderer->GetBloomEnabled()) {
        bloom_enabled = "true";
    }
    out += "\"bloom\": " + bloom_enabled;

    std::string ssao_enabled = "false";
    if (renderer->GetSSAOEnabled()) {
        ssao_enabled = "true";
    }
    out += " ,\"ssao\": " + ssao_enabled;

    std::string normal_mapping_enabled = "false";
    if (renderer->GetNormalMappingEnabled()) {
        normal_mapping_enabled = "true";
    }
    out += " ,\"normal_mapping\": " + normal_mapping_enabled;

    std::string shadow_mapping_enabled = "false";
    if (renderer->GetShadowsEnabled()) {
        shadow_mapping_enabled = "true";
    }
    out += " ,\"shadows\": " + shadow_mapping_enabled;

    std::string dof_enabled = "false";
    if (renderer->GetDepthOfFieldEnabled()) {
        dof_enabled = "true";
    }
    out += " ,\"dof\": " + dof_enabled;

    out += "}";
    std::cout << "Bin Response: " << out << std::endl;
    return out.c_str();
}

std::string SetVideoSettingsListener::Call(std::map<std::string, std::string> args) {
    std::cout << "Dongle" << std::endl;
    if (args["bloom"] == "true") {
        renderer->SetBloomEnabled(true);
    } else {
        renderer->SetBloomEnabled(false);
    }
    if (args["normal_mapping"] == "true") {
        renderer->SetNormalMappingEnabled(true);
    } else {
        renderer->SetNormalMappingEnabled(false);
    }
    if (args["ssao"] == "true") {
        renderer->SetSSAOEnabled(true);
    } else {
        renderer->SetSSAOEnabled(false);
    }
    if (args["shadows"] == "true") {
        renderer->SetShadowsEnabled(true);
    } else {
        renderer->SetShadowsEnabled(false);
    }
    if (args["dof"] == "true") {
        renderer->SetDepthOfFieldEnabled(true);
    } else {
        renderer->SetDepthOfFieldEnabled(false);
    }
    return "{\"status\": true}";
}

std::string FileBrowserListener::Call(std::map< std::string, std::string > args) {
    // Make config! Go with scene!
    std::string path = boost::filesystem::current_path().c_str() + EG::Utility::StringMethods::SearchAndReplace(args["path"], "%2F", "/");
    std::cout << "File Browser @: " << path << std::endl;
    std::string out = "[";
    boost::filesystem::directory_iterator end_itr; // Default ctor yields past-the-end
    bool first = true;
    unsigned int id = 0;
    for (boost::filesystem::directory_iterator i(path); i != end_itr; ++i, id += 1) {
        if (first) {
            first = false;
        } else {
            out += ",";
        }

        std::string name = i->path().c_str();
        unsigned int index = name.find_last_of('/');
        name = name.substr(index + 1);

        out += "{\"path\": \"";
        out += name;
        out += "\", \"type\": \"";
        if (boost::filesystem::is_regular_file(i->status())) {
            out += "file";
        }
        if (boost::filesystem::is_directory(i->status())) {
            out += "dir";
        }
        out += "\", \"id\": ";
        char id_str[6];
        sprintf(id_str, "%d", id);
        out += id_str;
        out += "}";
    }
    out += "]";
    std::cout << out << std::endl;
    return out.c_str();
}

std::string ReadObjectsListener::Call(std::map<std::string, std::string> args) {
    EG::Utility::UnsignedIntDictionary<EG::Game::Object *> *objects = scene->GetObjectManager()->GetObjects();
    EG::Utility::UnsignedIntDictionaryKeysIterator iter = objects->GetKeysBegin();

    std::stringstream out;
    out << "[";
    bool first = true;
    while (iter != objects->GetKeysEnd()) {
        EG::Game::Object *object = scene->GetObjectManager()->GetObjects()->Get((*iter));
        if (first) {
            first = false;
        } else {
            out << ",";
        }
        out << "{\"id\": \"" << object->GetObjectName() << "\", ";
        out << "\"record_id\": " << object->GetObjectId();

        if (object->HasAttributesOfType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_RENDERING_MESH)) {
            out << ", \"materials\": [";
            std::vector<EG::Game::ObjectAttribute *> *material_attributes = object->GetAttributesByType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_RENDERING_MESH);
            std::vector<EG::Game::ObjectAttribute *>::iterator material_iter = material_attributes->begin();
            while (material_iter != material_attributes->end()) {
                EG::Game::ObjectAttributeRenderingMesh *material_attribute = static_cast<EG::Game::ObjectAttributeRenderingMesh *>(*material_iter);
                EG::Graphics::RenderingMaterial *material = material_attribute->GetMaterial();
                // Mesh Id
                out << "{\"mesh_id\": \"" << material_attribute->GetMeshId() << "\"";

                // Color
                glm::vec4 c = material->GetColor();
                out << ",\"color\": [" << c.x << ", " << c.y << ", " << c.z << ", " << c.w << "]";

                // Specular
                out << ",\"specular\": " << material->GetSpecular();

                // Fin
                out << "}";
                ++material_iter;
            }
            out << "]";
        }

        out << "}";
        ++iter;
    }
    out << "]";
    std::cout << out.str() << std::endl;
    return out.str().c_str();
}

std::string LoadModelEventListener::Call(std::map<std::string, std::string> args){
    std::string filename = args["path"].substr(3);
    filename = EG::Utility::StringMethods::SearchAndReplace(filename, "%2F", "/");
    model = new EG::Media::ModelLoader(scene);
    model_loaded = model->Load(filename);
    std::cout << "Loading Model: " << filename << std::endl;
    if (model_loaded){
        std::cout << "Model Loaded!" << std::endl;
        model_object = new EG::Game::Object(filename);

        /*SetLitCallback *litcallback = new SetLitCallback();
        litcallback->object = model_object;
        gui->AddResponseHandler("set_lit", litcallback);

        SetShadowsCallback *shadowscallback = new SetShadowsCallback();
        shadowscallback->object = model_object;
        gui->AddResponseHandler("set_shadows", shadowscallback);

        SetDecalCallback *decalcallback = new SetDecalCallback();
        decalcallback->object = model_object;
        decalcallback->scene = scene;
        gui->AddResponseHandler("set_decal", decalcallback);

        SetNormalCallback *normalcallback = new SetNormalCallback();
        normalcallback->object = model_object;
        normalcallback->scene = scene;
        gui->AddResponseHandler("set_normal", normalcallback);

        SetHeightCallback *heightcallback = new SetHeightCallback();
        heightcallback->object = model_object;
        heightcallback->scene = scene;
        gui->AddResponseHandler("set_height", heightcallback);

        SetSpecularCallback *specularcallback = new SetSpecularCallback();
        specularcallback->object = model_object;
        specularcallback->scene = scene;
        gui->AddResponseHandler("set_specular", specularcallback);

        SaveCallback *savecallback = new SaveCallback();
        savecallback->object = model_object;
        savecallback->scene = scene;
        gui->AddResponseHandler("save_model", savecallback);*/

        // TODO: Loop through the meshes and perhaps combine them into one, and get the transforms right, if not, then just make transforms available as an option on a mesh as well!
        model_object->AddAttribute(new EG::Game::ObjectAttributeBasicTransformation(glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.0f, 0.0f)), glm::vec3(0.02f, 0.02f, 0.02f))));
        for (unsigned int i = 0; i < model->GetMeshMaterialPairCount(); i++) {
            std::cout << "Mesh Material Index: " << i << std::endl;
            model->GetMaterial(i)->SetLit(true);
            model->GetMaterial(i)->SetCastsShadows(true);
            EG::Game::ObjectAttributeRenderingMesh *rm = new EG::Game::ObjectAttributeRenderingMesh(model->GetMesh(i), model->GetMaterial(i));
            model_object->AddAttribute(rm);
        }
        if (model->HasAnimations()) {
            EG::Dynamics::AnimationState *animation_state = new EG::Dynamics::AnimationState(model->GetAnimations());
            EG::Game::ObjectAttributeControlAnimationState *attrib = new EG::Game::ObjectAttributeControlAnimationState(animation_state);
            model_object->AddAttribute(attrib);
        }
        scene->GetObjectManager()->AddObject(model_object);

        std::string out = "{\"status\": true";

        if (model->GetMaterial(0)->GetLit()){
            out += ", \"lit\": true";
        } else {
            out += ", \"lit\": false";
        }
        if (model->GetMaterial(0)->GetCastsShadows()){
            out += ", \"shadows\": true";
        } else {
            out += ", \"shadows\": false";
        }
        if (model->GetMaterial(0)->HasTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_DECAL)){
            std::string file_path = scene->GetTextureManager()->GetTexture(model->GetMaterial(0)->GetTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_DECAL))->GetFilePath();
            out += ", \"decal\": \"" + file_path + "\"";
        } else {
            out += ", \"decal\": \"\"";
        }
        if (model->GetMaterial(0)->HasTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_NORMAL)){
            std::string file_path = scene->GetTextureManager()->GetTexture(model->GetMaterial(0)->GetTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_NORMAL))->GetFilePath();
            out += ", \"normal\": \"" + file_path + "\"";
        } else {
            out += ", \"normal\": \"\"";
        }
        if (model->GetMaterial(0)->HasTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_HEIGHT)){
            std::string file_path = scene->GetTextureManager()->GetTexture(model->GetMaterial(0)->GetTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_HEIGHT))->GetFilePath();
            out += ", \"height\": \"" + file_path + "\"";
        } else {
            out += ", \"height\": \"\"";
        }
        if (model->GetMaterial(0)->HasTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_SPECULAR)){
            std::string file_path = scene->GetTextureManager()->GetTexture(model->GetMaterial(0)->GetTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_SPECULAR))->GetFilePath();
            out += ", \"specular\": \"" + file_path + "\"}";
        } else {
            out += ", \"specular\": \"\"}";
        }
        return out;
    }else{
        return "{\"status\": false}";
    }
}

Editor::Editor(EG::Utility::Window *_window, EG::Game::Scene *_scene) : Game(_window, _scene){
    gui->Initialize("Assets/GUIs/Editor", "index.html");

    GetVideoSettingsListener *get_video_listener = new GetVideoSettingsListener();
    get_video_listener->scene = scene;
    get_video_listener->renderer = renderer;
    gui->AddResponseHandler("get_video_settings", get_video_listener);

    SetVideoSettingsListener *set_video_listener = new SetVideoSettingsListener();
    set_video_listener->scene = scene;
    set_video_listener->renderer = renderer;
    gui->AddResponseHandler("set_video_settings", set_video_listener);

    FileBrowserListener *file_system_ = new FileBrowserListener();
    gui->AddResponseHandler("list_directory", file_system_);

    LoadModelEventListener *listener = new LoadModelEventListener();
    listener->scene = scene;
    listener->model_loaded = false;
    gui->AddResponseHandler("load_model", listener);

    ReadObjectsListener *read_objects_listener = new ReadObjectsListener();
    read_objects_listener->scene = scene;
    gui->AddResponseHandler("read_objects", read_objects_listener);
}

Editor::~Editor(void){
    //
}

void Editor::PickObject(glm::vec2 mouse_position) {
    EG::Graphics::Camera *c = scene->GetCurrentCamera();
    glm::vec3 begin = c->GetPosition();
    glm::vec3 dir = EG::Math::Utility::ProjectClick(glm::vec2(graphics->GetViewportWidth(), graphics->GetViewportHeight()),
                                                    mouse_position, scene->GetCurrentCamera()->GetNearFar().y,
                                                    c->GetPosition(), c->GetViewMatrix(), c->GetProjectionMatrix());

    EG::Graphics::MeshManager *meshes = scene->GetMeshManager();
    EG::Utility::UnsignedIntDictionary<EG::Game::Object *> *objects = scene->GetObjectManager()->GetObjects();
    EG::Utility::UnsignedIntDictionaryKeysIterator iter = objects->GetKeysBegin();
    bool object_picked = false;
    while (iter != objects->GetKeysEnd()) {
        unsigned int object_id = (*iter);
        EG::Game::Object *object = objects->Get(object_id);
        glm::mat4 trans = glm::mat4(1.0f);
        if (object->HasAttributesOfType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_BASIC_TRANSFORMATION)) {
            std::vector<EG::Game::ObjectAttribute *> *tattrs = object->GetAttributesByType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_BASIC_TRANSFORMATION);
            EG::Game::ObjectAttribute *tattr = (*tattrs)[0];
            trans = (static_cast<EG::Game::ObjectAttributeBasicTransformation *>(&(tattr[0])))->GetTransformation();
        }
        if (object->HasAttributesOfType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_RENDERING_MESH)) {
            std::vector<EG::Game::ObjectAttribute *> *attributes = object->GetAttributesByType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_RENDERING_MESH);
            std::vector<EG::Game::ObjectAttribute *>::iterator attr_iter = attributes->begin();
            while (attr_iter != attributes->end()) {
                EG::Game::ObjectAttributeRenderingMesh *mattr = (static_cast<EG::Game::ObjectAttributeRenderingMesh *>(*attr_iter));
                EG::Graphics::Mesh *mesh = meshes->Get(mattr->GetMeshId());
                glm::vec3 *box = mesh->GetBoundingBox();
                glm::mat4 mtrans = trans * mattr->GetOffset();
                object_picked = EG::Math::Utility::RayAABBTest(begin, dir, box[0], box[1], mtrans);
                if (object_picked) {
                    break;
                }
                ++attr_iter;
            }
        }
        if (object_picked) {
            break;
        }
        ++iter;
    }
    if (object_picked) {
        EG::Game::Object *object = objects->Get((*iter));
        std::stringstream out;
        out << "Object Picked: " << object->GetObjectName();
        console->Print(out.str());
        std::stringstream script;
        script << "main_view.tools.select_object(\"" << object->GetObjectName() << "\");";
        console->Print(script.str());
        gui->ExecuteScript(script.str().c_str());
    } else {
        console->Print("No Object Picked");
    }
}

void Editor::Update(void){
    float movement_speed = time->GetFrameTime() * 2.0f;
    if (input->IsKeyDown(EG::Input::v)) {
        movement_speed /= 100.0f;
    }
    if (input->IsMouseToggled(EG::Input::mouse_left)) {
        PickObject(input->GetMousePosition());
    }
    if (input->IsMouseDown(EG::Input::mouse_right)){
        scene->GetCurrentCamera()->RotateByMouse(input->GetMouseDelta());
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

    physics->Update(time->GetFrameTime());
}
