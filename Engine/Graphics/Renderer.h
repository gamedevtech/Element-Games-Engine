#ifndef EG_GRAPHICS_RENDERER_H
#define EG_GRAPHICS_RENDERER_H

#include "GraphicsSubsystem.h"
#include "ShaderManager.h"
#include "Camera.h"
#include "../Game/Scene.h"

namespace EG{
	namespace Graphics{
		class Renderer{
			public:
				Renderer(void);
				~Renderer(void);

				void Initialize(void);
				void Render(EG::Game::Scene *scene);

				// TEMP, SHOULD GO IN SCENE MANAGER WHICH SHOULD HOLD A CAMERA MANAGER
				EG::Graphics::Camera *GetCamera(void){
					return camera;
				}
			protected:
				// TEMP, SHOULD GO IN SCENE MANAGER WHICH SHOULD HOLD A CAMERA MANAGER
				EG::Graphics::Camera *camera;
				EG::Graphics::ShaderManager *shaders;
				glm::mat4 orthographics_projection_matrix;
				bool initialized;
		};
	}
}

#endif
