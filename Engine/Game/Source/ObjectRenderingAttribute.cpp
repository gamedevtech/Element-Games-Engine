#include "../ObjectRenderingAttribute.h"

namespace EG{
    namespace Game{
        ObjectAttributeRenderingMesh::ObjectAttributeRenderingMesh(std::string _mesh_id, EG::Graphics::RenderingMaterial *_material){
            type = EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_RENDERING_MESH;
            mesh_id = _mesh_id;
            material = _material;
        }
        ObjectAttributeRenderingMesh::~ObjectAttributeRenderingMesh(void){
            delete material;
        }

        void ObjectAttributeRenderingMesh::SetMeshId(std::string _mesh_id){
            mesh_id = _mesh_id;
        }
        void ObjectAttributeRenderingMesh::SetMaterial(EG::Graphics::RenderingMaterial *_material){
            material = _material;
        }
        std::string ObjectAttributeRenderingMesh::GetMeshId(void){
            return mesh_id;
        }
        EG::Graphics::RenderingMaterial *ObjectAttributeRenderingMesh::GetMaterial(void){
            return material;
        }

	ObjectAttributeRenderingCamera::ObjectAttributeRenderingCamera(EG::Graphics::Camera *_camera){
		type = EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_RENDERING_CAMERA;
		camera = _camera;
	}
	ObjectAttributeRenderingCamera::~ObjectAttributeRenderingCamera(void){
		delete camera;
	}
	void ObjectAttributeRenderingCamera::SetCamera(EG::Graphics::Camera *_camera){
		camera = _camera;
	}
	EG::Graphics::Camera *ObjectAttributeRenderingCamera::GetCamera(void){
		return camera;
	}
    }
}
