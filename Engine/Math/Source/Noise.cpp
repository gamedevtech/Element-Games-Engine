/* coherent noise function over 1, 2 or 3 dimensions */
/* (copyright Ken Perlin) */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#include "../Noise.h"

#define B SAMPLE_SIZE
#define BM (SAMPLE_SIZE-1)

#define N 0x1000
#define NP 12   /* 2^N */
#define NM 0xfff

#define s_curve(t) ( t * t * (3.0f - 2.0f * t) )
#define lerp(t, a, b) ( a + t * (b - a) )

#define setup(i,b0,b1,r0,r1)\
    t = vec[i] + N;\
    b0 = ((int)t) & BM;\
    b1 = (b0+1) & BM;\
    r0 = t - (int)t;\
    r1 = r0 - 1.0f;

#define value_at(x, y, w)

namespace EG{
    namespace Math{
        float Noise::Noise1(float arg){
            int bx0, bx1;
            float rx0, rx1, sx, t, u, v, vec[1];

            vec[0] = arg;

            if (start){
                srand(seed);
                start = false;
                Init();
            }

            setup(0, bx0, bx1, rx0, rx1);

            sx = s_curve(rx0);

            u = rx0 * g1[ p[ bx0 ] ];
            v = rx1 * g1[ p[ bx1 ] ];

            return lerp(sx, u, v);
        }

        float Noise::Noise2(float vec[2]){
            int bx0, bx1, by0, by1, b00, b10, b01, b11;
            float rx0, rx1, ry0, ry1, *q, sx, sy, a, b, t, u, v;
            int i, j;

            if (start){
                srand(seed);
                start = false;
                Init();
            }

            setup(0, bx0, bx1, rx0, rx1);
            setup(1, by0, by1, ry0, ry1);

            i = p[bx0];
            j = p[bx1];

            b00 = p[i + by0];
            b10 = p[j + by0];
            b01 = p[i + by1];
            b11 = p[j + by1];

            sx = s_curve(rx0);
            sy = s_curve(ry0);

            #define at2(rx,ry) ( rx * q[0] + ry * q[1] )

            q = g2[b00];
            u = at2(rx0, ry0);
            q = g2[b10];
            v = at2(rx1, ry0);
            a = lerp(sx, u, v);

            q = g2[b01];
            u = at2(rx0,ry1);
            q = g2[b11];
            v = at2(rx1,ry1);
            b = lerp(sx, u, v);

            return lerp(sy, a, b);
        }

        float Noise::Noise3(float vec[3]){
            int bx0, bx1, by0, by1, bz0, bz1, b00, b10, b01, b11;
            float rx0, rx1, ry0, ry1, rz0, rz1, *q, sy, sz, a, b, c, d, t, u, v;
            int i, j;

            if (start){
                srand(seed);
                start = false;
                Init();
            }

            setup(0, bx0, bx1, rx0, rx1);
            setup(1, by0, by1, ry0, ry1);
            setup(2, bz0, bz1, rz0, rz1);

            i = p[ bx0 ];
            j = p[ bx1 ];

            b00 = p[ i + by0 ];
            b10 = p[ j + by0 ];
            b01 = p[ i + by1 ];
            b11 = p[ j + by1 ];

            t  = s_curve(rx0);
            sy = s_curve(ry0);
            sz = s_curve(rz0);

            #define at3(rx,ry,rz) ( rx * q[0] + ry * q[1] + rz * q[2] )

            q = g3[ b00 + bz0 ] ; u = at3(rx0,ry0,rz0);
            q = g3[ b10 + bz0 ] ; v = at3(rx1,ry0,rz0);
            a = lerp(t, u, v);

            q = g3[ b01 + bz0 ] ; u = at3(rx0,ry1,rz0);
            q = g3[ b11 + bz0 ] ; v = at3(rx1,ry1,rz0);
            b = lerp(t, u, v);

            c = lerp(sy, a, b);

            q = g3[ b00 + bz1 ] ; u = at3(rx0,ry0,rz1);
            q = g3[ b10 + bz1 ] ; v = at3(rx1,ry0,rz1);
            a = lerp(t, u, v);

            q = g3[ b01 + bz1 ] ; u = at3(rx0,ry1,rz1);
            q = g3[ b11 + bz1 ] ; v = at3(rx1,ry1,rz1);
            b = lerp(t, u, v);

            d = lerp(sy, a, b);

            return lerp(sz, c, d);
        }

