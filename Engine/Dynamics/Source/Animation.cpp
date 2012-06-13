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

        KeyFrame::KeyFrame(float _duration, unsigned int _index, Skeleton *_skeleton){
            skeleton = _skeleton;
            index = _index;
            duration = _duration;
        }
        KeyFrame::~KeyFrame(void){
            //
        }
        void KeyFrame::SetDuration(float _duration){
            duration = _duration;
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
        float KeyFrame::GetDuration(void){
            return duration;
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

        Animation::Animation(std::string animation_name, float _duration, KeyFrame *_frames){
            duration = _duration;
            name = animation_name;
            frames = _frames;
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

        AnimationState::AnimationState(Animations *_animations){
            current_animation = "default";
            animations = _animations;
        }
        AnimationState::~AnimationState(void){
            //
        }
        void AnimationState::SetAnimation(std::string animation){
            current_animation = animation;
        }
    }
}
