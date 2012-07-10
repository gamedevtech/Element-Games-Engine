#ifndef NOISE_H
#define NOISE_H

#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <SFML/Graphics/Image.hpp>
#include "../Graphics/Mesh.h"
#include "../Graphics/MeshGenerator.h"
#include "Math.h"
#define SAMPLE_SIZE 1024

/*
Credit to John Ratcliff where he posted the basic implentation at http://www.flipcode.com/archives/Perlin_Noise_Class.shtml which I exaplanded on. These are also his notes which are a big help!

NOTE: I changed the name of the class to Noise to better suit what I need it for!

This source provides a C++ wrapper around Ken Perlin's noise function. I know there already is a Perlin noise function on the COTD collection, but this one serves a specific purpose. The C++ wrapper allows you to create multiple instances of perlin noise functions so you can have completely determinstic and distinct noise textures. Each instance of the 'Perlin' class can be sampled independently of the other, always returning the same randomized results.

You construct an instance of Perlin as follows: Perlin *perlin = new Perlin(4, 4, 1, 94);
Parameters:
	Cctaves: The first parameter is the number of octaves, this is how noisy or smooth the function is. This is valid between 1 and 16. A value of 4 to 8 octaves produces fairly conventional noise results.

	Frequency: The second parameter is the noise frequency. Values betwen 1 and 8 are reasonable here. You can try sampling the data and plotting it to the screen to see what numbers you like.

	Amplitude: The last parameter is the amplitude. Setting this to a value of 1 will return randomized samples between -1 and +1.

	Seed: The last parameter is the random number seed. This number is what causes this instance of the Perlin to be deterministic and distanct from any other instance. The perlin noise function creates some random number tables that are sampled during queries. This random number seed determines the contents of those tables so that you will get the same exact results every time you use it.

	To retrieve a sample you simply call the method 'Get' and pass it the X and Y sample point to query.
		X and Y should be in the ranges of 0 to 1.
		So if you are sampling for a bitmap be sure and scale the pixel co-ordinates down into normalized values.

	Each instance of Perlin contains it's own random number tables and sampling values.
	This class is extremely convenient if you just need a quick and dirty way to do some kind of distribution pattern that looks halfway decent.
*/

namespace EG{
	namespace Math{
		class Noise{
			public:
				// Leaving the seed as 0, will have the Noise constructor itself generate a psuedo random seed.
				Noise(int _seed = 0, int _octaves = 0, float _frequency = 0.0f, float _amplitude = 1.0);

				float Get(float x, float y){
					float vec[2];
					vec[0] = x;
					vec[1] = y;
					return PerlinNoise2D(vec);
				};
				float Get(float x, float y, float z){
					float vec[3];
					vec[0] = x;
					vec[1] = y;
					vec[2] = z;
					return PerlinNoise3D(vec);
				}

			private:
				void InitNoise(int n, float p);
				float PerlinNoise2D(float vec[2]);
				float PerlinNoise3D(float vec[3]);

				float Noise1(float arg);
				float Noise2(float vec[2]);
				float Noise3(float vec[3]);
				void Normalize2(float v[2]);
				void Normalize3(float v[3]);
				void Init(void);

				bool start;
				int seed;
				int octaves;
				float frequency;
				float amplitude;

				int p[SAMPLE_SIZE + SAMPLE_SIZE + 2];
				float g3[SAMPLE_SIZE + SAMPLE_SIZE + 2][3];
				float g2[SAMPLE_SIZE + SAMPLE_SIZE + 2][2];
				float g1[SAMPLE_SIZE + SAMPLE_SIZE + 2];
		};

		class ColorGradient{
		public:
			ColorGradient(float _range_min, float _range_max, glm::vec4 _begin_color, glm::vec4 _end_color){
				range_min = _range_min;
				range_max = _range_max;
				distance = range_max - range_min;
				begin_color = _begin_color;
				end_color = _end_color;
			}

			~ColorGradient(void){
				//
			}

			glm::vec4 GetColor(float value_in_range){
				glm::vec4 output;
				if (value_in_range <= range_max && value_in_range >= range_min){
					float percentages[2];
					float temp = value_in_range - range_min;
					percentages[1] = temp / distance;
					percentages[0] = 1.0f - percentages[1];
					output = (begin_color * percentages[0]) + (end_color * percentages[1]);
				}
				return output;
			}

			float GetRangeMin(void){
				return range_min;
			}

			float GetRangeMax(void){
				return range_max;
			}

		private:
			float range_min, range_max;
			float distance;
			glm::vec4 begin_color, end_color;
		};

		class ColorGradientSet{
		public:
			ColorGradientSet(void){
				//
			}
			~ColorGradientSet(void){
				//
			}

			void AddColorGradient(float range_min, float range_max, glm::vec4 begin_color, glm::vec4 end_color){
				std::pair<float, float> range;
				range.first = range_min;
				range.second = range_max;
				ColorGradient *color_gradient = new ColorGradient(range_min, range_max, begin_color, end_color);
				gradient_set[range] = color_gradient;
			}

			glm::vec4 GetColor(float value){
				std::map<std::pair<float, float>, ColorGradient*>::iterator giter = gradient_set.begin();
				while (giter != gradient_set.end()){
					if (value <= giter->first.second && value >= giter->first.first){
						return giter->second->GetColor(value);
					}
					++giter;
				}
				return glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			}

			std::map<std::pair<float, float>, ColorGradient*> *GetGradientSet(void){
				return &gradient_set;
			}

		private:
			std::map<std::pair<float, float>, ColorGradient*> gradient_set;
		};

		// NEED Many other features like craters and shit!
		float *GenerateSphereHeightMap(unsigned int width, unsigned int height, Noise *noise_generator, std::string file_path = "");
		float **GenerateCubeSphereHeightMap(unsigned int width, unsigned height, Noise *noise_generator, std::string file_path = "");
        void GenerateSphereNormalHeightMap(unsigned int width, unsigned int height, float *heights, std::string file_path);
		void GenerateCubeSphereNormalHeightMap(unsigned int width, unsigned int height, float **heights, std::string file_path = "");
        glm::vec3 GenerateNormalFromHeights(float left, float right, float top, float bottom, float normal_scaler = 1.1f);
		glm::vec4 *GenerateSphereDecalMap(unsigned int width, unsigned int height, float *heights, unsigned int layer_count, ColorGradientSet *gradients, std::string file_path = "");
		glm::vec4 **GenerateCubeSphereDecalMap(unsigned int width, unsigned int height, float **heights, unsigned int layer_count, ColorGradientSet *gradients, std::string file_path = "");
		glm::vec4 *GenerateGradientMap(unsigned int width, unsigned int height, unsigned int layer_count, ColorGradientSet *gradients, std::string file_path = "");
	}
}

#endif