        void Noise::Normalize2(float v[2]){
            float s;

            s = (float)sqrt(v[0] * v[0] + v[1] * v[1]);
            s = 1.0f/s;
            v[0] = v[0] * s;
            v[1] = v[1] * s;
        }

        void Noise::Normalize3(float v[3]){
            float s;

            s = (float)sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
            s = 1.0f/s;

            v[0] = v[0] * s;
            v[1] = v[1] * s;
            v[2] = v[2] * s;
        }

        void Noise::Init(void){
            int i, j, k;

            for (i = 0 ; i < B ; i++){
                p[i] = i;
                g1[i] = (float)((rand() % (B + B)) - B) / B;
                for (j = 0 ; j < 2 ; j++)
                    g2[i][j] = (float)((rand() % (B + B)) - B) / B;
                Normalize2(g2[i]);
                for (j = 0 ; j < 3 ; j++)
                    g3[i][j] = (float)((rand() % (B + B)) - B) / B;
                Normalize3(g3[i]);
            }

            while (--i){
                k = p[i];
                p[i] = p[j = rand() % B];
                p[j] = k;
            }

            for (i = 0 ; i < B + 2 ; i++){
                p[B + i] = p[i];
                g1[B + i] = g1[i];
                for (j = 0 ; j < 2 ; j++)
                    g2[B + i][j] = g2[i][j];
                for (j = 0 ; j < 3 ; j++)
                    g3[B + i][j] = g3[i][j];
            }
        }

        float Noise::PerlinNoise2D(float vec[2]){
            float result = 0.0f;
            float amp = amplitude;

            vec[0] *= frequency;
            vec[1] *= frequency;

            for(int i = 0; i < octaves; i++){
                result += Noise2(vec) * amp;
                vec[0] *= 2.0f;
                vec[1] *= 2.0f;
                amp *= 0.5f;
            }

            return result;
        }

        float Noise::PerlinNoise3D(float vec[3]){
            float result = 0.0f;
            float amp = amplitude;

            vec[0] *= frequency;
            vec[1] *= frequency;
            vec[2] *= frequency;

            for(int i = 0; i < octaves; i++){
                result += Noise3(vec) * amp;
                vec[0] *= 2.0f;
                vec[1] *= 2.0f;
                vec[2] *= 2.0f;
                amp *= 0.5f;
            }

            return result;
        }

        Noise::Noise(int _seed, int _octaves, float _frequency, float _amplitude){
            octaves = _octaves;
            frequency = _frequency;
            amplitude = _amplitude;
            seed = _seed;
            start = true;

            if (seed == 0){
                srand((unsigned)time(NULL));
                seed = rand() % 99999999;
            }
            if (octaves == 0){
                octaves = rand() % 16;
                octaves += 1;
            }
            if (frequency <= 0.0f){
                frequency = (rand() % 80000) / 10000.0f;
            }
        }

        float *GenerateSphereHeightMap(unsigned int width, unsigned int height, Noise *noise_generator, std::string file_path){
            double PI = 3.1415926535897;
            unsigned int height_count = width * height;
            float *heights = new float[height_count];
            unsigned int height_index = 0;

            bool image_dump = (file_path != "") ? true : false;
            unsigned char *pixels;
            unsigned int index = 0;
            if (image_dump){
                pixels = new unsigned char[width * height * 4];
            }

            for (unsigned int y_index = 0; y_index < height; ++y_index){
                //float y = -((2.0 * (y_index / double(height - 1))) - 1.0); // Worse Distribution
                float y = -cos(PI * y_index / height); // Better Distribution
                float r = sqrt(1 - pow(y, 2));
                for (unsigned int x_index = 0; x_index < width; ++x_index){
                    float angle = (x_index / double(width - 1)) * 2.0 * PI;
                    float x = r * sin(angle);
                    float z = r * cos(angle);

                    float x2 = (x + 1.0f) / 2.0f;
                    float y2 = (y + 1.0f) / 2.0f;
                    float z2 = (z + 1.0f) / 2.0f;

                    float generated_height = noise_generator->Get(x2, y2, z2);
                    heights[height_index] = generated_height;
                    height_index += 1;

// 						if (y_index == 0 || y_index == (height - 1)){
// 							std::cout << y_index << ": " << generated_height << std::endl;
// 						}

                    if (image_dump){
                        unsigned char pixel_value = (((generated_height + 1.0f) / 2.0f) * 255);
                        pixels[index] = pixel_value;
                        pixels[index + 1] = pixel_value;
                        pixels[index + 2] = pixel_value;
                        pixels[index + 3] = 255;
                        index += 4;
                    }
                }
            }
            if (image_dump){
                sf::Image image_out;
                image_out.create(width, height, pixels);
                bool image_result = image_out.saveToFile(file_path);
                delete []pixels;
            }
            return heights;
        }

