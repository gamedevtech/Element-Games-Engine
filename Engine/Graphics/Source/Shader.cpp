#include "../Shader.h"

#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>

namespace EG{
	namespace Graphics{
		ShaderSource::ShaderSource(std::string vertex_shader_path, std::string fragment_shader_path, std::string geometry_shader_path, std::string tessellation_control_shader_path, std::string tessellation_evaluation_shader_path){
			std::vector<std::string> vertex_shader_source_raw = LoadTextFile(vertex_shader_path);
			std::vector<std::string> fragment_shader_source_raw = LoadTextFile(fragment_shader_path);

			vertex_shader_line_count = vertex_shader_source_raw.size();
			fragment_shader_line_count = fragment_shader_source_raw.size();

			vertex_shader_source = new char *[vertex_shader_line_count];
			fragment_shader_source = new char *[fragment_shader_line_count];

			vertex_shader_line_sizes = new int[vertex_shader_line_count];
			fragment_shader_line_sizes = new int[fragment_shader_line_count];

			unsigned int index = 0;
			for (unsigned int vertex_shader_index = 0; vertex_shader_index < vertex_shader_source_raw.size(); vertex_shader_index++){
				std::string vertex_shader_line = vertex_shader_source_raw[vertex_shader_index];
				vertex_shader_line_sizes[index] = vertex_shader_line.length();
				vertex_shader_source[index] = new char[vertex_shader_line_sizes[index]];
				memcpy((char *)(vertex_shader_source[index]), (char *)(vertex_shader_line.c_str()), sizeof(char) * vertex_shader_line_sizes[index]);
				index += 1;
			}
			index = 0;
			for (unsigned int fragment_shader_index = 0; fragment_shader_index < fragment_shader_source_raw.size(); fragment_shader_index++){
				std::string fragment_shader_line = fragment_shader_source_raw[fragment_shader_index];
				fragment_shader_line_sizes[index] = fragment_shader_line.length();
				fragment_shader_source[index] = new char[fragment_shader_line_sizes[index]];
				memcpy(fragment_shader_source[index], fragment_shader_line.c_str(), fragment_shader_line_sizes[index]);
				index += 1;
			}
			if (geometry_shader_path.length() > 2){
				std::vector<std::string> geometry_shader_source_raw = LoadTextFile(geometry_shader_path);
				geometry_shader_line_count = geometry_shader_source_raw.size();
				geometry_shader_source = new char *[geometry_shader_line_count];
				geometry_shader_line_sizes = new int[geometry_shader_line_count];
				index = 0;
				for (unsigned int geometry_shader_index = 0; geometry_shader_index < geometry_shader_source_raw.size(); geometry_shader_index++){
					std::string geometry_shader_line = geometry_shader_source_raw[geometry_shader_index];
					geometry_shader_line_sizes[index] = geometry_shader_line.length();
					geometry_shader_source[index] = new char[geometry_shader_line_sizes[index]];
					memcpy(geometry_shader_source[index], geometry_shader_line.c_str(), geometry_shader_line_sizes[index]);
					index += 1;
				}
			}else{
				geometry_shader_line_count = 0;
			}

			if (tessellation_control_shader_path.length() > 2){
				std::vector<std::string> tessellation_control_shader_source_raw = LoadTextFile(tessellation_control_shader_path);
				tessellation_control_shader_line_count = tessellation_control_shader_source_raw.size();
				tessellation_control_shader_source = new char *[tessellation_control_shader_line_count];
				tessellation_control_shader_line_sizes = new int[tessellation_control_shader_line_count];
				index = 0;
				for (unsigned int tessellation_control_shader_index = 0; tessellation_control_shader_index < tessellation_control_shader_source_raw.size(); tessellation_control_shader_index++){
					std::string tessellation_control_shader_line = tessellation_control_shader_source_raw[tessellation_control_shader_index];
					tessellation_control_shader_line_sizes[index] = tessellation_control_shader_line.length();
					tessellation_control_shader_source[index] = new char[tessellation_control_shader_line_sizes[index]];
					memcpy(tessellation_control_shader_source[index], tessellation_control_shader_line.c_str(), tessellation_control_shader_line_sizes[index]);
					index += 1;
				}
				//std::cout << "Loaded Source for Tessellation Control." << std::endl;
			}else{
				tessellation_control_shader_line_count = 0;
			}

			if (tessellation_evaluation_shader_path.length() > 2){
				std::vector<std::string> tessellation_evaluation_shader_source_raw = LoadTextFile(tessellation_evaluation_shader_path);
				tessellation_evaluation_shader_line_count = tessellation_evaluation_shader_source_raw.size();
				tessellation_evaluation_shader_source = new char *[tessellation_evaluation_shader_line_count];
				tessellation_evaluation_shader_line_sizes = new int[tessellation_evaluation_shader_line_count];
				index = 0;
				for (unsigned int tessellation_evaluation_shader_index = 0; tessellation_evaluation_shader_index < tessellation_evaluation_shader_source_raw.size(); tessellation_evaluation_shader_index++){
					std::string tessellation_evaluation_shader_line = tessellation_evaluation_shader_source_raw[tessellation_evaluation_shader_index];
					tessellation_evaluation_shader_line_sizes[index] = tessellation_evaluation_shader_line.length();
					tessellation_evaluation_shader_source[index] = new char[tessellation_evaluation_shader_line_sizes[index]];
					memcpy(tessellation_evaluation_shader_source[index], tessellation_evaluation_shader_line.c_str(), tessellation_evaluation_shader_line_sizes[index]);
					index += 1;
				}
				//std::cout << "Loaded Source for Tessellation Evaluation." << std::endl;
			}else{
				tessellation_evaluation_shader_line_count = 0;
			}
		}
		ShaderSource::~ShaderSource(void){
			for (int i = 0; i < vertex_shader_line_count; i++){
				delete vertex_shader_source[i];
			}
			for (int i = 0; i < fragment_shader_line_count; i++){
				delete fragment_shader_source[i];
			}
			for (int i = 0; i < geometry_shader_line_count; i++){
				delete geometry_shader_source[i];
			}
			for (int i = 0; i < tessellation_control_shader_line_count; i++){
				delete tessellation_control_shader_source[i];
			}
			for (int i = 0; i < tessellation_evaluation_shader_line_count; i++){
				delete tessellation_evaluation_shader_source[i];
			}
			if (vertex_shader_source){
				delete vertex_shader_source;
			}
			if (fragment_shader_source){
				delete fragment_shader_source;
			}
			if (geometry_shader_source){
				delete geometry_shader_source;
			}
			if (tessellation_control_shader_source){
				delete tessellation_control_shader_source;
			}
			if (tessellation_evaluation_shader_source){
				delete tessellation_evaluation_shader_source;
			}
			if (vertex_shader_line_sizes){
				delete vertex_shader_line_sizes;
			}
			if (fragment_shader_line_sizes){
				delete fragment_shader_line_sizes;
			}
			if (geometry_shader_line_sizes){
				delete geometry_shader_line_sizes;
			}
			if (tessellation_control_shader_line_sizes){
				delete tessellation_control_shader_line_sizes;
			}
			if (tessellation_evaluation_shader_line_sizes){
				delete tessellation_evaluation_shader_line_sizes;
			}
			vertex_shader_line_count = 0;
			fragment_shader_line_count = 0;
			geometry_shader_line_count = 0;
			tessellation_control_shader_line_count = 0;
			tessellation_evaluation_shader_line_count = 0;
		}

		std::vector<std::string> ShaderSource::LoadTextFile(std::string file_path){
			std::vector<std::string> output;
			std::string line;
			std::ifstream shader_file(file_path.c_str());
			if (shader_file.is_open()){
				while (!shader_file.eof()){
					std::getline(shader_file, line);
					line += "\n";
					output.push_back(line);
				}
				shader_file.close();
			}
			return output;
		}
	}
}
