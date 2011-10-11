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
			EG::Graphics::Camera *current_cam = cameras.Get(camera_name);
			//if (cameras.count(camera_name) < 1){
			if (current_cam == NULL){
				//cameras[camera_name] = camera;
				cameras.Set(camera_name, camera);
			}
		}

		EG::Graphics::Camera *CameraManager::Get(std::string camera_name){
			EG::Graphics::Camera *current_cam = cameras.Get(camera_name);
			//if (cameras.count(camera_name) > 0){
			if (current_cam != NULL){
				//return cameras[camera_name];
				return current_cam;
			}
			return NULL;
		}
	}
}
