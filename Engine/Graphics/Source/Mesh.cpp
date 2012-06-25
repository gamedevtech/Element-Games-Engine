#include "../Mesh.h"
#include "../GraphicsSubsystem.h"

#include <iostream>

namespace EG{
    namespace Graphics{
        Triangle::Triangle(void){
            // Just default to white
            colors[0] = glm::vec4(1.0f);
            colors[1] = glm::vec4(1.0f);
            colors[2] = glm::vec4(1.0f);
        }

        Triangle::~Triangle(void){
            // nothing yet to do... perhaps zero everything?
        }

        Triangle &Triangle::operator=(Triangle &t){
            vertices[0] = t.vertices[0];
            vertices[1] = t.vertices[1];
            vertices[2] = t.vertices[2];
            texcoords[0] = t.texcoords[0];
            texcoords[1] = t.texcoords[1];
            texcoords[2] = t.texcoords[2];
            colors[0] = t.colors[0];
            colors[1] = t.colors[1];
            colors[2] = t.colors[2];
            face_normal = t.face_normal;
            normals[0] = t.normals[0];
            normals[1] = t.normals[1];
            normals[2] = t.normals[2];
            face_tangent = t.face_tangent;
            tangents[0] = t.tangents[0];
            tangents[1] = t.tangents[1];
            tangents[2] = t.tangents[2];
            face_bitangent = t.face_bitangent;
            bitangents[0] = t.bitangents[0];
            bitangents[1] = t.bitangents[1];
            bitangents[2] = t.bitangents[2];
            return *this;
        }

        void Triangle::CalculateFaceNormal(void){
            //If you take the cross product of the two vectors: (p1 - p0) and (p2 - p0)
            //where p0, p1 and p2 are three vertices of the triangle, you'll get the normal. A triangle is considered to be pointing towards you if the vertices are ordered clockwise with respect to its outward normal. This is called the left hand rule. Imagine holding your left hand with your fingers curled from p0 to p1, your thumb sticks out in the direction of the face normal:

            glm::vec3 first_vector = glm::vec3(vertices[1].x - vertices[0].x, vertices[1].y - vertices[0].y, vertices[1].z - vertices[0].z);
            glm::vec3 second_vector = glm::vec3(vertices[2].x - vertices[0].x, vertices[2].y - vertices[0].y, vertices[2].z - vertices[0].z);
            glm::vec3 resulting_normal = glm::normalize(glm::cross(first_vector, second_vector));
            face_normal.x = resulting_normal.x;
            face_normal.y = resulting_normal.y;
            face_normal.z = resulting_normal.z;
            face_normal.w = 1.0f;
            normals[0] = face_normal;
            normals[1] = face_normal;
            normals[2] = face_normal;
        }

        void Triangle::CalculateFaceTangent(void){
            float uva = texcoords[1].x - texcoords[0].x;
            float uvb = texcoords[2].x - texcoords[0].x;
            float uvc = texcoords[1].y - texcoords[0].y;
            float uvd = texcoords[2].y - texcoords[0].y;

            float uvk = uvb * uvc - uva * uvd;

            glm::vec4 v1 = vertices[1] - vertices[0];
            glm::vec4 v2 = vertices[2] - vertices[0];

            if (uvk != 0){
                face_tangent = ((v2 * uvc) - (v1 * uvd)) / uvk;
            }else{
                if (uva != 0){
                    face_tangent = v1 / uva;
                    face_tangent.w = 1.0f;
                }else if (uvb != 0){
                    face_tangent = v2 / uvb;
                    face_tangent.w = 1.0f;
                }else{
                    face_tangent = glm::vec4(0.0f);
                }
            }
            tangents[0] = face_tangent;
            tangents[1] = face_tangent;
            tangents[2] = face_tangent;
        }

