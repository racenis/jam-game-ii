#ifndef JAM_GAME_II_MONGUS_H
#define JAM_GAME_II_MONGUS_H

#include <framework/entitycomponent.h>
#include <framework/entity.h>
#include <framework/event.h>
#include <framework/ui.h>

#include <components/rendercomponent.h>
#include <components/triggercomponent.h>
#include <components/armaturecomponent.h>

using namespace Core;

class Mongus;
extern Mongus* MAIN_MONGUS;

class MongusComponent : public EntityComponent {
    //MongusComponent() = default;
    //~MongusComponent() = default;
public:
    MongusComponent() : keydown (this), keyup (this) {}
    
    void Start() {
        keydown.make();
        keyup.make();
    }
    
    void EventHandler (Event &event) {
        if (event.type == Event::KEYDOWN || event.type == Event::KEYUP) {
            char dir = event.type == Event::KEYDOWN ? 1 : -1;
            
            switch (event.subtype) {
                case UI::KEY_ACTION_FORWARD: movdir_forward += dir; break;
                case UI::KEY_ACTION_BACKWARD: movdir_forward -= dir; break;
                case UI::KEY_ACTION_LEFT: movdir_side -= dir; break;
                case UI::KEY_ACTION_RIGHT: movdir_side += dir; break;
            }
            
            fall = true;
            //direction = glm::length(direction) > 0.0f ? glm::normalize(direction) : vec3(0.0f, 0.0f, 0.0f);
        }
        
        if (event.type == Event::KEYDOWN && event.subtype == UI::KEY_ACTION_JUMP) {
            jump = true;
        }
        
    }
    
    void Move() {
        if (is_paused) return;
        
        bool is_run = movdir_forward || movdir_side;
        
        vec3 direction_forward = glm::normalize(Render::CAMERA_ROTATION * DIRECTION_FORWARD);
        vec3 direction_side = glm::normalize(Render::CAMERA_ROTATION * DIRECTION_SIDE);
        
        direction_forward.y = 0.0f;
        direction_side.y = 0.0f;
        
        direction_target = direction_forward * (float) movdir_forward;
        direction_target += direction_side * (float) movdir_side;
        
        direction_target = glm::normalize(direction_target);
        
        if (std::isnan(direction_target.x), std::isnan(direction_target.y), std::isnan(direction_target.z)) {
            direction_target = direction;
        }
        
        direction = glm::mix(direction, direction_target, 0.1f);
        
        if (is_run) {
            float speed = glm::length(velocity);
            float add_speed = (is_in_air ? 0.04f : 0.08f) - speed;
            float turn_speed = add_speed * glm::dot(direction, direction_target);
            float clip_speed = add_speed < 0.0f ? 0.0f : turn_speed;
            
            velocity += direction * clip_speed;
        }
        
        
        
        /*Render::AddLine(parent->GetLocation(), parent->GetLocation() + direction, Render::COLOR_BLUE);*/
        
        vec3 new_pos = parent->GetLocation() + velocity;
        quat new_rot = vec3(0.0f, 3.14f + (atan(direction.x/direction.z) - (direction.z < 0.0f ? 3.14f : 0.0f)), 0.0f);
        
        vec3 in_pos = parent->GetLocation() + vec3(0.0f, 1.0f, 0.0f);
        vec3 ground_pos = parent->GetLocation() - vec3(0.0f, 0.02f, 0.0f);
        
        auto collision = Physics::Raycast(in_pos, ground_pos);
        
        if (collision.collider) {
            new_pos.y = collision.point.y;
            velocity.y = 0.0f;
            //velocity *= 0.89f;
            velocity *= 0.79f;
            
            is_in_air = false;
            
            if (jump && ticks_until_jump < 0) {
                jump = false;
                
                //armature_comp->PlayAnimation("mongus-jump", 1, 1.0f, 4.0f);
                velocity.y = 0.15f;
                new_pos.y += 0.05f;
            }
        } else {
            if (fall) {
                velocity.y -= 0.005f;
            }
            
            is_in_air = true;
            Render::AddLineMarker(parent->GetLocation() + vec3(0.0f, 1.0f, 0.0f), Render::COLOR_CYAN);
        }
        
        /*Render::AddLineMarker(collision.point, Render::COLOR_RED);*/
        
        trigger_comp->SetLocation(new_pos + vec3(0.0f, 1.2f, 0.0f));
        if (trigger_comp->Poll().size()) {
            vec3 average_normal = vec3(0.0f, 0.0f, 0.0f);
            auto poll = trigger_comp->Poll();
            for (auto& coll : poll) {
                average_normal += coll.normal;
            }
            average_normal /= poll.size();
            
            auto v_dir = glm::normalize(velocity);
            auto s_dir = glm::normalize(average_normal);

            auto n_dir = glm::normalize(v_dir - s_dir);

            auto n_vel = n_dir * glm::length(velocity) * glm::dot(v_dir, n_dir);

            if (std::isnan(n_vel.x) || std::isnan(n_vel.z)) n_vel = vec3(0.0f, 0.0f, 0.0f);
            
            velocity = n_vel;
            
            new_pos = parent->GetLocation() + velocity;
        }
        
        parent->UpdateTransform(new_pos, new_rot);
        
        
        
        //std::cout << movdir_forward << " " << movdir_side << std::endl;
        
        if (!armature_comp->IsPlayingAnimation("mongus-wag-tail")) {
            armature_comp->PlayAnimation("mongus-wag-tail", 100, 1.0f, 1.0f);
        }
        
        if (ticks_since_sway > 300 && !is_run) {
            armature_comp->PlayAnimation("mongus-sway", 1, 1.0f, 1.0f);
            ticks_since_sway = 0;
        }
        
        if (is_run && !was_run) {
            armature_comp->PlayAnimation("mongus-run", 100, 1.0f, 2.0f);
            armature_comp->FadeAnimation("mongus-run", true, 0.1f);
            
            if (armature_comp->IsPlayingAnimation("mongus-sway")) {
                armature_comp->FadeAnimation("mongus-sway", false, 0.1f);
            }
        }
        
        if (!is_run && was_run) {
            armature_comp->FadeAnimation("mongus-run", false, 0.025f);
        }
        
        
        auto shadow = Physics::Raycast(in_pos, in_pos - vec3(0.0f, 5.0f, 0.0f));
        
        if (shadow.collider) {
            Render::AddLineMarker(shadow.point, Render::COLOR_GREEN);
        }
        
        was_run = is_run;
        ticks_since_sway++;
        ticks_until_jump--;
    }
    
