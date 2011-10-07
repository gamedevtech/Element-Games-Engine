#include "../MeshGenerator.h"

#include <iostream>
#include <cmath>

namespace EG{
	namespace Graphics{
		glm::vec4 CubeToSphereMapping(glm::vec4 in){
			if (in.x < -1.0f || in.x > 1.0f || in.y < -1.0f || in.y > 1.0f || in.z < -1.0f || in.z > 1.0f){
				std::cout << "Out of RANGE!" << std::endl;
			}

			glm::vec4 out;
			out.x = in.x * sqrtf(1.0f - in.y * in.y * 0.5f - in.z * in.z * 0.5f + in.y * in.y * in.z * in.z / 3.0f);
			out.y = in.y * sqrtf(1.0f - in.z * in.z * 0.5f - in.x * in.x * 0.5f + in.z * in.z * in.x * in.x / 3.0f);
			out.z = in.z * sqrtf(1.0f - in.x * in.x * 0.5f - in.y * in.y * 0.5f + in.x * in.x * in.y * in.y / 3.0f);
			out.w = 1.0f;
			return out;
		}

		EG::Graphics::Mesh *GenerateCube(void){
			EG::Graphics::Triangle *triangles = new EG::Graphics::Triangle[12];
			glm::vec4 triangle_vertices[8];
			triangle_vertices[0] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			triangle_vertices[1] = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
			triangle_vertices[2] = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
			triangle_vertices[3] = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
			triangle_vertices[4] = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
			triangle_vertices[5] = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
			triangle_vertices[6] = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
			triangle_vertices[7] = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);

			// VERTICES 1 5 6 1 6 2
			// front
			triangles[0].vertices[0] = triangle_vertices[0];
			triangles[0].vertices[1] = triangle_vertices[1];
			triangles[0].vertices[2] = triangle_vertices[2];
			triangles[1].vertices[0] = triangle_vertices[0];
			triangles[1].vertices[1] = triangle_vertices[2];
			triangles[1].vertices[2] = triangle_vertices[3];
			// back
			triangles[2].vertices[0] = triangle_vertices[5];
			triangles[2].vertices[1] = triangle_vertices[4];
			triangles[2].vertices[2] = triangle_vertices[7];
			triangles[3].vertices[0] = triangle_vertices[5];
			triangles[3].vertices[1] = triangle_vertices[7];
			triangles[3].vertices[2] = triangle_vertices[6];
			// top
			triangles[4].vertices[0] = triangle_vertices[3];
			triangles[4].vertices[1] = triangle_vertices[2];
			triangles[4].vertices[2] = triangle_vertices[6];
			triangles[5].vertices[0] = triangle_vertices[3];
			triangles[5].vertices[1] = triangle_vertices[6];
			triangles[5].vertices[2] = triangle_vertices[7];
			// bottom
			triangles[6].vertices[0] = triangle_vertices[4];
			triangles[6].vertices[1] = triangle_vertices[5];
			triangles[6].vertices[2] = triangle_vertices[1];
			triangles[7].vertices[0] = triangle_vertices[4];
			triangles[7].vertices[1] = triangle_vertices[1];
			triangles[7].vertices[2] = triangle_vertices[0];
			// left
			triangles[8].vertices[0] = triangle_vertices[4];
			triangles[8].vertices[1] = triangle_vertices[0];
			triangles[8].vertices[2] = triangle_vertices[3];
			triangles[9].vertices[0] = triangle_vertices[4];
			triangles[9].vertices[1] = triangle_vertices[3];
			triangles[9].vertices[2] = triangle_vertices[7];
			// right
			triangles[10].vertices[0] = triangle_vertices[1];
			triangles[10].vertices[1] = triangle_vertices[5];
			triangles[10].vertices[2] = triangle_vertices[6];
			triangles[11].vertices[0] = triangle_vertices[1];
			triangles[11].vertices[1] = triangle_vertices[6];
			triangles[11].vertices[2] = triangle_vertices[2];

