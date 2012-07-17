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
                ObjectScript(std::string _filename = "");
                ~ObjectScript(void);

                void Run(void);
                static void *object;
            private:
                std::string filename;
                lua_State *lua_state;

                static void hello_world(lua_State *state);
                static void set_integer(lua_State *state);
                static void set_float(lua_State *state);
        };
    }
}

#endif
