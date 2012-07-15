#ifndef EG_GAME_OBJECT_RENDERING_ATTRIBUTE_H
#define EG_GAME_OBJECT_RENDERING_ATTRIBUTE_H

#include <string>
#include "ObjectAttribute.h"
#include "../Graphics/RenderingMaterial.h"
#include "../Graphics/Camera.h"

namespace EG{
	namespace Game{
		class ObjectAttributeRenderingMesh : public ObjectAttribute{
			public:
				ObjectAttributeRenderingMesh(std::string _mesh_id, EG::Graphics::RenderingMaterial *_material, glm::mat4 _offset = glm::mat4(1.0f));
				~ObjectAttributeRenderingMesh(void);

				void SetMeshId(std::string _mesh_id);
				void SetMaterial(EG::Graphics::RenderingMaterial *_material);
				std::string GetMeshId(void);
				EG::Graphics::RenderingMaterial *GetMaterial(void);
                void SetOffset(glm::mat4 _offset);
                glm::mat4 GetOffset(void);
			private:
				std::string mesh_id;
				EG::Graphics::RenderingMaterial *material;
                glm::mat4 offset;
		};

		class ObjectAttributeRenderingCamera : public ObjectAttribute{
			public:
				ObjectAttributeRenderingCamera(EG::Graphics::Camera *_camera);
				~ObjectAttributeRenderingCamera(void);

				void SetCamera(EG::Graphics::Camera *_camera);
				EG::Graphics::Camera *GetCamera(void);
			private:
				EG::Graphics::Camera *camera;
		};
	}
}

#endif
