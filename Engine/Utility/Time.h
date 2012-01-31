#ifndef EG_UTILITY_TIME_H
#define EG_UTILITY_TIME_H

#include "../Media/SFMLInterface.h"

namespace EG{
	namespace Utility{
		class Time{
			public:
				Time(void);
				~Time(void);

				void Update(void);
				float GetFrameTime(void);
				float GetElapsedTime(void);
				float GetFPS(void);
			private:
				unsigned int frame_counter;
				float fps_elapsed;
				unsigned int started, last;
				float fps;
				float frame;
				float elapsed;
		};
	}
}

#endif
