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
                std::map<unsigned int, Bone *> *GetBoneMap(void);
                void AddBone(Bone *bone);
                Bone *GetBone(unsigned int bone_id);

                void PrintRecursive(Bone *b = NULL);
            private:
                unsigned int print_level;
                std::map<unsigned int, Bone *> bone_map;
                std::vector<Bone *> bones;
                Bone *root;
        };

        class Animation {
            public:
                Animation(std::string animation_name, float _duration);
                ~Animation(void);

                float GetDuration(void);
                std::string GetName(void);
                std::vector<glm::mat4> GetTransforms(float time_stamp, std::map<unsigned int, std::pair<float, unsigned int> > *position_state, std::map<unsigned int, std::pair<float, unsigned int> > *scaling_state, std::map<unsigned int, std::pair<float, unsigned int> > *rotation_state);
                unsigned int GetBoneCount(void);

                void SetBoneCount(unsigned int _bone_count);
                void AddBonePosition(unsigned int bone_id, float time, glm::vec3 position);
                void AddBoneScaling(unsigned int bone_id, float time, glm::vec3 scaling);
                void AddBoneRotation(unsigned int bone_id, float time, glm::quat rotation);

                std::map<unsigned int, std::vector<std::pair<float, glm::vec3> > > *GetPositions(void);
                std::map<unsigned int, std::vector<std::pair<float, glm::vec3> > > *GetScalings(void);
                std::map<unsigned int, std::vector<std::pair<float, glm::quat> > > *GetRotations(void);
            private:
                float duration;
                std::string name;
                unsigned int bone_count;
                // Store frame time stamp (not frame duration) and value for each component of the transformation.
                // This makes it easier and cheaper to interpolate with the cost of a bit of super cheap ram.
                // Per bone_id, store a list of pairs of timestamps and transform components
                std::map<unsigned int, std::vector<std::pair<float, glm::vec3> > > positions;
                std::map<unsigned int, std::vector<std::pair<float, glm::vec3> > > scalings;
                std::map<unsigned int, std::vector<std::pair<float, glm::quat> > > rotations;
        };

        class Animations{
            public:
                Animations(void);
                ~Animations(void);

                void Add(Animation *_animation);
                Animation *Get(std::string name);
                void SetBindPose(Skeleton *_bind_pose_skeleton);
                Skeleton *GetBindPose(void);
                std::vector<std::string> GetAnimationNames(void);
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
                std::map<unsigned int, std::pair<float, unsigned int> > position_state;
                std::map<unsigned int, std::pair<float, unsigned int> > rotation_state;
                std::map<unsigned int, std::pair<float, unsigned int> > scaling_state;
                Animations *animations;
                float animation_time;
                std::map<unsigned int, glm::mat4> transforms;
        };
    }
}

#endif
