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
			private:
				unsigned int started, last;
				float frame;
		};
	}
}

#endif
