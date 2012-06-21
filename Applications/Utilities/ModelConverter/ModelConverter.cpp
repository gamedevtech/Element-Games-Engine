#include "ModelConverter.h"

#include "../../../Engine/Graphics/RendererMultipass.h"
#include "../../../Engine/Graphics/RendererDeferred.h"
#include "../../../Engine/Game/ObjectBasicAttribute.h"
#include "../../../Engine/Game/ObjectControlAttribute.h"
#include "../../../Engine/Game/ObjectEmissionAttribute.h"
#include "../../../Engine/Game/ObjectRenderingAttribute.h"
#include "../../../Engine/Utility/StringMethods.h"
#include "../../../Engine/Media/ObjectWriter.h"
#include <complex>

std::string LoadModelEventListener::Call(std::map<std::string, std::string> args){
    std::string filename = args["filename"];
    filename = EG::Utility::StringMethods::SearchAndReplace(filename, "%2F", "/");
    model = new EG::Media::ModelLoader(scene);
    model_loaded = model->Load(filename);
    std::cout << "Model Loaded!!!" << std::endl;
    if (model_loaded){
        model_object = new EG::Game::Object(filename);

        SetLitCallback *litcallback = new SetLitCallback();
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
        gui->AddResponseHandler("save_model", savecallback);

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

std::string SetLitCallback::Call(std::map<std::string, std::string> args){
    if (object){
        std::vector<EG::Game::ObjectAttribute *> *mesh_attributes = object->GetAttributesByType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_RENDERING_MESH);
        std::vector<EG::Game::ObjectAttribute *>::iterator mesh_attribute_iterator = mesh_attributes->begin();
        while (mesh_attribute_iterator != mesh_attributes->end()){
            EG::Game::ObjectAttributeRenderingMesh *mesh_attribute = static_cast<EG::Game::ObjectAttributeRenderingMesh *>(*mesh_attribute_iterator);
            EG::Graphics::RenderingMaterial *material = mesh_attribute->GetMaterial();

            bool lit_status = material->GetLit();
            material->SetLit(!lit_status);
            ++mesh_attribute_iterator;
        }
        return "{\"status\": true}";
    }
    return "{\"status\": false}";
}

std::string SetShadowsCallback::Call(std::map<std::string, std::string> args){
    if (object){
        std::vector<EG::Game::ObjectAttribute *> *mesh_attributes = object->GetAttributesByType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_RENDERING_MESH);
        std::vector<EG::Game::ObjectAttribute *>::iterator mesh_attribute_iterator = mesh_attributes->begin();
        while (mesh_attribute_iterator != mesh_attributes->end()){
            EG::Game::ObjectAttributeRenderingMesh *mesh_attribute = static_cast<EG::Game::ObjectAttributeRenderingMesh *>(*mesh_attribute_iterator);
            EG::Graphics::RenderingMaterial *material = mesh_attribute->GetMaterial();

            bool shadows_status = material->GetCastsShadows();
            material->SetCastsShadows(!shadows_status);
            ++mesh_attribute_iterator;
        }
        return "{\"status\": true}";
    }
    return "{\"status\": false}";
}

std::string SetDecalCallback::Call(std::map<std::string, std::string> args){
    if (object){
        std::string new_texture_path = args["decal"];
        new_texture_path = EG::Utility::StringMethods::SearchAndReplace(new_texture_path, "%2F", "/");
                new_texture_path = EG::Utility::StringMethods::RemoveSpecialCharactersFromPathString(new_texture_path);

            if (!(scene->GetTextureManager()->HasTexture(new_texture_path))){
                    EG::Graphics::Texture *texture = new EG::Graphics::Texture(new_texture_path);
                    scene->GetTextureManager()->AddTexture(new_texture_path, texture);
            }

            // Meshes
            std::vector<EG::Game::ObjectAttribute *> *mesh_attributes = object->GetAttributesByType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_RENDERING_MESH);
            std::vector<EG::Game::ObjectAttribute *>::iterator mesh_attribute_iterator = mesh_attributes->begin();
            while (mesh_attribute_iterator != mesh_attributes->end()){
                    EG::Game::ObjectAttributeRenderingMesh *mesh_attribute = static_cast<EG::Game::ObjectAttributeRenderingMesh *>(*mesh_attribute_iterator);
                    EG::Graphics::RenderingMaterial *material = mesh_attribute->GetMaterial();

                    material->SetTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_DECAL, new_texture_path);
                    ++mesh_attribute_iterator;
            }
        return "{\"status\": true}";
    }
    return "{\"status\": false}";
}

