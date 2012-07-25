#include "../Scene.h"

namespace EG{
    namespace Game{
        Scene::Scene(void){
            objects = new EG::Game::ObjectManager();
            meshes = new EG::Graphics::MeshManager();
            textures = new EG::Graphics::TextureManager();
            current_camera = NULL;

            // Default Data
            textures->AddTexture("test_decal", new EG::Graphics::Texture("Assets/Textures/test.png"));
            textures->AddTexture("default_decal", new EG::Graphics::Texture("Assets/Textures/default_decal.png"));
            textures->AddTexture("default_normal", new EG::Graphics::Texture("Assets/Textures/default_normal.png"));
            textures->AddTexture("default_height", new EG::Graphics::Texture("Assets/Textures/default_height.png"));
            textures->AddTexture("default_specular", new EG::Graphics::Texture("Assets/Textures/default_specular.png"));
            textures->AddTexture("eglogo", new EG::Graphics::Texture("Assets/Textures/eglogo.png"));
            textures->AddCubeMap("default_decal_cube_map", new EG::Graphics::CubeMap("Assets/Textures/default_decal.png", "Assets/Textures/default_decal.png", "Assets/Textures/default_decal.png", "Assets/Textures/default_decal.png", "Assets/Textures/default_decal.png", "Assets/Textures/default_decal.png"));
            textures->AddCubeMap("default_normal_cube_map", new EG::Graphics::CubeMap("Assets/Textures/default_normal.png", "Assets/Textures/default_normal.png", "Assets/Textures/default_normal.png", "Assets/Textures/default_normal.png", "Assets/Textures/default_normal.png", "Assets/Textures/default_normal.png"));
            textures->AddCubeMap("default_height_cube_map", new EG::Graphics::CubeMap("Assets/Textures/default_height.png", "Assets/Textures/default_height.png", "Assets/Textures/default_height.png", "Assets/Textures/default_height.png", "Assets/Textures/default_height.png", "Assets/Textures/default_height.png"));
        }

        Scene::~Scene(void){
            delete objects;
        }

        EG::Game::ObjectManager *Scene::GetObjectManager(void){
            return objects;
        }

        EG::Graphics::MeshManager *Scene::GetMeshManager(void){
            return meshes;
        }

        EG::Graphics::TextureManager *Scene::GetTextureManager(void){
            return textures;
        }

        void Scene::SetCurrentCamera(EG::Graphics::Camera *camera){
            current_camera = camera;
        }

        EG::Graphics::Camera *Scene::GetCurrentCamera(void){
            return current_camera;
        }

        void Scene::SetConsole(EG::Utility::Console *_console) {
            console = _console;
        }

        EG::Utility::Console *Scene::GetConsole(void) {
            return console;
        }
    }
}
