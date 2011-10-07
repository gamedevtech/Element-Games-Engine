#ifndef EG_UTILITY_GUI_H
#define EG_UTILITY_GUI_H

#include "RocketInterface.h"

namespace EG{
	namespace Utility{
		class GUI{
			public:
				GUI(void);
				~GUI(void);
			private:
				EG::Utility::RocketInterface *rocket;
		};
	}
}

#endif
