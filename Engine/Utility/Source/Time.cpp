#include "../Time.h"

namespace EG{
	namespace Utility{
		Time::Time(void){
			started = 0;
			last = 0;
			frame = 0.0f;
			elapsed = 0.0f;
			sfml_interface->ResetTime();
			frame_counter = 0;
			fps = 0.0f;
			fps_elapsed = 0.0f;
		}

		Time::~Time(void){
			//
		}

		void Time::Update(void){
			unsigned int temp = last;
			last = sfml_interface->GetElapsedTime();
			frame = (last - float(temp)) / 1000.0f;

			elapsed = (last - started) / 1000.0f;

			frame_counter += 1;
			fps_elapsed += frame;
			if (fps_elapsed > 1.0f){
				fps = frame_counter / fps_elapsed;
				frame_counter = 0;
				fps_elapsed = 0.0f;
			}
		}

		float Time::GetElapsedTime(void){
			return elapsed;
		}

		float Time::GetFrameTime(void){
			return frame;
		}

		float Time::GetFPS(void){
			return fps;
		}
	}
}