std::string SetNormalCallback::Call(std::map<std::string, std::string> args){
        if (object){
                std::string new_texture_path = args["normal"];
                new_texture_path = EG::Utility::StringMethods::SearchAndReplace(new_texture_path, "%2F", "/");
                new_texture_path = EG::Utility::StringMethods::RemoveSpecialCharactersFromPathString(new_texture_path);
                std::cout << "new_texture_path: " << new_texture_path << std::endl;

                if (!(scene->GetTextureManager()->HasTexture(new_texture_path))){
                        EG::Graphics::Texture *texture = new EG::Graphics::Texture(new_texture_path);
                        scene->GetTextureManager()->AddTexture(new_texture_path, texture);
                }

                // Meshes
                std::vector<EG::Game::ObjectAttribute *> *mesh_attributes = object->GetAttributesByType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_RENDERING_MESH);
                std::vector<EG::Game::ObjectAttribute *>::iterator mesh_attribute_iterator = mesh_attributes->begin();
                while (mesh_attribute_iterator != mesh_attributes->end()){
                        EG::Game::ObjectAttributeRenderingMesh *mesh_attribute = static_cast<EG::Game::ObjectAttributeRenderingMesh *>(*mesh_attribute_iterator);
                        EG::Graphics::RenderingMaterial *material = mesh_attribute->GetMaterial();

                        material->SetTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_NORMAL, new_texture_path);
                        ++mesh_attribute_iterator;
                }
                return "{\"status\": true}";
        }
        return "{\"status\": false}";
}

std::string SetHeightCallback::Call(std::map<std::string, std::string> args){
        if (object){
                std::string new_texture_path = args["height"];
                new_texture_path = EG::Utility::StringMethods::SearchAndReplace(new_texture_path, "%2F", "/");
                new_texture_path = EG::Utility::StringMethods::RemoveSpecialCharactersFromPathString(new_texture_path);

                if (!(scene->GetTextureManager()->HasTexture(new_texture_path))){
                        EG::Graphics::Texture *texture = new EG::Graphics::Texture(new_texture_path);
                        scene->GetTextureManager()->AddTexture(new_texture_path, texture);
                }

                // Meshes
                std::vector<EG::Game::ObjectAttribute *> *mesh_attributes = object->GetAttributesByType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_RENDERING_MESH);
                std::vector<EG::Game::ObjectAttribute *>::iterator mesh_attribute_iterator = mesh_attributes->begin();
                while (mesh_attribute_iterator != mesh_attributes->end()){
                        EG::Game::ObjectAttributeRenderingMesh *mesh_attribute = static_cast<EG::Game::ObjectAttributeRenderingMesh *>(*mesh_attribute_iterator);
                        EG::Graphics::RenderingMaterial *material = mesh_attribute->GetMaterial();

                        material->SetTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_HEIGHT, new_texture_path);
                        ++mesh_attribute_iterator;
                }
                return "{\"status\": true}";
        }
        return "{\"status\": false}";
}

std::string SetSpecularCallback::Call(std::map<std::string, std::string> args){
        if (object){
                std::string new_texture_path = args["specular"];
                new_texture_path = EG::Utility::StringMethods::SearchAndReplace(new_texture_path, "%2F", "/");
        new_texture_path = EG::Utility::StringMethods::RemoveSpecialCharactersFromPathString(new_texture_path);

                if (!(scene->GetTextureManager()->HasTexture(new_texture_path))){
                        EG::Graphics::Texture *texture = new EG::Graphics::Texture(new_texture_path);
                        scene->GetTextureManager()->AddTexture(new_texture_path, texture);
                }

                // Meshes
                std::vector<EG::Game::ObjectAttribute *> *mesh_attributes = object->GetAttributesByType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_RENDERING_MESH);
                std::vector<EG::Game::ObjectAttribute *>::iterator mesh_attribute_iterator = mesh_attributes->begin();
                while (mesh_attribute_iterator != mesh_attributes->end()){
                        EG::Game::ObjectAttributeRenderingMesh *mesh_attribute = static_cast<EG::Game::ObjectAttributeRenderingMesh *>(*mesh_attribute_iterator);
                        EG::Graphics::RenderingMaterial *material = mesh_attribute->GetMaterial();

                        material->SetTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_SPECULAR, new_texture_path);
                        ++mesh_attribute_iterator;
                }
                return "{\"status\": true}";
        }
        return "{\"status\": false}";
}

std::string SaveCallback::Call(std::map<std::string, std::string> args){
    if (object && scene){
        std::string file_path = args["output_path"];
        file_path = EG::Utility::StringMethods::SearchAndReplace(file_path, "%2F", "/");
            file_path = EG::Utility::StringMethods::RemoveSpecialCharactersFromPathString(file_path);

            EG::Media::ObjectWriter *writer = new EG::Media::ObjectWriter(object, scene, file_path, "Assets/Textures/", "Assets/Models/");
            writer->Write(file_path);
        return "{\"status\": true}";
    }
    return "{\"status\": false}";
}

ModelConverter::ModelConverter(EG::Utility::Window *_window, EG::Game::Scene *_scene) : Game(_window, _scene){
    gui->Initialize("Assets/GUIs/ModelConverter", "index.html");

    LoadModelEventListener *listener = new LoadModelEventListener();
    listener->scene = scene;
    listener->model_loaded = false;
    gui->AddResponseHandler("load_model", listener);
}

ModelConverter::~ModelConverter(void){
    //
}

void ModelConverter::Update(void){
    float movement_speed = time->GetFrameTime() * 2.0f;
    if (input->IsMouseDown(EG::Input::mouse_right)){
        scene->GetCurrentCamera()->RotateByMouse(input->GetMouseDelta() * 2.0f);
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
}

