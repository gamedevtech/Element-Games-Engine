#ifndef EG_GRAPHICS_CAMERA_MANAGER_H
#define EG_GRAPHICS_CAMERA_MANAGER_H

#include <unordered_map>
#include <string>
#include "Camera.h"

namespace EG{
    namespace Graphics{
        class CameraManager{
        public:
            CameraManager(void);
            ~CameraManager(void);

            void Add(std::string camera_name, EG::Graphics::Camera *camera);
            EG::Graphics::Camera *Get(std::string camera_name);
        private:
            std::unordered_map<std::string, EG::Graphics::Camera *> cameras;
        };
    }
}

#endif
