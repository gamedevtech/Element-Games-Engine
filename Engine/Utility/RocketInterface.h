#ifndef EG_UTILITY_ROCKET_INTERFACE_H
#define EG_UTILITY_ROCKET_INTERFACE_H

#include <Rocket/Core.h>
#include <Rocket/Controls.h>
//#include <Rocket/Debugger.h>

#include "Time.h"

namespace EG{
	namespace Utility{
		class RocketSystemInterface : public Rocket::Core::SystemInterface{
			public:
				RocketSystemInterface(void);
				~RocketSystemInterface(void);

				void SetTimer(EG::Utility::Time *_time);

				virtual float GetElapsedTime(void);
			private:
				EG::Utility::Time *time;
		};

		class RocketRenderInterface : public Rocket::Core::RenderInterface{
			public:
				RocketRenderInterface(void);
				~RocketRenderInterface(void);

				virtual void RenderGeometry(Rocket::Core::Vertex* vertices, int num_vertices, int* indices, int num_indices, Rocket::Core::TextureHandle texture, const Rocket::Core::Vector2f& translation);
				virtual void EnableScissorRegion(bool enabled);
				virtual void SetScissorRegion(int x, int y, int w, int h);

			private:
				//
		};

		class RocketInterface{
			public:
				RocketInterface(EG::Utility::Time *time);
				~RocketInterface(void);

				void Update(void);
				void Draw(void);
			private:
				RocketRenderInterface *render_interface;
				RocketSystemInterface *system_interface;
				Rocket::Core::Context *context;
		};
	}
}

#endif
