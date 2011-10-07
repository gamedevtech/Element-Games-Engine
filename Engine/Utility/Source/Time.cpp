#include "../Time.h"

namespace EG{
	namespace Utility{
		Time::Time(void){
			started = 0;
			last = 0;
			frame = 0.0f;
			sfml_interface->ResetTime();
		}

		Time::~Time(void){
			//
		}

		void Time::Update(void){
			unsigned int temp = last;
			last = sfml_interface->GetElapsedTime();
			frame = (last - float(temp)) / 1000.0f;
		}

		float Time::GetFrameTime(void){
			return frame;
		}
	}
}