			// TEXCOORDS
			glm::vec4 triangle_texcoords[4];
			triangle_texcoords[0] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			triangle_texcoords[1] = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
			triangle_texcoords[2] = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
			triangle_texcoords[3] = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
			// front
			triangles[0].texcoords[0] = triangle_texcoords[0];
			triangles[0].texcoords[1] = triangle_texcoords[1];
			triangles[0].texcoords[2] = triangle_texcoords[2];
			triangles[1].texcoords[0] = triangle_texcoords[0];
			triangles[1].texcoords[1] = triangle_texcoords[2];
			triangles[1].texcoords[2] = triangle_texcoords[3];
			// back
			triangles[2].texcoords[0] = triangle_texcoords[0];
			triangles[2].texcoords[1] = triangle_texcoords[1];
			triangles[2].texcoords[2] = triangle_texcoords[2];
			triangles[3].texcoords[0] = triangle_texcoords[0];
			triangles[3].texcoords[1] = triangle_texcoords[2];
			triangles[3].texcoords[2] = triangle_texcoords[3];
			// left
			triangles[4].texcoords[0] = triangle_texcoords[0];
			triangles[4].texcoords[1] = triangle_texcoords[1];
			triangles[4].texcoords[2] = triangle_texcoords[2];
			triangles[5].texcoords[0] = triangle_texcoords[0];
			triangles[5].texcoords[1] = triangle_texcoords[2];
			triangles[5].texcoords[2] = triangle_texcoords[3];
			// right
			triangles[6].texcoords[0] = triangle_texcoords[0];
			triangles[6].texcoords[1] = triangle_texcoords[1];
			triangles[6].texcoords[2] = triangle_texcoords[2];
			triangles[7].texcoords[0] = triangle_texcoords[0];
			triangles[7].texcoords[1] = triangle_texcoords[2];
			triangles[7].texcoords[2] = triangle_texcoords[3];
			// top
			triangles[8].texcoords[0] = triangle_texcoords[0];
			triangles[8].texcoords[1] = triangle_texcoords[1];
			triangles[8].texcoords[2] = triangle_texcoords[2];
			triangles[9].texcoords[0] = triangle_texcoords[0];
			triangles[9].texcoords[1] = triangle_texcoords[2];
			triangles[9].texcoords[2] = triangle_texcoords[3];
			// bottom
			triangles[10].texcoords[0] = triangle_texcoords[0];
			triangles[10].texcoords[1] = triangle_texcoords[1];
			triangles[10].texcoords[2] = triangle_texcoords[2];
			triangles[11].texcoords[0] = triangle_texcoords[0];
			triangles[11].texcoords[1] = triangle_texcoords[2];
			triangles[11].texcoords[2] = triangle_texcoords[3];

			EG::Graphics::TriangleMesh *triangle_mesh = new EG::Graphics::TriangleMesh(12, triangles, true, false, true, true, true, true);
			return new EG::Graphics::Mesh(triangle_mesh);
		}

		EG::Graphics::Mesh *GeneratePlane(float width, float length, unsigned int resolution){
			unsigned int triangle_count = ((resolution * resolution) * 2);
			EG::Graphics::Triangle *triangles = new EG::Graphics::Triangle[triangle_count];

			unsigned int triangle_index = 0;
			float x_position = -(width / 2.0f);
			float z_position = -(length / 2.0f);
			float x_slice = width / float(resolution);
			float z_slice = length / float(resolution);
			for (unsigned int x_index = 0; x_index < resolution; x_index++){
				x_position += x_slice;
				z_position = -(length / 2.0f);
				for (unsigned int z_index = 0; z_index < resolution; z_index++){
					glm::vec4 vertices[4];
					vertices[0] = glm::vec4(x_position, 0.0f, z_position, 1.0f);
					vertices[1] = glm::vec4(x_position + x_slice, 0.0f, z_position, 1.0f);
					vertices[2] = glm::vec4(x_position + x_slice, 0.0f, z_position + z_slice, 1.0f);
					vertices[3] = glm::vec4(x_position, 0.0f, z_position + z_slice, 1.0f);
					glm::vec4 texcoords[4];
					texcoords[0] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
					texcoords[1] = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
					texcoords[2] = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
					texcoords[3] = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
					glm::vec4 normal = glm::vec4(0.0f, -1.0f, 0.0f, 1.0f);

					triangles[triangle_index].vertices[0] = vertices[0];
					triangles[triangle_index].vertices[1] = vertices[2];
					triangles[triangle_index].vertices[2] = vertices[1];
					triangles[triangle_index].texcoords[0] = texcoords[0];
					triangles[triangle_index].texcoords[1] = texcoords[2];
					triangles[triangle_index].texcoords[2] = texcoords[1];
					triangles[triangle_index].normals[0] = normal;
					triangles[triangle_index].normals[1] = normal;
					triangles[triangle_index].normals[2] = normal;
					triangle_index += 1;
					triangles[triangle_index].vertices[0] = vertices[0];
					triangles[triangle_index].vertices[1] = vertices[3];
					triangles[triangle_index].vertices[2] = vertices[2];
					triangles[triangle_index].texcoords[0] = texcoords[0];
					triangles[triangle_index].texcoords[1] = texcoords[3];
					triangles[triangle_index].texcoords[2] = texcoords[2];
					triangles[triangle_index].normals[0] = normal;
					triangles[triangle_index].normals[1] = normal;
					triangles[triangle_index].normals[2] = normal;
					triangle_index += 1;

					z_position += z_slice;
				}
			}

			EG::Graphics::TriangleMesh *triangle_mesh = new EG::Graphics::TriangleMesh(triangle_count, triangles, false, false, true, true, true, true);
			return new EG::Graphics::Mesh(triangle_mesh);
		}

