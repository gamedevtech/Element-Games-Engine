#include "../Animation.h"

#include <iostream>

namespace EG{
    namespace Dynamics{
        Bone::Bone(unsigned int _id, glm::mat4 _offset){
            id = _id;
            offset = _offset;
            parent = NULL;
        }
        Bone::~Bone(void){
            //
        }
        unsigned int Bone::GetId(void){
            return id;
        }
        glm::mat4 Bone::GetOffset(void){
            return offset;
        }
        void Bone::SetParent(Bone *_parent){
            parent = _parent;
        }
        void Bone::AddChild(Bone *_child){
            _child->SetParent(this);
            children.push_back(_child);
        }
        Bone *Bone::GetParent(void) {
            return parent;
        }
        std::vector<Bone *> *Bone::GetChildren(void) {
            return &children;
        }

        Skeleton::Skeleton(void){
            root = NULL;
        }
        Skeleton::~Skeleton(void){
            //
        }
        void Skeleton::SetRoot(Bone *_root) {
            root = _root;
        }
        Bone *Skeleton::GetRoot(void){
            return root;
        }
        std::vector<Bone *> *Skeleton::GetBones(void){
            return &bones;
        }
        std::map<unsigned int, Bone *> *Skeleton::GetBoneMap(void) {
            return &bone_map;
        }
        void Skeleton::AddBone(Bone *bone) {
            bones.push_back(bone);
            bone_map[bone->GetId()] = bone;
        }
        void Skeleton::PrintRecursive(Bone *b) {
            if (b == NULL) {
                print_level = 0;
                PrintRecursive(root);
            } else {
                for (unsigned int i = 0; i < print_level; i++) {
                    std::cout << '\t';
                }
                std::cout << "Bone: " << b->GetId() << ' ' << b->GetChildren()->size() << std::endl;
                std::vector<Bone *>::iterator bi = b->GetChildren()->begin();
                while (bi != b->GetChildren()->end()) {
                    PrintRecursive((*bi));
                    print_level += 1;
                    ++bi;
                }
            }
        }
        Bone *Skeleton::GetBone(unsigned int bone_id) {
            return bone_map[bone_id];
        }
        
        Animation::Animation(std::string animation_name, float _duration) {
            name = animation_name;
            duration = _duration;
        }
        Animation::~Animation(void) {
            //
        }

        float Animation::GetDuration(void) {
            return duration;
        }
        std::string Animation::GetName(void) {
            return name;
        }
        std::vector<glm::mat4> Animation::GetTransforms(float time_stamp, std::map<unsigned int, std::pair<float, unsigned int> > *position_state, std::map<unsigned int, std::pair<float, unsigned int> > *scaling_state, std::map<unsigned int, std::pair<float, unsigned int> > *rotation_state) {
            std::vector<glm::mat4> out;
            for (unsigned int bone_index = 0; bone_index < bone_count; bone_index++) {
                glm::vec3 position;
                glm::vec3 scaling;
                glm::quat rotation;

                unsigned int i = (*position_state)[bone_index].second;
                if ((*position_state)[bone_index].first > time_stamp) {
                    i = 0;
                }
                for (i; i <= positions[bone_index].size(); i++) {
                    if (time_stamp < positions[bone_index][i].first) {
                        if (i == 0) {
                            position = positions[bone_index][i].second;
                        } else {
                            float diff = positions[bone_index][i].first - positions[bone_index][i - 1].first;
                            float factor = (time_stamp - positions[bone_index][i - 1].first) / diff;
                            position = glm::mix(positions[bone_index][i - 1].second, positions[bone_index][i].second, factor);
                        }
                        break;
                    }
                }
                (*position_state)[bone_index] = std::pair<float, unsigned int>(time_stamp, i);

                i = (*scaling_state)[bone_index].second;
                if ((*scaling_state)[bone_index].first > time_stamp) {
                    i = 0;
                }
                for (i; i < scalings[bone_index].size(); i++) {
                    if (time_stamp < scalings[bone_index][i].first) {
                        if (i == 0) {
                            scaling = scalings[bone_index][i].second;
                        } else {
                            float diff = scalings[bone_index][i].first - scalings[bone_index][i - 1].first;
                            float factor = (time_stamp - scalings[bone_index][i - 1].first) / diff;
                            scaling = glm::mix(scalings[bone_index][i - 1].second, scalings[bone_index][i].second, factor);
                        }
                        break;
                    }
                }
                (*scaling_state)[bone_index] = std::pair<float, unsigned int>(time_stamp, i);

                i = (*rotation_state)[bone_index].second;
                if ((*rotation_state)[bone_index].first > time_stamp) {
                    i = 0;
                }
                for (i; i < rotations[bone_index].size(); i++) {
                    if (time_stamp < rotations[bone_index][i].first) {
//                         if (i == 0) {
                            rotation = rotations[bone_index][i].second;
//                         } else {
//                             float diff = rotations[bone_index][i].first - rotations[bone_index][i - 1].first;
//                             float factor = (time_stamp - rotations[bone_index][i - 1].first) / diff;
//                             rotation = glm::mix(rotations[bone_index][i - 1].second, rotations[bone_index][i].second, factor);
//                         }
                        break;
                    }
                }
                (*rotation_state)[bone_index] = std::pair<float, unsigned int>(time_stamp, i);

                glm::mat4 mat = EG::Math::Utility::GenerateTransform(position, scaling, rotation);
                out.push_back(mat);
            }
            return out;
        }
        unsigned int Animation::GetBoneCount(void) {
            return bone_count;
        }

