#ifndef JAM_GAME_II_ENTITIES_CRAB_H
#define JAM_GAME_II_ENTITIES_CRAB_H

#include <framework/entity.h>
#include <components/rendercomponent.h>
#include <components/armaturecomponent.h>
#include <framework/serializeddata.h>

#include "../mongus.h"

using namespace Core;

enum CrabState {
    CRABSTATE_NONE,
    CRABSTATE_SNIBETI_SNAB,
    CRABSTATE_OWEE
};

class CrabComponent : public EntityComponent {
    //CrabComponent() = default;
    //~CrabComponent() = default;
public:
    CrabComponent() {}
    
    void Start() {
        
    }
    
    void EventHandler (Event &event) {

    }
    
    void Move() {
        float mongus_distance = glm::distance(MAIN_MONGUS->GetLocation(), parent->GetLocation());
        
        if (mongus_distance < 10.0f) {
            state = CRABSTATE_SNIBETI_SNAB;
        }
        
        if (mongus_distance > 15.0f) {
            state = CRABSTATE_NONE;
        }
        
        if (state == CRABSTATE_NONE) {
            if (armaturecomponent->IsPlayingAnimation("krabis-walk")) {
                armaturecomponent->StopAnimation("krabis-walk");
            }
            
            if (armaturecomponent->IsPlayingAnimation("krabis-snibetisnab")) {
                armaturecomponent->StopAnimation("krabis-snibetisnab");
            }
        }
        
        if (state == CRABSTATE_SNIBETI_SNAB) {
            vec3 direction = glm::normalize(MAIN_MONGUS->GetLocation() - parent->GetLocation());
            quat rot_target = vec3(0.0f, 3.14f + (atan(direction.x/direction.z) - (direction.z < 0.0f ? 3.14f : 0.0f)), 0.0f);
            
            quat new_rot = glm::mix(rot_target, parent->GetRotation(), 0.9f); 
            vec3 new_pos = parent->GetLocation() + new_rot * DIRECTION_FORWARD * 0.05f;
            
            bool hit_wall = false;
            vec3 wall_pos;
            
            triggercomponent->SetLocation(parent->GetLocation() + vec3(0.0f, 0.5f, 0.0f));
            auto poll = triggercomponent->Poll();
            if (poll.size()) {
                new_pos = parent->GetLocation();
                wall_pos = poll[0].point;
                hit_wall = true;
            }
            
            triggercomponent->SetLocation(parent->GetLocation() + vec3(0.0f, 2.0f, 0.0f));
            auto ground = Physics::Raycast(parent->GetLocation() + vec3(0.0f, 1.0f, 0.0f), parent->GetLocation() - vec3(0.0f, 0.1f, 0.0f));
            
            if (!ground.collider && !hit_wall) { 
                fall_velocity += 0.001f;
                new_pos += vec3(0.0f,-1.0f,0.0f) * fall_velocity;
            } else {
                fall_velocity = 0.0f;
                
                if (!hit_wall) {
                    new_pos.y = ground.point.y;
                } else {
                    new_pos.y = wall_pos.y;
                }
            }
            
            parent->UpdateTransform(new_pos, new_rot);
            
            Render::AddLine(parent->GetLocation(), parent->GetLocation() + direction, Render::COLOR_CYAN);
            
            if (!armaturecomponent->IsPlayingAnimation("krabis-walk")) {
                armaturecomponent->PlayAnimation("krabis-walk", 1000.0f, 1.0f, 8.0f);
            }
            
            if (!armaturecomponent->IsPlayingAnimation("krabis-snibetisnab")) {
                armaturecomponent->PlayAnimation("krabis-snibetisnab", 1000.0f, 1.0f, 2.0f);
            }
        }
        
        if (parent->GetLocation().y < -10.0f) {
            parent->Unload();
        }
    }
    
    static void Update() {
        for (auto& m : PoolProxy<CrabComponent>::GetPool()) {
            m.Move();
        }
    }
    
    void SetTriggerComponent(TriggerComponent* triggercomponent) {
        this->triggercomponent = triggercomponent;
    }
    
    void SetArmatureComponent(ArmatureComponent* armaturecomponent) {
        this->armaturecomponent = armaturecomponent;
    }
    
    CrabState state = CRABSTATE_NONE;
    float fall_velocity = 0.0f;
    
    TriggerComponent* triggercomponent = nullptr;
    ArmatureComponent* armaturecomponent = nullptr;
};

class Crab : public Entity {
public:
    Crab(std::string_view& str);

    void UpdateParameters();
    void SetParameters();
    void Load();
    void Unload();
    void Serialize();
    void MessageHandler(Message& msg);

    class Data: public SerializedEntityData {
    public:
        Field<name_t> model;

        void ToString(std::string& str) {
            model.ToString(str);
        }

        void FromString(std::string_view& str) {
            model.FromString(str);
        }
        
        std::vector<FieldInfo> GetFieldInfo() {
            return std::vector<FieldInfo> {
                 { FieldInfo::FIELD_MODELNAME, "model" },
            };
        }
        
        char const* GetType() {
            return "crab";
        }
    };
protected:
    Component<RenderComponent> rendercomponent;
    Component<ArmatureComponent> armaturecomponent;
    Component<TriggerComponent> triggercomponent;
    Component<CrabComponent> crabcomponent;
    SerializedData<Data> serializeddata;
};


#endif // JAM_GAME_II_ENTITIES_CRAB_H