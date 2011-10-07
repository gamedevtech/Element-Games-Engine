#ifndef EG_GRAPHICS_TEXTURE_H
#define EG_GRAPHICS_TEXTURE_H

#include <vector>
#include <string>

namespace EG{
	namespace Graphics{
		class Texture{
			public:
				Texture(std::string _file_path = "", bool _filtering = true, float anisotropic_param = 16.0f);
				~Texture(void);

				unsigned int Load(std::string _file_path = "", bool _filtering = true, float anisotropic_param = 16.0f);
				unsigned int GetId(void);
				std::string GetFilePath(void);
				unsigned int GetWidth(void);
				unsigned int GetHeight(void);
			private:
				unsigned int width, height;
				bool filtering;
				unsigned int id;
				std::string file_path;
		};

		class CubeMap{
			public:
				CubeMap(std::string positive_x_file_path, std::string negative_x_file_path, std::string positive_y_file_path, std::string negative_y_file_path, std::string positive_z_file_path, std::string negative_z_file_path, bool _filtering = true, float anisotropic_param = 16.0f);
				~CubeMap(void);

				unsigned int Load(std::string positive_x_file_path, std::string negative_x_file_path, std::string positive_y_file_path, std::string negative_y_file_path, std::string positive_z_file_path, std::string negative_z_file_path, bool _filtering = true, float anisotropic_param = 16.0f);

				unsigned int GetId(void);
				std::string GetFilePath(unsigned int index = 0);
				unsigned int GetWidth(void);
				unsigned int GetHeight(void);
			private:
				unsigned int width, height;
				std::string file_paths[6];
				bool filtering;
				unsigned int id;
		};
	}
}

#endif
