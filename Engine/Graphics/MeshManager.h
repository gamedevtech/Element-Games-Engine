#ifndef EG_GRAPHICS_MESH_MANAGER_H
#define EG_GRAPHICS_MESH_MANAGER_H

#include <unordered_map>
#include <string>
#include "Mesh.h"

namespace EG{
    namespace Graphics{
        class MeshManager{
            public:
                MeshManager(void);
                ~MeshManager(void);

                bool Add(std::string mesh_id, EG::Graphics::Mesh *mesh);
                EG::Graphics::Mesh *Get(std::string mesh_id);
            private:
                std::unordered_map<std::string, EG::Graphics::Mesh *> meshes;
        };
    }
}

#endif
