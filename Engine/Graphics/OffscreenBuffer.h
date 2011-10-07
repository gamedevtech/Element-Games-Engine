#ifndef EG_GRAPHICS_OFFSCREEN_BUFFER_H
#define EG_GRAPHICS_OFFSCREEN_BUFFER_H

namespace EG{
	namespace Graphics{
		class OffscreenBuffer{
			public:
				enum OffscreenBufferFilteringType{
					OFFSCREEN_BUFFER_FILTERING_NONE = 0,
					OFFSCREEN_BUFFER_FILTERING_LINEAR = 1
				};

				OffscreenBuffer(unsigned int _width, unsigned int _height, unsigned int _color_buffer_count, bool _use_depth, OffscreenBufferFilteringType _filtering_type);
				~OffscreenBuffer(void);

				unsigned int GetWidth(void);
				unsigned int GetHeight(void);
				unsigned int GetColorBufferCount(void);
				bool GetUseDepth(void);
				OffscreenBufferFilteringType GetFilteringType(void);
				unsigned int *GetTextureIds(void);
				unsigned int *GetBufferId(void);
				unsigned int GetTextureId(unsigned int index);
				unsigned int GetDepthTextureId(void);
			private:
				unsigned int width, height;
				unsigned int color_buffer_count;
				bool use_depth;

				OffscreenBufferFilteringType filtering_type;
				unsigned int buffer_id;
				unsigned int *textures;
				unsigned int *depth_texture;
		};
	}
}

#endif