        void GenerateSphereNormalHeightMap(unsigned int width, unsigned int height, float *heights, std::string file_path){
            unsigned int normal_count = height * width;
            unsigned int normal_index = 0;

            bool image_dump = (file_path != "") ? true : false;
            unsigned char *pixels;
            unsigned int index = 0;
            if (image_dump){
                pixels = new unsigned char[normal_count * 4];
            }

            for (unsigned int z = 0; z < height; ++z){
                for(unsigned int x = 0; x < width; ++x){
                    unsigned int x_left =   (x == 0)            ? x : (x - 1);
                    unsigned int x_right =  (x == (width - 1))  ? x : (x + 1);
                    unsigned int z_top =    (z == (height - 1)) ? z : (z + 1);
                    unsigned int z_bottom = (z == 0)            ? z : (z - 1);

                    float normal_map_scalar = 1.1f;
                    float left = heights[(z * width) + x_left] * normal_map_scalar;
                    float right = heights[(z * width) + x_right] * normal_map_scalar;
                    float top = heights[(z_top * width) + x] * normal_map_scalar;
                    float bottom = heights[(z_bottom * width) + x] * normal_map_scalar;
                    glm::vec3 n = GenerateNormalFromHeights(left, right, top, bottom);
                    normal_index += 1;

                    if (image_dump){
                        pixels[index] = (unsigned char)((n.x * 128.0f) + 128.0f);
                        pixels[index + 1] = (unsigned char)((n.y * 128.0f) + 128.0f);
                        pixels[index + 2] = (unsigned char)((n.z * 128.0f) + 128.0f);
                        pixels[index + 3] = 255;
                        index += 4;
                    }
                }
            }

            if (image_dump){
                sf::Image image_out;
                image_out.create(width, height, pixels);
                bool image_result = image_out.saveToFile(file_path);
                delete []pixels;
            }
        }

        glm::vec4 *GenerateSphereDecalMap(unsigned int width, unsigned int height, float *heights, unsigned int layer_count, ColorGradientSet *gradients, std::string file_path){
            unsigned char *pixels;
            bool image_dump = (file_path != "") ? true : false;
            unsigned int p_index = 0;
            if (image_dump){
                pixels = new unsigned char[width * height * 4];
            }

            glm::vec4 *colors = new glm::vec4[width * height];
            unsigned int color_index = 0;
            for (unsigned int y_index = 0; y_index < height; ++y_index){
                for (unsigned int x_index = 0; x_index < width; ++x_index){
                    float generated_height = heights[(y_index * width) + x_index];
                    colors[color_index] = gradients->GetColor(generated_height);

                    if (image_dump){
                        pixels[p_index] = (unsigned char)(colors[color_index].x * 255); // r
                        pixels[p_index + 1] = (unsigned char)(colors[color_index].y * 255); // g
                        pixels[p_index + 2] = (unsigned char)(colors[color_index].z * 255); // b
                        pixels[p_index + 3] = (unsigned char)(colors[color_index].w * 255); // a
                        p_index += 4;
                    }

                    color_index += 1;
                }
            }
            if (image_dump){
                sf::Image image_out;
                image_out.create(width, height, pixels);
                bool image_result = image_out.saveToFile(file_path);
                delete []pixels;
            }
            return colors;
        }

