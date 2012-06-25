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
        void Skeleton::AddBone(Bone *bone) {
            bones.push_back(bone);
        }

        KeyFrame::KeyFrame(float _frame_time, unsigned int _index, Skeleton *_skeleton){
            skeleton = _skeleton;
            index = _index;
            frame_time = _frame_time;
        }
        KeyFrame::~KeyFrame(void){
            //
        }
        void KeyFrame::SetFrameTime(float _frame_time){
            frame_time = _frame_time;
        }
        void KeyFrame::SetIndex(float _index){
            index = _index;
        }
        void KeyFrame::SetSkeleton(Skeleton *_skeleton){
            skeleton = _skeleton;
        }
        unsigned int KeyFrame::GetIndex(void){
            return index;
        }
        float KeyFrame::GetFrameTime(void){
            return frame_time;
        }
        Skeleton *KeyFrame::GetSkeleton(void){
            return skeleton;
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

        Animation::Animation(std::string animation_name, float _duration, KeyFrame *_frames, unsigned int _frame_count){
            duration = _duration;
            name = animation_name;
            frames = _frames;
            frame_count = _frame_count;
        }
        Animation::~Animation(void){
            //
        }
        float Animation::GetDuration(void){
            return duration;
        }
        std::string Animation::GetName(void){
            return name;
        }
        KeyFrame *Animation::GetFrames(void){
            return frames;
        }
        unsigned int Animation::GetFrameCount(void) {
            return frame_count;
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

        AnimationState::AnimationState(Animations *_animations){
            current_animation = "";
            animations = _animations;
            animation_time = 0.0f;
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
            //std::cout << "Frame Time Animations: " << frame_time << std::endl;
            EG::Dynamics::Animation *animation = animations->Get(current_animation);
            //std::cout << "Current Animation: " << current_animation << std::endl;

            float animation_duration = animation->GetDuration();
            //std::cout << "Animation Duration: " << animation_duration << std::endl;
            animation_time += frame_time;
            if (animation_time > animation_duration) {
                animation_time = glm::mod(animation_time, animation_duration);
            }

            unsigned int frame_count = animation->GetFrameCount();
            EG::Dynamics::KeyFrame *key_frames = animation->GetFrames();
            float frame_time_sum = 0.0f;
            unsigned int frame_index = 0;
            for (frame_index = 0; frame_index < frame_count; frame_index++) {
                frame_time_sum = key_frames[frame_index].GetFrameTime();
                if (animation_time < frame_time_sum) {
                    break;
                }
            }
            unsigned int previous_frame_index = frame_index - 1;
            if (frame_index == 0) {
                previous_frame_index = frame_count - 1;
            }
            //std::cout << "Animation Time: " << animation_time << std::endl;
            //std::cout << "Frame Index: " << frame_index << " / " << frame_count << std::endl;

            EG::Dynamics::Skeleton *post_frame = key_frames[frame_index].GetSkeleton();
            EG::Dynamics::Skeleton *prev_frame = key_frames[previous_frame_index].GetSkeleton();

            float previous_frame_sum = key_frames[previous_frame_index].GetFrameTime();
            float temp_animation_time = animation_time - previous_frame_sum;
            float interpolation_factor = temp_animation_time / (key_frames[frame_index].GetFrameTime() - key_frames[previous_frame_index].GetFrameTime());
//             std::cout << "Interpolation Factor: " << interpolation_factor << std::endl;

            glm::mat4 bind_trans = glm::mat4(1.0f);
            glm::mat4 start_trans = glm::mat4(1.0f);
            glm::mat4 end_trans = glm::mat4(1.0f);
            Interpolate(bind_trans, animations->GetBindPose()->GetRoot(), start_trans, prev_frame->GetRoot(), end_trans, post_frame->GetRoot(), interpolation_factor);
        }
        void AnimationState::Interpolate(glm::mat4 &bind_trans, Bone *bone, glm::mat4 &start_trans, Bone *start, glm::mat4 &end_trans, Bone *end, float i) {
            bind_trans *= bone->GetOffset();
            start_trans *= start->GetOffset();
            const glm::mat4 tmp_start_trans = bind_trans * start_trans;
            end_trans *= end->GetOffset();
            const glm::mat4 tmp_end_trans = bind_trans * end_trans;
            transforms[bone->GetId()] = EG::Math::Utility::Interpolate(tmp_start_trans, tmp_end_trans, i);//glm::interpolate(tmp_start_trans, tmp_end_trans, i);

            std::vector<Bone *> *bones = bone->GetChildren();
            std::vector<Bone *> *start_bones = start->GetChildren();
            std::vector<Bone *> *end_bones = end->GetChildren();
            for (unsigned int bone_index = 0; bone_index < bones->size(); bone_index++) {
                Interpolate(bind_trans, (*bones)[bone_index], start_trans, (*start_bones)[bone_index], end_trans, (*end_bones)[bone_index], i);
            }
        }
        std::vector<glm::mat4> AnimationState::GetTransforms(void) {
            unsigned int size = transforms.size();
            std::vector<glm::mat4> out;
            for (unsigned int i = 0; i < size; i++) {
                out.push_back(transforms[i]);
            }
            //std::cout << "Transform Count: " << out.size() << std::endl;
            return out;
        }
    }
}
