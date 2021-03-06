#ifndef EG_GRAPHICS_MESH_H
#define EG_GRAPHICS_MESH_H

#include <string>
#include <vector>
#include <map>
#include "../Math/Math.h"

namespace EG{
    namespace Graphics{// assumes vertices are in clockwise order
        class Triangle{
            public:
                Triangle(void);
                ~Triangle(void);

                Triangle &operator=(Triangle &t);

                void CalculateFaceNormal(void);
                void CalculateFaceTangent(void);
                void CalculateFaceBitangent(void);

                // supplied data
                glm::vec4 vertices[3];		// vertices
                glm::vec4 texcoords[3];		// texcoords
                glm::vec4 colors[3];		// colors

                // Post computed data for lighting (tangents, bitangents, normals)
                // switch to Vector3f for normals, tangents and bitangents
                glm::vec4 face_normal;		// triangle wide normal
                glm::vec4 normals[3];		// per vertex tangents normals
                glm::vec4 face_tangent;		// triangle wide tangent
                glm::vec4 tangents[3];		// per vertex tangents
                glm::vec4 face_bitangent;	// triangle wide bitangent
                glm::vec4 bitangents[3];	// per vertex bitangents

                // skeletal information
                std::vector<std::pair<float, float> > weights[3];
        };

        class TriangleMesh{
            public:
                TriangleMesh(unsigned int _triangle_count, Triangle *_triangles, bool compute_face_normals = false, bool compute_vertex_normals = false, bool compute_face_tangents = false, bool compute_vertex_tangents = false, bool compute_face_bitangents = false, bool compute_vertex_bitangents = false, bool _has_skeleton = false);
                ~TriangleMesh(void);

                int GenerateFaceNormals(void);
                int GenerateNormals(float adjacency_tolerance = 0.00000000001f);
                int GenerateFaceTangents(void);
                int GenerateTangents(float adjacency_tolerance = 0.00000000001f);
                int GenerateFaceBitangents(void);
                int GenerateBitangents(float adjacency_tolerance = 0.00000000001f);

                unsigned int GetTriangleCount(void);
                std::vector<Triangle> *GetTriangles(void);
                std::map<unsigned int, glm::mat4> *GetBoneTransformations(void);

                bool HasVertices(void);
                bool HasTexCoords(void);
                bool HasNormals(void);
                bool HasBinormals(void);
                bool HasBitangents(void);
                bool HasSkeleton(void);
            private:
                unsigned int triangle_count;
                std::vector<Triangle> triangles;
                bool has_vertices, has_texcoords, has_normals, has_binormals, has_bitangents, has_skeleton;
        };

        class Mesh{
            public:
                Mesh(EG::Graphics::TriangleMesh *triangle_mesh);
                Mesh(unsigned int _vertex_count, unsigned int _stride, float *_vertices, bool _has_vertices, float *_texcoords, bool _has_texcoords, float *_normals, bool _has_normals, float *_binormals, bool _has_binormals, float *_bitangents, bool _has_bitangents, float *_weights, float *_bone_indices, float _has_skeleton);
                ~Mesh(void);

                unsigned int GetVertexCount(void);
                unsigned int GetStride(void);
                float *GetVertices(void);
                float *GetTexCoords(void);
                float *GetNormals(void);
                float *GetBinormals(void);
                float *GetBitangents(void);
                float *GetWeights(void);
                float *GetWeightVertexIndices(void);
                bool HasVertices(void);
                bool HasNormals(void);
                bool HasTexCoords(void);
                bool HasBinormals(void);
                bool HasBitangents(void);
                bool HasSkeleton(void);
                void GenerateBoundingBox(void);
                glm::vec3 *GetBoundingBox(void);

                void Draw(void);
            private:
                // Mesh Data
                float *vertices, *texcoords, *normals, *binormals, *bitangents;
                bool has_vertices, has_texcoords, has_normals, has_binormals, has_bitangents, has_skeleton;
                unsigned int vertex_count;
                unsigned int stride;

                // Picking Data
                glm::vec3 box[2];

                // Skeletal Data
                //unsigned int *weight_vertex_indices;
                float *weights, *weight_vertex_indices;

                // Buffer Data
                unsigned int *vertex_buffer_object_ids;
                unsigned int vertex_array_object_id;
                void GenerateBuffer(void);

                void GenerateMeshFromTriangleArrayMesh(EG::Graphics::TriangleMesh *triangle_mesh);
        };
    }
}

#endif