        glm::vec4 *GenerateGradientMap(unsigned int width, unsigned int height, unsigned int layer_count, ColorGradientSet *gradients, std::string file_path){
            unsigned char *pixels;
            bool image_dump = (file_path != "") ? true : false;
            if (image_dump){
                pixels = new unsigned char[width * height * 4];
            }
            glm::vec4 *colors = new glm::vec4[width * height];
            //glm::vec4 temp = gradients->GetColor(0.5f);
            //std::cout << temp.x << ' ' << temp.y << ' ' << temp.z << std::endl;

            unsigned int pixel_index = 0;
            unsigned int color_index = 0;
            for (unsigned int height_index = 0; height_index < height; height_index++){
                for (unsigned int width_index = 0; width_index < width; width_index++){
                    float gradient_index = ((width_index / float(width)) * 2.0f) - 1.0f;
                    //gradient_index = (width_index / float(width));

                    colors[color_index] = gradients->GetColor(gradient_index);
                    //std::cout << colors[color_index].x << ' ' << colors[color_index].y << ' ' << colors[color_index].z << std::endl;

                    pixels[pixel_index] = (unsigned char)(colors[color_index].x * 255);
                    pixels[pixel_index + 1] = (unsigned char)(colors[color_index].y * 255);
                    pixels[pixel_index + 2] = (unsigned char)(colors[color_index].z * 255);
                    pixels[pixel_index + 3] = 255;//(unsigned char)(colors[color_index].x * 255);
                    pixel_index += 4;
                    color_index += 1;
                }
            }

            if (image_dump){
                sf::Image image_out;
                image_out.create(width, height, pixels);
                bool image_result = image_out.saveToFile(file_path);
                //std::cout << file_path << std::endl;
                delete []pixels;
            }
            return colors;
        }

        float **GenerateCubeSphereHeightMap(unsigned int width, unsigned height, Noise *noise_generator, std::string file_path){
            float **heights;
            bool image_dump = (file_path != "") ? true : false;
            if (image_dump){
                heights = new float*[6];
            }
            for (unsigned int i = 0; i < 6; i++){
                unsigned char *pixels;
                unsigned int p_index = 0;
                if (image_dump){
                    pixels = new unsigned char[width * height * 4];
                    heights[i] = new float[width * height];
                }

                unsigned int height_index = 0;
                for (unsigned int y_index = 0; y_index < height; y_index++){
                    float y = ((y_index / float(height)) * 2.0f) - 1.0f;
                    for (unsigned int x_index = 0; x_index < width; x_index++){
                        float x = ((x_index / float(width)) * 2.0f) - 1.0f;

                        float out = 0.0f;
                        glm::vec4 in;
                        if (i == CUBE_FRONT){ // Front
                            in = glm::vec4(-x, y, -1.0f, 1.0f);
                        }else if (i == CUBE_BACK){ // Back
                            in = glm::vec4(x, y, 1.0f, 1.0f);
                        }else if (i == CUBE_LEFT){ // Left
                            in = glm::vec4(-1.0f, y, x, 1.0f);
                        }else if (i == CUBE_RIGHT){ // Right
                            in = glm::vec4(1.0f, y, -x, 1.0f);
                        }else if (i == CUBE_TOP){ // Top
                            in = glm::vec4(x, -1.0f, y, 1.0f);
                        }else if (i == CUBE_BOTTOM){ // Bottom
                            in = glm::vec4(x, 1.0f, -y, 1.0f);
                        }
                        in = EG::Graphics::CubeToSphereMapping(in);

                        out = noise_generator->Get(in.x, in.y, in.z);
                        heights[i][height_index] = out;
                        height_index += 1;
                        if (image_dump){
                            unsigned char pixel_value = (((out + 1.0f) / 2.0f) * 255);
                            pixels[p_index] = pixel_value;
                            pixels[p_index + 1] = pixel_value;
                            pixels[p_index + 2] = pixel_value;
                            pixels[p_index + 3] = 255;
                            p_index += 4;
                        }
                    }
                }
                if (image_dump){
                    sf::Image image_out;
                    image_out.create(width, height, pixels);
                    std::stringstream file_path_out;
                    std::string first_part = file_path.substr(0, file_path.find_first_of('.'));
                    file_path_out << first_part << '_' << i << ".png";
                    bool image_result = image_out.saveToFile(file_path_out.str());
                    delete []pixels;
                }
            }
            return heights;
        }

