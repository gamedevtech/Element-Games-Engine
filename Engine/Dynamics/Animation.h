#ifndef ANIMATION_H
#define ANIMATION_H

#include <vector>
#include <string>
#include <map>
#include "../Math/Math.h"

namespace EG{
    namespace Dynamics{
        class Bone{
            public:
                Bone(unsigned int _id, glm::mat4 _offset = glm::mat4(1.0));
                ~Bone(void);

                unsigned int GetId(void);
                glm::mat4 GetOffset(void);

                void SetParent(Bone *_parent);
                void AddChild(Bone *_child);

                Bone *GetParent(void);
                std::vector<Bone *> *GetChildren(void);
            private:
                unsigned int id;
                glm::mat4 offset;
                Bone *parent;
                std::vector<Bone *> children;
        };

        class Skeleton{
            public:
                Skeleton(void);
                ~Skeleton(void);

                void SetRoot(Bone *_root);
                Bone *GetRoot(void);
                std::vector<Bone *> *GetBones(void);
                void AddBone(Bone *bone);

                void PrintRecursive(Bone *b = NULL);
            private:
                unsigned int print_level;
                std::vector<Bone *> bones;
                Bone *root;
        };

        class KeyFrame{
            public:
                KeyFrame(float _duration, unsigned int _index, Skeleton *_skeleton);
                ~KeyFrame(void);

                unsigned int GetIndex(void);
                float GetDuration(void);
                Skeleton *GetSkeleton(void);
            private:
                float duration;
                unsigned int index;
                Skeleton *skeleton;
        };

        class Animation{
            public:
                Animation(std::string animation_name, float _duration, KeyFrame *_frames);
                ~Animation(void);

                float GetDuration(void);
                std::string GetName(void);
                KeyFrame *GetFrames(void);
            private:
                float duration;
                std::string name;
                KeyFrame *frames;
        };

        class Animations{
            public:
                Animations(void);
                ~Animations(void);

                void Add(Animation *_animation);
                Animation *Get(std::string name);
            private:
                std::map<std::string, Animation *> animations;
        };

        class AnimationState{
            public:
                AnimationState(Animations *_animations);
                ~AnimationState(void);

                void SetAnimation(std::string animation);
            private:
                std::string current_animation;
                Animations *animations;
        };
    }
}

#endif
