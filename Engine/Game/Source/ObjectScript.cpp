#include "../ObjectScript.h"
#include "../Object.h"

#include "../ObjectBasicAttribute.h"
#include "../ObjectControlAttribute.h"
#include "../ObjectRenderingAttribute.h"
#include "../ObjectEmissionAttribute.h"

namespace EG {
    namespace Game {
        ObjectScript::ObjectScript(std::string _filename) {
            filename = _filename;

            lua_state = luaL_newstate();
            luaL_openlibs(lua_state);
            lua_register(lua_state, "hello_world", (lua_CFunction)(&hello_world));
            lua_register(lua_state, "set_integer", (lua_CFunction)(&set_integer));
            lua_register(lua_state, "set_float", (lua_CFunction)(&set_float));
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

        void ObjectScript::set_integer(lua_State *state) {
            EG::Game::Object *o = static_cast<EG::Game::Object *>(object);
            int n = lua_gettop(state);
            std::string key(lua_tolstring(state, 1, NULL));
            lua_Number value = lua_tonumber(state, 2);

            bool found_attribute = false;
            EG::Game::ObjectAttributeBasicInteger *desired = NULL;
            if (o->HasAttributesOfType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_BASIC_INTEGER)) {
                std::vector<EG::Game::ObjectAttribute *> *attributes = o->GetAttributesByType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_BASIC_INTEGER);
                std::vector<EG::Game::ObjectAttribute *>::iterator attr_iter = attributes->begin();
                while (attr_iter != attributes->end()) {
                    EG::Game::ObjectAttributeBasicInteger *attr = static_cast<EG::Game::ObjectAttributeBasicInteger *>(*attr_iter);
                    if (attr->GetKey() == key) {
                        desired = attr;
                        found_attribute = true;
                        break;
                    }
                    ++attr_iter;
                }
            }

            if (found_attribute) {
                desired->SetValue(value);
            } else {
                o->AddAttribute(new EG::Game::ObjectAttributeBasicInteger(key, value));
            }
        }

        void ObjectScript::set_float(lua_State *state) {
            EG::Game::Object *o = static_cast<EG::Game::Object *>(object);
            int n = lua_gettop(state);
            std::string key(lua_tolstring(state, 1, NULL));
            lua_Number value = lua_tonumber(state, 2);

            bool found_attribute = false;
            EG::Game::ObjectAttributeBasicFloat *desired = NULL;
            if (o->HasAttributesOfType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_BASIC_FLOAT)) {
                std::vector<EG::Game::ObjectAttribute *> *attributes = o->GetAttributesByType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_BASIC_FLOAT);
                std::vector<EG::Game::ObjectAttribute *>::iterator attr_iter = attributes->begin();
                while (attr_iter != attributes->end()) {
                    EG::Game::ObjectAttributeBasicFloat *attr = static_cast<EG::Game::ObjectAttributeBasicFloat *>(*attr_iter);
                    if (attr->GetKey() == key) {
                        desired = attr;
                        found_attribute = true;
                        break;
                    }
                    ++attr_iter;
                }
            }

            if (found_attribute) {
                desired->SetValue(value);
            } else {
                o->AddAttribute(new EG::Game::ObjectAttributeBasicFloat(key, value));
            }
        }

        void *ObjectScript::object = NULL;
    }
}
