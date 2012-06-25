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
                KeyFrame(float _frame_time = 0, unsigned int _index = 0, Skeleton *_skeleton = NULL);
                ~KeyFrame(void);

                void SetFrameTime(float _duration);
                void SetIndex(float _index);
                void SetSkeleton(Skeleton *_skeleton);

                unsigned int GetIndex(void);
                float GetFrameTime(void);
                Skeleton *GetSkeleton(void);
            private:
                float frame_time;
                unsigned int index;
                Skeleton *skeleton;
        };

        class Animation{
            public:
                Animation(std::string animation_name, float _duration, KeyFrame *_frames, unsigned int _frame_count);
                ~Animation(void);

                float GetDuration(void);
                std::string GetName(void);
                KeyFrame *GetFrames(void);
                unsigned int GetFrameCount(void);
            private:
                float duration;
                std::string name;
                KeyFrame *frames;
                unsigned int frame_count;
        };

        class Animations{
            public:
                Animations(void);
                ~Animations(void);

                void Add(Animation *_animation);
                Animation *Get(std::string name);
                void SetBindPose(Skeleton *_bind_pose_skeleton);
                Skeleton *GetBindPose(void);
            private:
                EG::Dynamics::Skeleton *bind_pose;
                std::map<std::string, Animation *> animations;
        };

        class AnimationState{
            public:
                AnimationState(Animations *_animations);
                ~AnimationState(void);

                void SetAnimation(std::string animation);
                std::string GetAnimation(void);
                Animations *GetAnimations(void);
                void Update(float frame_time);
                std::vector<glm::mat4> GetTransforms(void);
                void Interpolate(glm::mat4 &bind_trans, Bone *bone, glm::mat4 &start_trans, Bone *start, glm::mat4 &end_trans, Bone *end, float i);
            private:
                std::string current_animation;
                Animations *animations;
                float animation_time;
                std::map<unsigned int, glm::mat4> transforms;
        };
    }
}

#endif
