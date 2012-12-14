#ifndef EG_GAME_OBJECT_ATTRIBUTE_H
#define EG_GAME_OBJECT_ATTRIBUTE_H

namespace EG{
    namespace Game{
        class ObjectAttribute{
        public:
            enum ObjectAttributeType{
                OBJECT_ATTRIBUTE_DEFAULT,
                OBJECT_ATTRIBUTE_BASIC_TRANSFORMATION,
                OBJECT_ATTRIBUTE_BASIC_STRING,
                OBJECT_ATTRIBUTE_BASIC_INTEGER,
                OBJECT_ATTRIBUTE_BASIC_FLOAT,
                OBJECT_ATTRIBUTE_BASIC_ENTITY,
                OBJECT_ATTRIBUTE_EMISSION_LIGHT,
                OBJECT_ATTRIBUTE_EMISSION_PARTICLE_SYSTEM,
                OBJECT_ATTRIBUTE_RENDERING_MESH,
                OBJECT_ATTRIBUTE_RENDERING_MATERIAL,
                OBJECT_ATTRIBUTE_RENDERING_SHADER,
                OBJECT_ATTRIBUTE_RENDERING_CAMERA,
                OBJECT_ATTRIBUTE_CONTROL_ANIMATION,
                OBJECT_ATTRIBUTE_CONTROL_RIGID_BODY
            };

            ObjectAttribute(void);
            ~ObjectAttribute(void);

            EG::Game::ObjectAttribute::ObjectAttributeType GetType(void);
        protected:
            EG::Game::ObjectAttribute::ObjectAttributeType type;
        };
    }
}

#endif
