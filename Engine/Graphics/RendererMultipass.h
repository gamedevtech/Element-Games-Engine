#ifndef EG_GRAPHICS_RENDERER_MULTIPASS_H
#define EG_GRAPHICS_RENDERER_MULTIPASS_H

#include "Renderer.h"
#include "OffscreenBuffer.h"

namespace EG{
	namespace Graphics{
		class RendererMultipass : public Renderer{
			public:
			enum MultipassOutputType{
				MULTIPASS_OUTPUT_NORMAL, MULTIPASS_OUTPUT_BLOOM, MULTIPASS_OUTPUT_PREPASS
			};
				RendererMultipass(void);
				~RendererMultipass(void);

				void Initialize(void);
				void Render(EG::Game::Scene *scene);
				void CycleOutputType(void);
			private:
				EG::Graphics::OffscreenBuffer *light_buffer;
				EG::Graphics::OffscreenBuffer *color_buffer;
				EG::Graphics::OffscreenBuffer *bloom_buffer;
				EG::Graphics::OffscreenBuffer *hdr_buffer;
				glm::mat4 orthographics_projection_matrix;
				EG::Graphics::Mesh *rectangle;

				void Bloom(void);

				MultipassOutputType output_type;
		};
	}
}

#endif
