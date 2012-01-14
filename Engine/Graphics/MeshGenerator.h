#ifndef EG_GRAPHICS_MESH_GENERATOR_H
#define EG_GRAPHICS_MESH_GENERATOR_H

#include "Mesh.h"

#define CUBE_FRONT 0
#define CUBE_BACK 1
#define CUBE_LEFT 2
#define CUBE_RIGHT 3
#define CUBE_TOP 4
#define CUBE_BOTTOM 5

namespace EG{
    namespace Graphics{
        glm::vec4 CubeToSphereMapping(glm::vec4 in);

        EG::Graphics::Mesh *GenerateCube(void);
        EG::Graphics::Mesh *GeneratePlane(float width, float length, unsigned int resolution);
        EG::Graphics::Mesh *GenerateSphere(unsigned int stacks = 16, unsigned int slices = 16);
        EG::Graphics::Mesh *GenerateCubeSphere(unsigned int resolution = 8);
        EG::Graphics::Mesh *GenerateQuad(void);
    }
}

#endif
