#include "../ObjectScript.h"
#include "../Object.h"

namespace EG {
    namespace Game {
        ObjectScript::ObjectScript(void *_object, std::string _filename) {
            object = _object;
            filename = _filename;

            lua_state = luaL_newstate();
            luaL_openlibs(lua_state);
            lua_register(lua_state, "hello_world", (lua_CFunction)(&hello_world));
        }

        ObjectScript::~ObjectScript(void) {
            lua_close(lua_state);
        }

        void ObjectScript::Run(void) {
            luaL_dofile(lua_state, filename.c_str());
        }

        void ObjectScript::hello_world(lua_State *state) {
            std::cout << "Hello World!" << std::endl;
        }
    }
}
