#ifndef FONT_H
#define FONT_H

#include "../Graphics/GraphicsSubsystem.h"
#include "../Graphics/OpenGLInterface.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/Texture.h"
#include "../Graphics/Mesh.h"
#include "../Math/Math.h"
#include <string>
#include <unordered_map>

#include <FTGL/ftgl.h>
#include <FTGL/FTGLTextureFont.h>

#define font_manager EG::Utility::FontManager::Instance()

namespace EG{
    namespace Utility{
        class Font{
            public:
                Font(std::string font_path);
                ~Font(void);

                void Draw(std::string text, unsigned int size = 12);
            private:
                FTFont *font;
        };

        class FontManager{
            public:
                FontManager(void);
                ~FontManager(void);

                bool AddFont(std::string font_name, std::string font_path);
                bool Draw(std::string text, unsigned int size = 12, std::string font_name = "default");

                static FontManager *Instance(void);
            private:
                static FontManager *instance;
                EG::Graphics::Mesh *rect;
                std::unordered_map<std::string, EG::Utility::Font *> fonts;
        };
    }
}

#endif
