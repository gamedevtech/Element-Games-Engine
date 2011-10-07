#include "../OffscreenBuffer.h"
#include "../GraphicsSubsystem.h"

namespace EG{
	namespace Graphics{
		OffscreenBuffer::OffscreenBuffer(unsigned int _width, unsigned int _height, unsigned int _color_buffer_count, bool _use_depth, OffscreenBufferFilteringType _filtering_type){
			width = _width;
			height = _height;
			color_buffer_count = _color_buffer_count;
			use_depth = _use_depth;
			filtering_type = _filtering_type;

			// Initialize
			textures = new unsigned int[color_buffer_count];
			if (use_depth){
				depth_texture = new unsigned int;
			}
			graphics->CreateOffscreenBuffer(&buffer_id, textures, depth_texture, width, height, color_buffer_count, use_depth, filtering_type);
		}

		OffscreenBuffer::~OffscreenBuffer(void){
			// Free FBOs
		}

		unsigned int OffscreenBuffer::GetWidth(void){
			return width;
		}

		unsigned int OffscreenBuffer::GetHeight(void){
			return height;
		}

		unsigned int OffscreenBuffer::GetColorBufferCount(void){
			return color_buffer_count;
		}

		bool OffscreenBuffer::GetUseDepth(void){
			return use_depth;
		}

		OffscreenBuffer::OffscreenBufferFilteringType OffscreenBuffer::GetFilteringType(void){
			return filtering_type;
		}

		unsigned int *OffscreenBuffer::GetBufferId(void){
			return &buffer_id;
		}

		unsigned int *OffscreenBuffer::GetTextureIds(void){
			return textures;
		}

		unsigned int OffscreenBuffer::GetTextureId(unsigned int index){
			if (index < color_buffer_count && index >= 0){
				return textures[index];
			}
			return 0;
		}

		unsigned int OffscreenBuffer::GetDepthTextureId(void){
			if (use_depth){
				return *depth_texture;
			}
			return 0;
		}
	}
}