        void Triangle::CalculateFaceBitangent(void){
            float uva = texcoords[1].x - texcoords[0].x;
            float uvb = texcoords[2].x - texcoords[0].x;
            float uvc = texcoords[1].y - texcoords[0].y;
            float uvd = texcoords[2].y - texcoords[0].y;

            float uvk = uvb * uvc - uva * uvd;

            glm::vec4 v1 = vertices[1] - vertices[0];
            glm::vec4 v2 = vertices[2] - vertices[0];

            if (uvk != 0){
                face_bitangent = ((v2 * uva) - (v1 * uvb)) / uvk;
            }else{
                if (uvc != 0){
                    face_bitangent = v1 / uvc;
                    face_bitangent.w = 1.0f;
                }else if (uvd != 0){
                    face_bitangent = v2 / uvd;
                    face_bitangent.w = 1.0f;
                }else{
                    face_bitangent = glm::vec4(0.0f);
                }
            }
            bitangents[0] = face_bitangent;
            bitangents[1] = face_bitangent;
            bitangents[2] = face_bitangent;
        }

        TriangleMesh::TriangleMesh(unsigned int _triangle_count, Triangle *_triangles, bool compute_face_normals, bool compute_vertex_normals, bool compute_face_tangents, bool compute_vertex_tangents, bool compute_face_bitangents, bool compute_vertex_bitangents, bool _has_skeleton){
            has_vertices = true;
            has_texcoords = true;
            triangle_count = _triangle_count;
            has_skeleton = _has_skeleton;
            for (unsigned int i = 0; i < triangle_count; i++){
                triangles.push_back(_triangles[i]);
            }
            if (compute_vertex_normals || compute_face_normals){
                has_normals = true;
                GenerateFaceNormals();
                if (compute_vertex_normals){
                    GenerateNormals();
                }
            }
            if (compute_vertex_tangents || compute_face_tangents){
                has_binormals = true;
                GenerateFaceTangents();
                if (compute_vertex_tangents){
                    GenerateTangents();
                }
            }
            if (compute_vertex_bitangents || compute_face_bitangents){
                has_bitangents = true;
                GenerateFaceBitangents();
                if (compute_vertex_bitangents){
                    GenerateBitangents();
                }
            }
        }

        TriangleMesh::~TriangleMesh(void){
            triangles.clear();
        }

        int TriangleMesh::GenerateFaceNormals(void){
            has_normals = true;
            std::vector<Triangle>::iterator triangle_iterator = triangles.begin();
            while (triangle_iterator != triangles.end()){
                (*triangle_iterator).CalculateFaceNormal();
                ++triangle_iterator;
            }
            return 1;
        }

        int TriangleMesh::GenerateNormals(float adjacency_tolerance){
            has_normals = true;
            std::vector<Triangle>::iterator triangle_iterator = triangles.begin();
            std::vector<Triangle>::iterator vertex_find_triangle_iterator;
            unsigned int triangle_index = 0;
            while (triangle_iterator != triangles.end()){
                for (unsigned int vertex_index = 0; vertex_index < 3; vertex_index++){
                    glm::vec4 vertex = (*triangle_iterator).vertices[vertex_index];
                    std::vector<Triangle> adjacent_triangles;
                    vertex_find_triangle_iterator = triangles.begin();
                    unsigned int vertex_find_triangle_index = 0;
                    while (vertex_find_triangle_iterator != triangles.end()){
                        bool triangle_matched = false;
                        for (unsigned int vertex_find_vertex_index = 0; vertex_find_vertex_index < 3 && !triangle_matched; vertex_find_vertex_index++){
                            glm::vec4 vertex_find_vertex = (*vertex_find_triangle_iterator).vertices[vertex_find_vertex_index];
                            if (EG::Math::Utility::AbsoluteValue(vertex.x - vertex_find_vertex.x) < adjacency_tolerance && EG::Math::Utility::AbsoluteValue(vertex.y - vertex_find_vertex.y) < adjacency_tolerance && EG::Math::Utility::AbsoluteValue(vertex.z - vertex_find_vertex.z) < adjacency_tolerance){
                                triangle_matched = true;
                                adjacent_triangles.push_back((*vertex_find_triangle_iterator));
                            }
                        }
                        ++vertex_find_triangle_iterator;
                        ++vertex_find_triangle_index;
                    }
                    std::vector<Triangle>::iterator adjacent_triangles_iterator = adjacent_triangles.begin();
                    glm::vec4 vertex_averaged_normal;
                    while (adjacent_triangles_iterator != adjacent_triangles.end()){
                        vertex_averaged_normal += (*adjacent_triangles_iterator).face_normal;
                        ++adjacent_triangles_iterator;
                    }
                    vertex_averaged_normal.w = 1.0f;
                    vertex_averaged_normal = glm::normalize(vertex_averaged_normal);
                    (*triangle_iterator).normals[vertex_index] = vertex_averaged_normal;
                }
                ++triangle_iterator;
                ++triangle_index;
            }
            return 1;
        }