        void Animation::SetBoneCount(unsigned int _bone_count) {
            bone_count = _bone_count;
        }
        void Animation::AddBonePosition(unsigned int bone_id, float time, glm::vec3 position) {
            positions[bone_id].push_back(std::pair<float, glm::vec3>(time, position));
        }
        void Animation::AddBoneScaling(unsigned int bone_id, float time, glm::vec3 scaling) {
            scalings[bone_id].push_back(std::pair<float, glm::vec3>(time, scaling));
        }
        void Animation::AddBoneRotation(unsigned int bone_id, float time, glm::quat rotation) {
            rotations[bone_id].push_back(std::pair<float, glm::quat>(time, rotation));
        }
        std::map<unsigned int, std::vector<std::pair<float, glm::vec3> > > *Animation::GetPositions(void) {
            return &positions;
        }
        std::map<unsigned int, std::vector<std::pair<float, glm::vec3> > > *Animation::GetScalings(void) {
            return &scalings;
        }
        std::map<unsigned int, std::vector<std::pair<float, glm::quat> > > *Animation::GetRotations(void) {
            return &rotations;
        }

        Animations::Animations(void){
            //
        }
        Animations::~Animations(void){
            //
        }
        void Animations::Add(Animation *_animation){
            animations[_animation->GetName()] = _animation;
        }
        Animation *Animations::Get(std::string name){
            return animations[name];
        }
        void Animations::SetBindPose(Skeleton *_bind_pose_skeleton){
            bind_pose = _bind_pose_skeleton;
        }
        Skeleton *Animations::GetBindPose(void){
            return bind_pose;
        }
        std::vector<std::string> Animations::GetAnimationNames(void) {
            std::vector<std::string> out;
            std::map<std::string, Animation *>::iterator anim_iter = animations.begin();
            while (anim_iter != animations.end()) {
                out.push_back(anim_iter->first);
                ++anim_iter;
            }
            return out;
        }

        AnimationState::AnimationState(Animations *_animations){
            current_animation = "";
            animations = _animations;
            animation_time = 0.0f;
            unsigned int bone_count = animations->GetBindPose()->GetBones()->size();
            for (unsigned int i = 0; i < bone_count; i++) {
                position_state[i] = std::pair<float, unsigned int>(0.0f, 0);
                scaling_state[i] = std::pair<float, unsigned int>(0.0f, 0);
                rotation_state[i] = std::pair<float, unsigned int>(0.0f, 0);
            }
        }
        AnimationState::~AnimationState(void){
            //
        }
        void AnimationState::SetAnimation(std::string animation){
            current_animation = animation;
        }
        std::string AnimationState::GetAnimation(void){
            return current_animation;
        }
        Animations *AnimationState::GetAnimations(void){
            return animations;
        }
        void AnimationState::Update(float frame_time){
            EG::Dynamics::Animation *animation = animations->Get(current_animation);
            float animation_duration = animation->GetDuration();
            animation_time += frame_time;
            if (animation_time > animation_duration) {
                animation_time = glm::mod(animation_time, animation_duration);
            }
            // Traverse Tree and Multiply Aptly, as well as apply Bind Pose Properly
            std::vector<glm::mat4> unmultiplied_transforms = animation->GetTransforms(animation_time, &position_state, &scaling_state, &rotation_state);
            for (unsigned int i = 0; i < unmultiplied_transforms.size(); i++) {
                Bone *b = animations->GetBindPose()->GetBone(i);
                glm::mat4 offset = b->GetOffset();
                glm::mat4 result = glm::mat4(1.0f);
                std::vector<glm::mat4> stack;
                while (b) {
                    result = unmultiplied_transforms[b->GetId()] * result;
                    b = b->GetParent();
                }
                transforms[i] = result * offset;
            }
        }
        std::vector<glm::mat4> AnimationState::GetTransforms(void) {
            std::vector<glm::mat4> out;
            for (unsigned int i = 0; i < transforms.size(); i++) {
                out.push_back(transforms[i]);
            }
            return out;
        }
    }
}