		EG::Graphics::Mesh *GenerateSphere(unsigned int stacks, unsigned int slices){
			double PI = 3.1415926535897;
			std::vector<glm::vec4> points, uv;
			for (unsigned int stack = 0; stack < (stacks + 1); ++stack){
				//float y = -((2.0 * (stack / double(stacks))) - 1.0); // Worse Distribution
				float y = -cos(PI * stack / stacks); // Better Distribution
				float v = 1.0f - (stack / double(stacks));
				float r = sqrt(1 - pow(y, 2));
				for (unsigned int slice = 0; slice < (slices + 1); ++slice){
					float angle = (slice / double(slices)) * 2.0 * PI;
					float x = r * sin(angle);
					float z = r * cos(angle);
					float u = slice / double(slices);

					glm::vec4 point(x, y, z, 1.0f);
					glm::vec4 tcoord(u, v, 1.0f, 1.0f);
					points.push_back(point);
					uv.push_back(tcoord);
				}
			}

			unsigned int top_left = 0;
			unsigned int top_right = 1;
			unsigned int bottom_left = 2;
			unsigned int bottom_right = 3;
			unsigned int triangle_count = stacks * slices * 2;
			EG::Graphics::Triangle *triangles = new EG::Graphics::Triangle[triangle_count];
			unsigned int triangle_index = 0;
			for (unsigned int y = 0; y < stacks; y++){
				for (unsigned int x = 0; x < slices; x++){
					// Build Indices
					unsigned int point_indices[4];
					point_indices[top_left] = y * (slices + 1) + x;
					point_indices[top_right] = y * (slices + 1) + x + 1;
					point_indices[bottom_left] = (y + 1) * (slices + 1) + x;
					point_indices[bottom_right] = (y + 1) * (slices + 1) + x + 1;

					// Gather Corners
					glm::vec4 vertices[4];
					vertices[top_left] = points[point_indices[top_left]];
					vertices[top_right] = points[point_indices[top_right]];
					vertices[bottom_left] = points[point_indices[bottom_left]];
					vertices[bottom_right] = points[point_indices[bottom_right]];
					glm::vec4 texcoords[4];
					texcoords[top_left] = uv[point_indices[top_left]];
					texcoords[top_right] = uv[point_indices[top_right]];
					texcoords[bottom_left] = uv[point_indices[bottom_left]];
					texcoords[bottom_right] = uv[point_indices[bottom_right]];
					glm::vec4 normals[4];
					normals[top_left] = points[point_indices[top_left]];
					normals[top_right] = points[point_indices[top_right]];
					normals[bottom_left] = points[point_indices[bottom_left]];
					normals[bottom_right] = points[point_indices[bottom_right]];
					glm::normalize(normals[top_left]);
					glm::normalize(normals[top_right]);
					glm::normalize(normals[bottom_left]);
					glm::normalize(normals[bottom_right]);

					// Build Triangles
					EG::Graphics::Triangle *t = &(triangles[triangle_index]);
					t->vertices[0] = vertices[bottom_left];
					t->vertices[1] = vertices[bottom_right];
					t->vertices[2] = vertices[top_right];
					t->texcoords[0] = texcoords[bottom_left];
					t->texcoords[1] = texcoords[bottom_right];
					t->texcoords[2] = texcoords[top_right];
					t->normals[0] = normals[bottom_left];
					t->normals[1] = normals[bottom_right];
					t->normals[2] = normals[top_right];
					triangle_index += 1;

					t = &(triangles[triangle_index]);
					t->vertices[0] = vertices[bottom_left];
					t->vertices[1] = vertices[top_right];
					t->vertices[2] = vertices[top_left];
					t->texcoords[0] = texcoords[bottom_left];
					t->texcoords[1] = texcoords[top_right];
					t->texcoords[2] = texcoords[top_left];
					t->normals[0] = normals[bottom_left];
					t->normals[1] = normals[top_right];
					t->normals[2] = normals[top_left];
					triangle_index += 1;
				}
			}

			EG::Graphics::TriangleMesh *triangle_mesh = new EG::Graphics::TriangleMesh(triangle_count, triangles, false, false, true, true, true, true);
			return new EG::Graphics::Mesh(triangle_mesh);
		}