    static void Update() {
        for (auto& m : PoolProxy<MongusComponent>::GetPool()) {
            m.Move();
        }
    }
    
    void SetArmatureComponent (ArmatureComponent* armature_comp) {
        this->armature_comp = armature_comp;
    }
    
    void SetTriggerComponent (TriggerComponent* trigger_comp) {
        this->trigger_comp = trigger_comp;
    }
    
    ArmatureComponent* armature_comp = nullptr;
    TriggerComponent* trigger_comp = nullptr;
    
    char movdir_forward = 0; char movdir_side = 0;
    
    int32_t ticks_since_sway = 0;
    int32_t ticks_until_jump = -1;
    
    bool was_run = false;
    
    bool is_paused = false;
    
    bool jump = false;
    bool fall = false;
    
    bool is_in_air = false;
    
    vec3 direction = DIRECTION_FORWARD;
    vec3 direction_target = DIRECTION_FORWARD;
    
    vec3 velocity = {0.0f, 0.0f, 0.0f};
    
    EventListener<Event::KEYDOWN, MongusComponent> keydown;
    EventListener<Event::KEYUP, MongusComponent> keyup;
};

class Mongus : public Entity {
public:
    void Load() {
        mongus_comp.make();
        render_comp.make();
        physics_comp.make();
        trigger_comp.make();
        armature_comp.make();
        
        mongus_comp->SetParent(this);
        mongus_comp->Init();
        
        armature_comp->SetParent(this);
        armature_comp->SetModel("mongus");
        armature_comp->Init();
        
        render_comp->SetParent(this);
        render_comp->SetModel("mongus");
        render_comp->SetPose(armature_comp->GetPosePtr());
        render_comp->Init();
        
        //physics_comp->SetParent(this);
        physics_comp->SetKinematic();
        physics_comp->SetCollisionGroup(Physics::COLL_PLAYER);
        physics_comp->SetShape(Physics::CollisionShape::Cylinder(0.5, 0.5f));
        physics_comp->Init();
        
        trigger_comp->SetShape(Physics::CollisionShape::Capsule(0.4, 0.2f));
        //rigger_comp->SetCollisionGroup(Physics::COLL_PLAYER);
        trigger_comp->SetCollisionMask(-1 ^ Physics::COLL_PLAYER);
        trigger_comp->Init();
        
        mongus_comp->SetArmatureComponent(armature_comp.get());
        mongus_comp->SetTriggerComponent(trigger_comp.get());
        
        this->isloaded = true;
    }

    void Unload() {
        assert(false);
    }

    void Serialize() {
        assert(false);
    }

    void UpdateParameters() {
        render_comp->UpdateLocation(location);
        render_comp->UpdateRotation(rotation);
        physics_comp->SetLocation(location + vec3(0.0f, 1.6f, 0.0f));
    }
    
    void SetParameters() {
        assert(isloaded);
        UpdateParameters();
    }

    void MessageHandler(Message& msg) {
        
    }
    
    void MongusPause() {
        mongus_comp->fall = false;
        mongus_comp->velocity = {0.0f, 0.0f, 0.0f};
        mongus_comp->is_paused = true;
    }
    
    void MongusResume() {
        mongus_comp->is_paused = false;
    }
    
private:
    Component<MongusComponent> mongus_comp;
    Component<RenderComponent> render_comp;
    Component<PhysicsComponent> physics_comp;
    Component<TriggerComponent> trigger_comp;
    Component<ArmatureComponent> armature_comp;
};

#endif // JAM_GAME_II_MONGUS_H