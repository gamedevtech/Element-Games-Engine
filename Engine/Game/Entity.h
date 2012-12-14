#ifndef EG_GAME_ENTITY_H
#define EG_GAME_ENTITY_H

#include <string>

namespace EG{
    namespace Game{
        class Entity {
            public:
                Entity(void);
                ~Entity(void);
            private:
                std::string id;
        };
    }
}

#endif