        int TriangleMesh::GenerateFaceTangents(void){
            has_binormals = true;
            std::vector<Triangle>::iterator triangle_iterator = triangles.begin();
            while (triangle_iterator != triangles.end()){
                (*triangle_iterator).CalculateFaceTangent();
                ++triangle_iterator;
            }
            return 1;
        }
        int TriangleMesh::GenerateTangents(float adjacency_tolerance){
            has_binormals = true;
            std::vector<Triangle>::iterator triangle_iterator = triangles.begin();
            std::vector<Triangle>::iterator vertex_find_triangle_iterator;
            while (triangle_iterator != triangles.end()){
                for (unsigned int vertex_index = 0; vertex_index < 3; vertex_index++){
                    glm::vec4 vertex = (*triangle_iterator).vertices[vertex_index];
                    std::vector<Triangle> adjacent_triangles;
                    vertex_find_triangle_iterator = triangles.begin();
                    while (vertex_find_triangle_iterator != triangles.end()){
                        bool triangle_matched = false;
                        for (unsigned int vertex_find_vertex_index = 0; vertex_find_vertex_index < 3 && !triangle_matched; vertex_find_vertex_index++){
                            glm::vec4 vertex_find_vertex = (*vertex_find_triangle_iterator).vertices[vertex_find_vertex_index];
                            if (EG::Math::Utility::AbsoluteValue(vertex.x - vertex_find_vertex.x) < adjacency_tolerance && EG::Math::Utility::AbsoluteValue(vertex.y - vertex_find_vertex.y) < adjacency_tolerance && EG::Math::Utility::AbsoluteValue(vertex.z - vertex_find_vertex.z) < adjacency_tolerance){
                                triangle_matched = true;
                                adjacent_triangles.push_back((*vertex_find_triangle_iterator));
                            }
                        }
                        ++vertex_find_triangle_iterator;
                    }
                    std::vector<Triangle>::iterator adjacent_triangles_iterator = adjacent_triangles.begin();
                    glm::vec4 vertex_averaged_tangent;
                    while (adjacent_triangles_iterator != adjacent_triangles.end()){
                        vertex_averaged_tangent += (*adjacent_triangles_iterator).face_tangent;
                        ++adjacent_triangles_iterator;
                    }
                    vertex_averaged_tangent.w = 1.0f;
                    vertex_averaged_tangent = glm::normalize(vertex_averaged_tangent);
                    (*triangle_iterator).tangents[vertex_index] = vertex_averaged_tangent;
                }
                ++triangle_iterator;
            }
            return 1;
        }
        int TriangleMesh::GenerateFaceBitangents(void){
            has_bitangents = true;
            std::vector<Triangle>::iterator triangle_iterator = triangles.begin();
            while (triangle_iterator != triangles.end()){
                (*triangle_iterator).CalculateFaceBitangent();
                ++triangle_iterator;
            }
            return 1;
        }
        int TriangleMesh::GenerateBitangents(float adjacency_tolerance){
            has_bitangents = true;
            std::vector<Triangle>::iterator triangle_iterator = triangles.begin();
            std::vector<Triangle>::iterator vertex_find_triangle_iterator;
            while (triangle_iterator != triangles.end()){
                for (unsigned int vertex_index = 0; vertex_index < 3; vertex_index++){
                    glm::vec4 vertex = (*triangle_iterator).vertices[vertex_index];
                    std::vector<Triangle> adjacent_triangles;
                    vertex_find_triangle_iterator = triangles.begin();
                    while (vertex_find_triangle_iterator != triangles.end()){
                        bool triangle_matched = false;
                        for (unsigned int vertex_find_vertex_index = 0; vertex_find_vertex_index < 3 && !triangle_matched; vertex_find_vertex_index++){
                            glm::vec4 vertex_find_vertex = (*vertex_find_triangle_iterator).vertices[vertex_find_vertex_index];
                            if (EG::Math::Utility::AbsoluteValue(vertex.x - vertex_find_vertex.x) < adjacency_tolerance && EG::Math::Utility::AbsoluteValue(vertex.y - vertex_find_vertex.y) < adjacency_tolerance && EG::Math::Utility::AbsoluteValue(vertex.x - vertex_find_vertex.z) < adjacency_tolerance){
                                triangle_matched = true;
                                adjacent_triangles.push_back((*vertex_find_triangle_iterator));
                            }
                        }
                        ++vertex_find_triangle_iterator;
                    }
                    std::vector<Triangle>::iterator adjacent_triangles_iterator = adjacent_triangles.begin();
                    glm::vec4 vertex_averaged_bitangent;
                    while (adjacent_triangles_iterator != adjacent_triangles.end()){
                        vertex_averaged_bitangent += (*adjacent_triangles_iterator).face_bitangent;
                        ++adjacent_triangles_iterator;
                    }
                    vertex_averaged_bitangent.w = 1.0f;
                    vertex_averaged_bitangent = glm::normalize(vertex_averaged_bitangent);
                    (*triangle_iterator).bitangents[vertex_index] = vertex_averaged_bitangent;
                }
                ++triangle_iterator;
            }
            return 1;
        }
        unsigned int TriangleMesh::GetTriangleCount(void){
            return triangle_count;
        }
        std::vector<Triangle> *TriangleMesh::GetTriangles(void){
            return &triangles;
        }
        bool TriangleMesh::HasVertices(void){
            return has_vertices;
        }
        bool TriangleMesh::HasTexCoords(void){
            return has_texcoords;
        }
        bool TriangleMesh::HasNormals(void){
            return has_normals;
        }
        bool TriangleMesh::HasBinormals(void){
            return has_binormals;
        }
        bool TriangleMesh::HasBitangents(void){
            return has_bitangents;
        }
        bool TriangleMesh::HasSkeleton(void){
            return has_skeleton;
        }

