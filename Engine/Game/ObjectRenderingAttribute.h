#ifndef EG_GAME_OBJECT_RENDERING_ATTRIBUTE_H
#define EG_GAME_OBJECT_RENDERING_ATTRIBUTE_H

#include <string>
#include "ObjectAttribute.h"
#include "../Graphics/RenderingMaterial.h"

namespace EG{
	namespace Game{
		class ObjectAttributeRenderingMesh : public ObjectAttribute{
			public:
				ObjectAttributeRenderingMesh(std::string _mesh_id, EG::Graphics::RenderingMaterial *_material);
				~ObjectAttributeRenderingMesh(void);

				void SetMeshId(std::string _mesh_id);
				void SetMaterial(EG::Graphics::RenderingMaterial *_material);
				std::string GetMeshId(void);
				EG::Graphics::RenderingMaterial *GetMaterial(void);
			private:
				std::string mesh_id;
				EG::Graphics::RenderingMaterial *material;
		};
	}
}

#endif