		EG::Graphics::Mesh *GenerateCubeSphere(unsigned int resolution){
			unsigned int top_left = 0;
			unsigned int top_right = 1;
			unsigned int bottom_left = 2;
			unsigned int bottom_right = 3;

			unsigned int triangle_count = (resolution * resolution) * 2 * 6;
			EG::Graphics::Triangle *triangles = new EG::Graphics::Triangle[triangle_count];
			unsigned int triangle_index = 0;

			// Front
			for (unsigned int x_index = 0; x_index < resolution; x_index++){
				float x_left = ((x_index / float(resolution)) * 2.0f) - 1.0f;
				float x_right = (((x_index + 1) / float(resolution)) * 2.0f) - 1.0f;
				float u_left = x_index / float(resolution);
				float u_right = (x_index + 1) / float(resolution);
				for (unsigned int y_index = 0; y_index < resolution; y_index++){
					float y_bottom = ((y_index / float(resolution)) * 2.0f) - 1.0f;
					float y_top = (((y_index + 1) / float(resolution)) * 2.0f) - 1.0f;
					float v_bottom = y_index / float(resolution);
					float v_top = (y_index + 1) / float(resolution);

					EG::Graphics::Triangle *t1 = &(triangles[triangle_index]);
					++triangle_index;
					EG::Graphics::Triangle *t2 = &(triangles[triangle_index]);
					++triangle_index;

					glm::vec4 vertices[4];
					vertices[top_left] = glm::vec4(x_left, y_top, -1.0f, 1.0f);
					vertices[top_right] = glm::vec4(x_right, y_top, -1.0f, 1.0f);
					vertices[bottom_left] = glm::vec4(x_left, y_bottom, -1.0f, 1.0f);
					vertices[bottom_right] = glm::vec4(x_right, y_bottom, -1.0f, 1.0f);
					vertices[top_left] = CubeToSphereMapping(vertices[top_left]);
					vertices[top_right] = CubeToSphereMapping(vertices[top_right]);
					vertices[bottom_left] = CubeToSphereMapping(vertices[bottom_left]);
					vertices[bottom_right] = CubeToSphereMapping(vertices[bottom_right]);
// 						vertices[top_left].Print();

					glm::vec4 texcoords[4];
					texcoords[top_left] = glm::vec4(u_left, v_top, 0.0f, 1.0f);
					texcoords[top_right] = glm::vec4(u_right, v_top, 0.0f, 1.0f);
					texcoords[bottom_left] = glm::vec4(u_left, v_bottom, 0.0f, 1.0f);
					texcoords[bottom_right] = glm::vec4(u_right, v_bottom, 0.0f, 1.0f);

					t1->texcoords[0] = texcoords[bottom_left];
					t1->texcoords[1] = texcoords[bottom_right];
					t1->texcoords[2] = texcoords[top_right];

					t2->texcoords[0] = texcoords[bottom_left];
					t2->texcoords[1] = texcoords[top_right];
					t2->texcoords[2] = texcoords[top_left];

					t1->vertices[0] = vertices[bottom_left];
					t1->vertices[1] = vertices[bottom_right];
					t1->vertices[2] = vertices[top_right];

					t2->vertices[0] = vertices[bottom_left];
					t2->vertices[1] = vertices[top_right];
					t2->vertices[2] = vertices[top_left];

					t1->normals[0] = vertices[bottom_left];
					t1->normals[1] = vertices[bottom_right];
					t1->normals[2] = vertices[top_right];

					t2->normals[0] = vertices[bottom_left];
					t2->normals[1] = vertices[top_right];
					t2->normals[2] = vertices[top_left];
				}
			}

			// Back
			for (unsigned int x_index = 0; x_index < resolution; x_index++){
				float x_left = ((x_index / float(resolution)) * 2.0f) - 1.0f;
				float x_right = (((x_index + 1) / float(resolution)) * 2.0f) - 1.0f;
				float u_left = x_index / float(resolution);
				float u_right = (x_index + 1) / float(resolution);
				for (unsigned int y_index = 0; y_index < resolution; y_index++){
					float y_bottom = ((y_index / float(resolution)) * 2.0f) - 1.0f;
					float y_top = (((y_index + 1) / float(resolution)) * 2.0f) - 1.0f;
					float v_bottom = y_index / float(resolution);
					float v_top = (y_index + 1) / float(resolution);

					EG::Graphics::Triangle *t1 = &(triangles[triangle_index]);
					++triangle_index;
					EG::Graphics::Triangle *t2 = &(triangles[triangle_index]);
					++triangle_index;

					glm::vec4 vertices[4];
					vertices[top_left] = glm::vec4(x_left, y_top, 1.0f, 1.0f);
					vertices[top_right] = glm::vec4(x_right, y_top, 1.0f, 1.0f);
					vertices[bottom_left] = glm::vec4(x_left, y_bottom, 1.0f, 1.0f);
					vertices[bottom_right] = glm::vec4(x_right, y_bottom, 1.0f, 1.0f);
					vertices[top_left] = CubeToSphereMapping(vertices[top_left]);
					vertices[top_right] = CubeToSphereMapping(vertices[top_right]);
					vertices[bottom_left] = CubeToSphereMapping(vertices[bottom_left]);
					vertices[bottom_right] = CubeToSphereMapping(vertices[bottom_right]);

					glm::vec4 texcoords[4];
					texcoords[top_left] = glm::vec4(u_left, v_top, 0.0f, 1.0f);
					texcoords[top_right] = glm::vec4(u_right, v_top, 0.0f, 1.0f);
					texcoords[bottom_left] = glm::vec4(u_left, v_bottom, 0.0f, 1.0f);
					texcoords[bottom_right] = glm::vec4(u_right, v_bottom, 0.0f, 1.0f);

					t1->texcoords[0] = texcoords[bottom_left];
					t1->texcoords[1] = texcoords[bottom_right];
					t1->texcoords[2] = texcoords[top_right];

					t2->texcoords[0] = texcoords[bottom_left];
					t2->texcoords[1] = texcoords[top_right];
					t2->texcoords[2] = texcoords[top_left];

					t1->vertices[0] = vertices[bottom_left];
					t1->vertices[1] = vertices[bottom_right];
					t1->vertices[2] = vertices[top_right];

					t2->vertices[0] = vertices[bottom_left];
					t2->vertices[1] = vertices[top_right];
					t2->vertices[2] = vertices[top_left];

					t1->normals[0] = vertices[bottom_left];
					t1->normals[1] = vertices[bottom_right];
					t1->normals[2] = vertices[top_right];

					t2->normals[0] = vertices[bottom_left];
					t2->normals[1] = vertices[top_right];
					t2->normals[2] = vertices[top_left];
				}
			}

			// Left
			for (unsigned int x_index = 0; x_index < resolution; x_index++){
				float x_left = ((x_index / float(resolution)) * 2.0f) - 1.0f;
				float x_right = (((x_index + 1) / float(resolution)) * 2.0f) - 1.0f;
				float u_left = x_index / float(resolution);
				float u_right = (x_index + 1) / float(resolution);
				for (unsigned int y_index = 0; y_index < resolution; y_index++){
					float y_bottom = ((y_index / float(resolution)) * 2.0f) - 1.0f;
					float y_top = (((y_index + 1) / float(resolution)) * 2.0f) - 1.0f;
					float v_bottom = y_index / float(resolution);
					float v_top = (y_index + 1) / float(resolution);

					EG::Graphics::Triangle *t1 = &(triangles[triangle_index]);
					++triangle_index;
					EG::Graphics::Triangle *t2 = &(triangles[triangle_index]);
					++triangle_index;

					glm::vec4 vertices[4];
					vertices[top_left] = glm::vec4(-1.0f, y_top, x_left, 1.0f);
					vertices[top_right] = glm::vec4(-1.0f, y_top, x_right, 1.0f);
					vertices[bottom_left] = glm::vec4(-1.0f, y_bottom, x_left, 1.0f);
					vertices[bottom_right] = glm::vec4(-1.0f, y_bottom, x_right, 1.0f);
					vertices[top_left] = CubeToSphereMapping(vertices[top_left]);
					vertices[top_right] = CubeToSphereMapping(vertices[top_right]);
					vertices[bottom_left] = CubeToSphereMapping(vertices[bottom_left]);
					vertices[bottom_right] = CubeToSphereMapping(vertices[bottom_right]);

					glm::vec4 texcoords[4];
					texcoords[top_left] = glm::vec4(u_left, v_top, 0.0f, 1.0f);
					texcoords[top_right] = glm::vec4(u_right, v_top, 0.0f, 1.0f);
					texcoords[bottom_left] = glm::vec4(u_left, v_bottom, 0.0f, 1.0f);
					texcoords[bottom_right] = glm::vec4(u_right, v_bottom, 0.0f, 1.0f);

					t1->texcoords[0] = texcoords[bottom_left];
					t1->texcoords[1] = texcoords[bottom_right];
					t1->texcoords[2] = texcoords[top_right];

					t2->texcoords[0] = texcoords[bottom_left];
					t2->texcoords[1] = texcoords[top_right];
					t2->texcoords[2] = texcoords[top_left];

					t1->vertices[0] = vertices[bottom_left];
					t1->vertices[1] = vertices[bottom_right];
					t1->vertices[2] = vertices[top_right];

					t2->vertices[0] = vertices[bottom_left];
					t2->vertices[1] = vertices[top_right];
					t2->vertices[2] = vertices[top_left];

					t1->normals[0] = vertices[bottom_left];
					t1->normals[1] = vertices[bottom_right];
					t1->normals[2] = vertices[top_right];

					t2->normals[0] = vertices[bottom_left];
					t2->normals[1] = vertices[top_right];
					t2->normals[2] = vertices[top_left];
				}
			}

			// Right
			for (unsigned int x_index = 0; x_index < resolution; x_index++){
				float x_left = ((x_index / float(resolution)) * 2.0f) - 1.0f;
				float x_right = (((x_index + 1) / float(resolution)) * 2.0f) - 1.0f;
				float u_left = x_index / float(resolution);
				float u_right = (x_index + 1) / float(resolution);
				for (unsigned int y_index = 0; y_index < resolution; y_index++){
					float y_bottom = ((y_index / float(resolution)) * 2.0f) - 1.0f;
					float y_top = (((y_index + 1) / float(resolution)) * 2.0f) - 1.0f;
					float v_bottom = y_index / float(resolution);
					float v_top = (y_index + 1) / float(resolution);

					EG::Graphics::Triangle *t1 = &(triangles[triangle_index]);
					++triangle_index;
					EG::Graphics::Triangle *t2 = &(triangles[triangle_index]);
					++triangle_index;

					glm::vec4 vertices[4];
					vertices[top_left] = glm::vec4(1.0f, y_top, x_left, 1.0f);
					vertices[top_right] = glm::vec4(1.0f, y_top, x_right, 1.0f);
					vertices[bottom_left] = glm::vec4(1.0f, y_bottom, x_left, 1.0f);
					vertices[bottom_right] = glm::vec4(1.0f, y_bottom, x_right, 1.0f);
					vertices[top_left] = CubeToSphereMapping(vertices[top_left]);
					vertices[top_right] = CubeToSphereMapping(vertices[top_right]);
					vertices[bottom_left] = CubeToSphereMapping(vertices[bottom_left]);
					vertices[bottom_right] = CubeToSphereMapping(vertices[bottom_right]);

					glm::vec4 texcoords[4];
					texcoords[top_left] = glm::vec4(u_left, v_top, 0.0f, 1.0f);
					texcoords[top_right] = glm::vec4(u_right, v_top, 0.0f, 1.0f);
					texcoords[bottom_left] = glm::vec4(u_left, v_bottom, 0.0f, 1.0f);
					texcoords[bottom_right] = glm::vec4(u_right, v_bottom, 0.0f, 1.0f);

					t1->texcoords[0] = texcoords[bottom_left];
					t1->texcoords[1] = texcoords[bottom_right];
					t1->texcoords[2] = texcoords[top_right];

					t2->texcoords[0] = texcoords[bottom_left];
					t2->texcoords[1] = texcoords[top_right];
					t2->texcoords[2] = texcoords[top_left];

					t1->vertices[0] = vertices[bottom_left];
					t1->vertices[1] = vertices[bottom_right];
					t1->vertices[2] = vertices[top_right];

					t2->vertices[0] = vertices[bottom_left];
					t2->vertices[1] = vertices[top_right];
					t2->vertices[2] = vertices[top_left];

					t1->normals[0] = vertices[bottom_left];
					t1->normals[1] = vertices[bottom_right];
					t1->normals[2] = vertices[top_right];

					t2->normals[0] = vertices[bottom_left];
					t2->normals[1] = vertices[top_right];
					t2->normals[2] = vertices[top_left];
				}
			}

			// Top
			for (unsigned int x_index = 0; x_index < resolution; x_index++){
				float x_left = ((x_index / float(resolution)) * 2.0f) - 1.0f;
				float x_right = (((x_index + 1) / float(resolution)) * 2.0f) - 1.0f;
				float u_left = x_index / float(resolution);
				float u_right = (x_index + 1) / float(resolution);
				for (unsigned int y_index = 0; y_index < resolution; y_index++){
					float y_bottom = ((y_index / float(resolution)) * 2.0f) - 1.0f;
					float y_top = (((y_index + 1) / float(resolution)) * 2.0f) - 1.0f;
					float v_bottom = y_index / float(resolution);
					float v_top = (y_index + 1) / float(resolution);

					EG::Graphics::Triangle *t1 = &(triangles[triangle_index]);
					++triangle_index;
					EG::Graphics::Triangle *t2 = &(triangles[triangle_index]);
					++triangle_index;

					glm::vec4 vertices[4];
					vertices[top_left] = glm::vec4(x_left, 1.0f, y_top, 1.0f);
					vertices[top_right] = glm::vec4(x_right, 1.0f, y_top, 1.0f);
					vertices[bottom_left] = glm::vec4(x_left, 1.0f, y_bottom, 1.0f);
					vertices[bottom_right] = glm::vec4(x_right, 1.0f, y_bottom, 1.0f);
					vertices[top_left] = CubeToSphereMapping(vertices[top_left]);
					vertices[top_right] = CubeToSphereMapping(vertices[top_right]);
					vertices[bottom_left] = CubeToSphereMapping(vertices[bottom_left]);
					vertices[bottom_right] = CubeToSphereMapping(vertices[bottom_right]);

					glm::vec4 texcoords[4];
					texcoords[top_left] = glm::vec4(u_left, v_top, 0.0f, 1.0f);
					texcoords[top_right] = glm::vec4(u_right, v_top, 0.0f, 1.0f);
					texcoords[bottom_left] = glm::vec4(u_left, v_bottom, 0.0f, 1.0f);
					texcoords[bottom_right] = glm::vec4(u_right, v_bottom, 0.0f, 1.0f);

					t1->texcoords[0] = texcoords[bottom_left];
					t1->texcoords[1] = texcoords[bottom_right];
					t1->texcoords[2] = texcoords[top_right];

					t2->texcoords[0] = texcoords[bottom_left];
					t2->texcoords[1] = texcoords[top_right];
					t2->texcoords[2] = texcoords[top_left];

					t1->vertices[0] = vertices[bottom_left];
					t1->vertices[1] = vertices[bottom_right];
					t1->vertices[2] = vertices[top_right];

					t2->vertices[0] = vertices[bottom_left];
					t2->vertices[1] = vertices[top_right];
					t2->vertices[2] = vertices[top_left];

					t1->normals[0] = vertices[bottom_left];
					t1->normals[1] = vertices[bottom_right];
					t1->normals[2] = vertices[top_right];

					t2->normals[0] = vertices[bottom_left];
					t2->normals[1] = vertices[top_right];
					t2->normals[2] = vertices[top_left];
				}
			}

			// Bottom
			for (unsigned int x_index = 0; x_index < resolution; x_index++){
				float x_left = ((x_index / float(resolution)) * 2.0f) - 1.0f;
				float x_right = (((x_index + 1) / float(resolution)) * 2.0f) - 1.0f;
				float u_left = x_index / float(resolution);
				float u_right = (x_index + 1) / float(resolution);
				for (unsigned int y_index = 0; y_index < resolution; y_index++){
					float y_bottom = ((y_index / float(resolution)) * 2.0f) - 1.0f;
					float y_top = (((y_index + 1) / float(resolution)) * 2.0f) - 1.0f;
					float v_bottom = y_index / float(resolution);
					float v_top = (y_index + 1) / float(resolution);

					EG::Graphics::Triangle *t1 = &(triangles[triangle_index]);
					++triangle_index;
					EG::Graphics::Triangle *t2 = &(triangles[triangle_index]);
					++triangle_index;

					glm::vec4 vertices[4];
					vertices[top_left] = glm::vec4(x_left, -1.0f, y_top, 1.0f);
					vertices[top_right] = glm::vec4(x_right, -1.0f, y_top, 1.0f);
					vertices[bottom_left] = glm::vec4(x_left, -1.0f, y_bottom, 1.0f);
					vertices[bottom_right] = glm::vec4(x_right, -1.0f, y_bottom, 1.0f);
					vertices[top_left] = CubeToSphereMapping(vertices[top_left]);
					vertices[top_right] = CubeToSphereMapping(vertices[top_right]);
					vertices[bottom_left] = CubeToSphereMapping(vertices[bottom_left]);
					vertices[bottom_right] = CubeToSphereMapping(vertices[bottom_right]);

					glm::vec4 texcoords[4];
					texcoords[top_left] = glm::vec4(u_left, v_top, 0.0f, 1.0f);
					texcoords[top_right] = glm::vec4(u_right, v_top, 0.0f, 1.0f);
					texcoords[bottom_left] = glm::vec4(u_left, v_bottom, 0.0f, 1.0f);
					texcoords[bottom_right] = glm::vec4(u_right, v_bottom, 0.0f, 1.0f);

					t1->texcoords[0] = texcoords[bottom_left];
					t1->texcoords[1] = texcoords[bottom_right];
					t1->texcoords[2] = texcoords[top_right];

					t2->texcoords[0] = texcoords[bottom_left];
					t2->texcoords[1] = texcoords[top_right];
					t2->texcoords[2] = texcoords[top_left];

					t1->vertices[0] = vertices[bottom_left];
					t1->vertices[1] = vertices[bottom_right];
					t1->vertices[2] = vertices[top_right];

					t2->vertices[0] = vertices[bottom_left];
					t2->vertices[1] = vertices[top_right];
					t2->vertices[2] = vertices[top_left];

					t1->normals[0] = vertices[bottom_left];
					t1->normals[1] = vertices[bottom_right];
					t1->normals[2] = vertices[top_right];

					t2->normals[0] = vertices[bottom_left];
					t2->normals[1] = vertices[top_right];
					t2->normals[2] = vertices[top_left];
				}
			}

			EG::Graphics::TriangleMesh *triangle_mesh = new EG::Graphics::TriangleMesh(triangle_count, triangles, false, false, true, true, true, true);
			return new EG::Graphics::Mesh(triangle_mesh);
		}
	}
}
