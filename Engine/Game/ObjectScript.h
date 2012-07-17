#ifndef OBJECT_ATTRIBUTE_CONTROL_SCRIPT_H
#define OBJECT_ATTRIBUTE_CONTROL_SCRIPT_H

#include <lua.hpp>
#include <string>
#include <fstream>
#include <vector>

namespace EG{
    namespace Game{
        class ObjectScript{
            public:
                ObjectScript(void *_object = NULL, std::string _filename = "");
                ~ObjectScript(void);

                void Run(void);
            private:
                std::string filename;
                void *object;
                lua_State *lua_state;

                static void hello_world(lua_State *state);
        };
    }
}

#endif
