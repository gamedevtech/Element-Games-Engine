#include "../Scene.h"

namespace EG{
    namespace Game{
        Scene::Scene(void){
            objects = new EG::Game::ObjectManager();
            meshes = new EG::Graphics::MeshManager();
            textures = new EG::Graphics::TextureManager();

            textures->AddTexture("test_decal", new EG::Graphics::Texture("Assets/Textures/test.png"));
            textures->AddTexture("default_decal", new EG::Graphics::Texture("Assets/Textures/default_decal.png"));
            textures->AddTexture("default_normal", new EG::Graphics::Texture("Assets/Textures/default_normal.png"));
            textures->AddTexture("default_height", new EG::Graphics::Texture("Assets/Textures/default_height.png"));
            textures->AddCubeMap("default_decal_cube_map", new EG::Graphics::CubeMap("Assets/default_decal/default_decal.png", "Assets/Textures/default_decal.png", "Assets/Textures/default_decal.png", "Assets/Textures/default_normal.png", "Assets/Textures/default_decal.png", "Assets/Textures/default_decal.png"));
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
    }
}
