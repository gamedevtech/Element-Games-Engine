#include "../CameraManager.h"

namespace EG{
	namespace Graphics{
		CameraManager::CameraManager(void){
			//
		}

		CameraManager::~CameraManager(void){
			//
		}

		void CameraManager::Add(std::string camera_name, EG::Graphics::Camera *camera){
			if (cameras.count(camera_name) < 1){
				cameras[camera_name] = camera;
			}
		}

		EG::Graphics::Camera *CameraManager::Get(std::string camera_name){
			if (cameras.count(camera_name) > 0){
				return cameras[camera_name];
			}
			return NULL;
		}
	}
}
