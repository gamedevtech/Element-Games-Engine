#ifndef EG_UTILITY_ROCKET_INTERFACE_H
#define EG_UTILITY_ROCKET_INTERFACE_H

#include <Rocket/Core.h>
#include <Rocket/Controls.h>
#include <Rocket/Debugger.h>

#include "../Graphics/ShaderManager.h"
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
				RocketRenderInterface(EG::Graphics::ShaderManager *_shaders);
				~RocketRenderInterface(void);

				virtual void RenderGeometry(Rocket::Core::Vertex* vertices, int num_vertices, int* indices, int num_indices, Rocket::Core::TextureHandle texture, const Rocket::Core::Vector2f& translation);
				virtual void EnableScissorRegion(bool enabled);
				virtual void SetScissorRegion(int x, int y, int w, int h);

				virtual bool LoadTexture(Rocket::Core::TextureHandle& texture_handle, Rocket::Core::Vector2i& texture_dimensions, const Rocket::Core::String& source);
				virtual bool GenerateTexture(Rocket::Core::TextureHandle& texture_handle, const Rocket::Core::byte* source, const Rocket::Core::Vector2i& source_dimensions);
				virtual void ReleaseTexture(Rocket::Core::TextureHandle texture_handle);
			private:
				void SetVertex(Rocket::Core::Vertex *vertex);
				void SetColor(Rocket::Core::Colourb color);
				EG::Graphics::ShaderManager *shaders;
		};

		typedef Rocket::Core::Event Event;
		class RocketEventListener : public Rocket::Core::EventListener{
			public:
				RocketEventListener(void);
				virtual void ProcessEvent(Rocket::Core::Event &event) = 0;

				void SetDocument(Rocket::Core::ElementDocument *_document);
			protected:
				Rocket::Core::ElementDocument *document;
		};

		class RocketInterface{
			public:
				RocketInterface(std::string file_path, EG::Utility::Time *time, EG::Graphics::ShaderManager *shaders, EG::Input::Input *_input);
				~RocketInterface(void);

				void Update(void);
				void Draw(void);

				void RegisterEventListener(std::string event_type, std::string element_id, RocketEventListener *listener);
			private:
				RocketRenderInterface *render_interface;
				RocketSystemInterface *system_interface;
				Rocket::Core::Context *context;
				Rocket::Core::ElementDocument *document;
				EG::Input::Input *input;
		};
	}
}

#endif