        glm::vec3 GenerateNormalFromHeights(float left, float right, float top, float bottom, float normal_scaler) {
            glm::vec3 v1(1.0f, 0.0f, right - left);
            glm::vec3 v2(0.0f, 1.0f, top - bottom);
            glm::vec3 n = glm::cross(v1, v2);
            n = glm::normalize(n);
            n *= normal_scaler;
            return n;
        }

        glm::vec4 **GenerateCubeSphereDecalMap(unsigned int width, unsigned int height, float **heights, unsigned int layer_count, ColorGradientSet *gradients, std::string file_path){
            glm::vec4 **out = new glm::vec4 *[6];

            bool image_dump = (file_path != "") ? true : false;

            for (unsigned int i = 0; i < 6; i++){
                std::stringstream file_path_out;
                std::string first_part = file_path.substr(0, file_path.find_first_of('.'));
                file_path_out << first_part << '_' << i << ".png";
                out[i] = EG::Math::GenerateSphereDecalMap(width, height, heights[i], layer_count, gradients, file_path_out.str());
            }

            return out;
        }

        void GenerateCubeSphereNormalHeightMap(unsigned int width, unsigned int height, float **heights, std::string file_path){
            bool image_dump = (file_path != "") ? true : false;

            for (unsigned int i = 0; i < 6; i++){
                std::stringstream file_path_out;
                std::string first_part = file_path.substr(0, file_path.find_first_of('.'));
                file_path_out << first_part << '_' << i << ".png";
                EG::Math::GenerateSphereNormalHeightMap(width, height, heights[i], file_path_out.str());
            }
        }

        void GenerateSphereSpecularMap(unsigned int width, unsigned int height, float *heights, std::vector<std::pair<glm::vec2, float> > specular_mapping, std::string file_path) {
            unsigned int pixel_count = height * width;

            bool image_dump = (file_path != "") ? true : false;
            unsigned char *pixels;
            unsigned int index = 0;
            if (image_dump){
                pixels = new unsigned char[pixel_count * 4];
            }

            for (unsigned int z = 0; z < height; ++z){
                for(unsigned int x = 0; x < width; ++x){
                    float height = heights[(z * width) + x];
                    float specular_value = 1.0f;

                    std::vector<std::pair<glm::vec2, float> >::iterator mapping_iterator = specular_mapping.begin();
                    while (mapping_iterator != specular_mapping.end()) {
                        glm::vec2 range = (*mapping_iterator).first;
                        float specular_scaling = (*mapping_iterator).second;

                        if (height >= range.x && height <= range.y) {
                            specular_value = specular_scaling;
                            break;
                        }

                        ++mapping_iterator;
                    }

                    if (image_dump){
                        unsigned char out_value = (unsigned char)(specular_value * 255.0f);
                        pixels[index] = out_value;
                        pixels[index + 1] = out_value;
                        pixels[index + 2] = out_value;
                        pixels[index + 3] = 255;
                        index += 4;
                    }
                }
            }

            if (image_dump){
                sf::Image image_out;
                image_out.create(width, height, pixels);
                bool image_result = image_out.saveToFile(file_path);
                delete []pixels;
            }
        }

        void GenerateCubeSphereSpecularMap(unsigned int width, unsigned int height, float **heights, std::vector<std::pair<glm::vec2, float> > specular_mapping, std::string file_path) {
            bool image_dump = (file_path != "") ? true : false;

            for (unsigned int i = 0; i < 6; i++){
                std::stringstream file_path_out;
                std::string first_part = file_path.substr(0, file_path.find_first_of('.'));
                file_path_out << first_part << '_' << i << ".png";
                EG::Math::GenerateSphereSpecularMap(width, height, heights[i], specular_mapping, file_path_out.str());
            }
        }
    }
}