        Mesh::Mesh(TriangleMesh *triangle_mesh){
            GenerateMeshFromTriangleArrayMesh(triangle_mesh);
            stride = 4;
            GenerateBuffer();
        }

        Mesh::Mesh(unsigned int _vertex_count, unsigned int _stride, float *_vertices, bool _has_vertices, float *_texcoords, bool _has_texcoords, float *_normals, bool _has_normals, float *_binormals, bool _has_binormals, float *_bitangents, bool _has_bitangents){
            vertex_count = _vertex_count;
            stride = _stride;

            has_vertices = _has_vertices;
            has_texcoords = _has_texcoords;
            has_normals = _has_normals;
            has_binormals = _has_binormals;
            has_bitangents = _has_bitangents;
            has_skeleton = false;

            if (has_vertices){
                vertices = _vertices;
            }
            if (has_texcoords){
                texcoords = _texcoords;
            }
            if (has_normals){
                normals = _normals;
            }
            if (has_binormals){
                binormals = _binormals;
            }
            if (has_bitangents){
                bitangents = _bitangents;
            }
            GenerateBuffer();
        }

        Mesh::~Mesh(void){
            if (has_vertices){
                delete []vertices;
            }
            if (has_texcoords){
                delete []texcoords;
            }
            if (has_normals){
                delete []normals;
            }
            if (has_binormals){
                delete []binormals;
            }
            if (has_bitangents){
                delete []bitangents;
            }
        }

        unsigned int Mesh::GetVertexCount(void){
            return vertex_count;
        }
        unsigned int Mesh::GetStride(void){
            return stride;
        }
        float *Mesh::GetVertices(void){
            return vertices;
        }
        float *Mesh::GetTexCoords(void){
            return texcoords;
        }
        float *Mesh::GetNormals(void){
            return normals;
        }
        float *Mesh::GetBinormals(void){
            return binormals;
        }
        float *Mesh::GetBitangents(void){
            return bitangents;
        }
        bool Mesh::HasVertices(void){
            return has_vertices;
        }
        bool Mesh::HasNormals(void){
            return has_normals;
        }
        bool Mesh::HasTexCoords(void){
            return has_texcoords;
        }
        bool Mesh::HasBinormals(void){
            return has_binormals;
        }
        bool Mesh::HasBitangents(void){
            return has_bitangents;
        }

