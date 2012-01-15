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
    }
}
