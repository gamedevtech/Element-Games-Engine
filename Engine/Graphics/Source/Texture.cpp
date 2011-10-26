#include "../Texture.h"

#include <iostream>
#include "../../Media/SFMLInterface.h"
#include "../../Graphics/GraphicsSubsystem.h"
#include "../../Graphics/MeshGenerator.h"

namespace EG{
	namespace Graphics{
		Texture::Texture(std::string _file_path, bool _filtering, float anisotropic_param){
			width = 0;
			height = 0;
			filtering = false;
			id = 0;
			Load(_file_path, _filtering, anisotropic_param);
		}

		Texture::~Texture(void){
			//
		}

		unsigned int Texture::Load(std::string _file_path, bool _filtering, float anisotropic_param){
			id = 0;
			file_path = _file_path;
			if (file_path.length() > 0){
				bool success = false;
				unsigned char *pixels = sfml_interface->LoadImage(file_path, &success, &width, &height);
				std::cout << "DONGLE1111" << std::endl;
				if (success){
					filtering = _filtering;
					id = graphics->CreateTexture(pixels, width, height, filtering, anisotropic_param);
					if (id == 0){
						std::cout << "Couldn't load image (" << _file_path << ")" << std::endl;
						return 0;
					}
					return id;
					delete []pixels;
				}
			}
			std::cout << "Couldn't load image (" << _file_path << ")" << std::endl;
			return 0;
		}

		std::string Texture::GetFilePath(void){
			return file_path;
		}

		unsigned int Texture::GetId(void){
			return id;
		}

		unsigned int Texture::GetWidth(void){
			return width;
		}

		unsigned int Texture::GetHeight(void){
			return height;
		}

		CubeMap::CubeMap(std::string positive_x_file_path, std::string negative_x_file_path, std::string positive_y_file_path, std::string negative_y_file_path, std::string positive_z_file_path, std::string negative_z_file_path, bool _filtering, float anisotropic_param){
			width = 0;
			height = 0;
			filtering = false;
			id = 0;
			//std::cout << "dongle" << std::endl;
			Load(positive_x_file_path, negative_x_file_path, positive_y_file_path, negative_y_file_path, positive_z_file_path, negative_z_file_path, _filtering, anisotropic_param);
		}

		CubeMap::~CubeMap(void){
			//
		}

		unsigned int CubeMap::Load(std::string positive_x_file_path, std::string negative_x_file_path, std::string positive_y_file_path, std::string negative_y_file_path, std::string positive_z_file_path, std::string negative_z_file_path, bool _filtering, float anisotropic_param){
			file_paths[CUBE_RIGHT] = positive_x_file_path;
			file_paths[CUBE_LEFT] = negative_x_file_path;
			file_paths[CUBE_TOP] = positive_y_file_path;
			file_paths[CUBE_BOTTOM] = negative_y_file_path;
			file_paths[CUBE_BACK] = positive_z_file_path;
			file_paths[CUBE_FRONT] = negative_z_file_path;
			filtering = _filtering;

			id = 0;
			//std::cout << file_paths[CUBE_FRONT] << std::endl;
			if (positive_x_file_path.size() > 0 && negative_x_file_path.size() > 0 && positive_y_file_path.size() > 0 && negative_y_file_path.size() > 0 && positive_z_file_path.size() > 0 && negative_z_file_path.size() > 0){
				unsigned char **pixels = new unsigned char *[6];
				for (unsigned int i = 0; i < 6; i++){
					bool success = false;
					pixels[i] = sfml_interface->LoadImage(file_paths[i], &success, &width, &height);
					if (!success){
						for (unsigned int p = 0; p < (i + 1); p++){
							delete []pixels[p];
						}
						delete []pixels;
						return 0;
					}
				}

				id = graphics->CreateCubeMap(pixels, width, height, filtering, anisotropic_param);
				for (unsigned int i = 0; i < 6; i++){
					delete []pixels[i];
				}
				delete []pixels;

				if (id == 0){
					std::cout << "Couldn't load cube images x:(" << file_paths[0] << ")" << std::endl;
					return 0;
				}
			}
			return id;
		}

		unsigned int CubeMap::GetId(void){
			return id;
		}

		std::string CubeMap::GetFilePath(unsigned int index){
			if (index >= 0 && index <= 5){
				return file_paths[index];
			}
			return "";
		}

		unsigned int CubeMap::GetWidth(void){
			return width;
		}

		unsigned int CubeMap::GetHeight(void){
			return height;
		}
	}
}