        void Mesh::Draw(void){
            graphics->DrawMesh(&vertex_array_object_id, vertex_count);
        }

        void Mesh::GenerateBuffer(void){
            vertex_buffer_object_ids = new unsigned int[5];
            graphics->GenerateMeshBuffer(&vertex_array_object_id, vertex_buffer_object_ids, vertex_count, has_vertices, vertices, has_texcoords, texcoords, has_normals, normals, has_binormals, binormals, has_bitangents, bitangents, has_skeleton, weight_vertex_indices, weights);
        }

        void Mesh::GenerateMeshFromTriangleArrayMesh(EG::Graphics::TriangleMesh *triangle_mesh){
            has_vertices = triangle_mesh->HasVertices();
            has_texcoords = triangle_mesh->HasTexCoords();
            has_normals = triangle_mesh->HasNormals();
            has_binormals = triangle_mesh->HasBinormals();
            has_bitangents = triangle_mesh->HasBitangents();
            has_skeleton = triangle_mesh->HasSkeleton();

            vertex_count = triangle_mesh->GetTriangleCount() * 3;
            vertices = new float[triangle_mesh->GetTriangleCount() * 3 * 4];
            texcoords = new float[triangle_mesh->GetTriangleCount() * 3 * 4];
            normals = new float[triangle_mesh->GetTriangleCount() * 3 * 4]; // experimental
            binormals = new float[triangle_mesh->GetTriangleCount() * 3 * 4];
            bitangents = new float[triangle_mesh->GetTriangleCount() * 3 * 4];
            weights = new float[triangle_mesh->GetTriangleCount() * 3 * 4];
            weight_vertex_indices = new unsigned int[triangle_mesh->GetTriangleCount() * 3 * 4];

            unsigned int index = 0;
            std::vector<Triangle>::iterator triangle_iterator = triangle_mesh->GetTriangles()->begin();
            while (triangle_iterator != triangle_mesh->GetTriangles()->end()){
                Triangle triangle = (*triangle_iterator);
                for (unsigned int i = 0; i < 3; i++){
                    vertices[index] = triangle.vertices[i].x;
                    vertices[index + 1] = triangle.vertices[i].y;
                    vertices[index + 2] = triangle.vertices[i].z;
                    vertices[index + 3] = triangle.vertices[i].w;
                    texcoords[index] = triangle.texcoords[i].x;
                    texcoords[index + 1] = triangle.texcoords[i].y;
                    texcoords[index + 2] = triangle.texcoords[i].z;
                    texcoords[index + 3] = triangle.texcoords[i].w;

                    if (has_normals){
                        normals[index] = triangle.normals[i].x;
                        normals[index + 1] = triangle.normals[i].y;
                        normals[index + 2] = triangle.normals[i].z;
                        normals[index + 3] = triangle.normals[i].w;
                    }
                    if (has_binormals){
                        binormals[index] = triangle.tangents[i].x;
                        binormals[index + 1] = triangle.tangents[i].y;
                        binormals[index + 2] = triangle.tangents[i].z;
                        binormals[index + 3] = triangle.tangents[i].w;
                    }
                    if (has_bitangents){
                        bitangents[index] = triangle.bitangents[i].x;
                        bitangents[index + 1] = triangle.bitangents[i].y;
                        bitangents[index + 2] = triangle.bitangents[i].z;
                        bitangents[index + 3] = triangle.bitangents[i].w;
                    }

                    if (has_skeleton){
                        std::vector<std::pair<unsigned int, float> > w = triangle.weights[i];
                        for (unsigned int i = 0; i < 4; i++) {
                            if (i < w.size()) {
                                weight_vertex_indices[index + i] = w[i].first;
                                weights[index + i] = w[i].second;
                            } else {
                                weight_vertex_indices[index + i] = 100000;
                                weights[index + i] = 0.0f;
                            }
                            std::cout << index + i << ": " << weight_vertex_indices[index + i] << " " << weights[index + i] << std::endl;
                        }
                    }

                    index += 4;
                }

                ++triangle_iterator;
            }
        }
    }
}
