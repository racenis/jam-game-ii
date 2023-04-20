#ifndef JAM_GAME_II_ENTITIES_CRAB_H
#define JAM_GAME_II_ENTITIES_CRAB_H

#include <framework/entity.h>
#include <components/rendercomponent.h>
#include <components/armaturecomponent.h>
#include <framework/serializeddata.h>

#include "../mongus.h"

using namespace tram;

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
        
        if (mongus_distance < 10.0f && state == CRABSTATE_NONE) {
            state = CRABSTATE_SNIBETI_SNAB;
            
            SetPlaying (SOUND_SNIBETISNAB, true);
        }
        
        if (mongus_distance > 15.0f && state == CRABSTATE_SNIBETI_SNAB) {
            state = CRABSTATE_NONE;
            
            SetPlaying (SOUND_SNIBETISNAB, false);
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
            
            quat new_rot = glm::mix(rot_target, parent->GetRotation(), 0.7f);
            vec3 new_pos = parent->GetLocation() + velocity;
            
            // make crab go in player direction
            float speed = 0.05f - glm::length(velocity);
            float clip_speed = speed < 0.0f ? 0.0f : speed;
            
            velocity += new_rot * DIRECTION_FORWARD * clip_speed;
            
            
            bool hit_wall = false;
            
            // check if the crab is inside a wall
            triggercomponent->SetCollisionMask(Physics::COLL_WORLDOBJ);
            triggercomponent->SetLocation(parent->GetLocation() + vec3(0.0f, 0.5f, 0.0f));
            auto poll = triggercomponent->Poll();
            if (poll.size()) {
                hit_wall = true;
                ticks_since_in_wall++;
            } else {
                ticks_since_in_wall = 0;
            }
            
            // check if the crab is inside the player
            triggercomponent->SetCollisionMask(Physics::COLL_PLAYER);
            for (auto& coll: triggercomponent->Poll()) {
                if (!coll.collider) continue;
                auto ent = coll.collider->GetParent();
                if (!ent) continue;
                Message::Send({
                    .type = 420,
                    .receiver = ent->GetID(),
                    .sender = parent->GetID()
                });
            }
            
            // I don't remember why this is here, but I won't remove it, just in case
            triggercomponent->SetLocation(parent->GetLocation() + vec3(0.0f, 2.0f, 0.0f));
            
            // raycast to the ground
            auto ground = Physics::Raycast(parent->GetLocation() + vec3(0.0f, 1.0f, 0.0f), parent->GetLocation() - vec3(0.0f, 0.1f, 0.0f), Physics::COLL_WORLDOBJ);
            
            bool on_ground = ground.collider;
            
            // check if crab is in air, or standing on the ground
            if (!on_ground || velocity.y > 0.0f) { 
                velocity.y -= 0.001f;
            } else {
                velocity.y = 0.0f;
                velocity *= 0.78f;
                
                if (!hit_wall) {
                    new_pos.y = ground.point.y;
                }
            }
            
            // try to dislodge crab if stuck in wall
            if (ticks_since_in_wall > 60) {
                velocity = {0.15f, 0.2f, 0.15f};
                ticks_since_in_wall = 0;
            }
            
            if (!hit_wall) {
                parent->UpdateTransform(new_pos, new_rot);
            }
            
            //Render::AddLine(parent->GetLocation(), parent->GetLocation() + direction, Render::COLOR_CYAN);
            
            if (!armaturecomponent->IsPlayingAnimation("krabis-walk")) {
                armaturecomponent->PlayAnimation("krabis-walk", 1000.0f, 1.0f, 8.0f);
            }
            
            if (!armaturecomponent->IsPlayingAnimation("krabis-snibetisnab")) {
                armaturecomponent->PlayAnimation("krabis-snibetisnab", 1000.0f, 1.0f, 2.0f);
            }
            
            SetPosition (SOUND_SNIBETISNAB, new_pos);
        }
        
        if (state == CRABSTATE_OWEE) {
            vec3 pos = parent->GetLocation();
            quat rot = parent->GetRotation();
            
            parent->UpdateTransform(pos + velocity, rot);
            
            velocity.y -= 0.001f;
            
            SetPosition (SOUND_SNIBETISNAB, pos);
            
            if (Physics::Raycast(pos + vec3(0.0f, 1.0f, 0.0f), pos - vec3(0.0f, 0.1f, 0.0f), Physics::COLL_WORLDOBJ).collider) {
                velocity = {0.0f, 0.0f, 0.0f};
                state = CRABSTATE_NONE;
                
                SetPlaying (SOUND_SNIBETISNAB, false);
            }
        }
        
        if (parent->GetLocation().y < -10.0f) {
            std::cout << "unloading the crab " << std::endl;
            SetPlaying (SOUND_SNIBETISNAB, false);
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
    
    void YeetIntoAir() {
        state = CRABSTATE_OWEE;
        
        velocity = {0.15f, 0.2f, 0.15f};
        
        vec3 pos = parent->GetLocation();
        quat rot = parent->GetRotation();
        
        parent->UpdateTransform(pos + vec3(0.0f, 0.3f, 0.0f), rot);
    }
    
    CrabState state = CRABSTATE_NONE;
    
    uint32_t ticks_since_in_wall = 0;
    
    vec3 velocity = {0.0f, 0.0f, 0.0f};
    
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
    Component<PhysicsComponent> physicscomponent;
    Component<CrabComponent> crabcomponent;
    SerializedData<Data> serializeddata;
};


#endif // JAM_GAME_II_ENTITIES_CRAB_H