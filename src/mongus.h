#ifndef JAM_GAME_II_MONGUS_H
#define JAM_GAME_II_MONGUS_H

#include <framework/entitycomponent.h>
#include <framework/entity.h>
#include <framework/event.h>
#include <framework/message.h>
#include <framework/ui.h>

#include <components/rendercomponent.h>
#include <components/triggercomponent.h>
#include <components/armaturecomponent.h>

#include "levelswitch.h"

#include <cmath>

using namespace tram;

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
                case UI::KEY_ACTION_STRAFE_LEFT: movdir_side -= dir; break;
                case UI::KEY_ACTION_STRAFE_RIGHT: movdir_side += dir; break;
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
        
        // calculate movement velocity based on inputs
        bool is_run = movdir_forward || movdir_side;
        
        vec3 direction_forward = glm::normalize(Render::GetCameraRotation(0) * DIRECTION_FORWARD);
        vec3 direction_side = glm::normalize(Render::GetCameraRotation(0) * DIRECTION_SIDE);
        
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
        
        vec3 old_pos = parent->GetLocation();
        
        vec3 new_pos = old_pos + velocity;
        quat new_rot = vec3(0.0f, 3.14f + (atan(direction.x/direction.z) - (direction.z < 0.0f ? 3.14f : 0.0f)), 0.0f);
        
        
        // calculate collision
        vec3 in_pos = old_pos + vec3(0.0f, 1.0f, 0.0f);
        vec3 ground_pos = old_pos - vec3(0.0f, velocity.y > 0.0f ? 0.01f : 0.1f, 0.0f);
 
        auto collision = Physics::Raycast(in_pos, ground_pos, Physics::COLL_WORLDOBJ);
        
        float ground_height = collision.point.y;
        bool is_on_ground = collision.collider;
        
        if (is_on_ground && ticks_since_jump > 3) {
            //new_pos.y = collision.point.y;
            velocity.y = 0.0f;
            velocity *= 0.79f;
            
            is_in_air = false;
            ticks_since_left_ground = 0;
        } else {
            if (fall) {
                velocity.y -= 0.005f;
            }
            
            is_in_air = true;
        }
        
        if (jump && ticks_since_left_ground < 30) {
            ticks_since_left_ground = 100;
            //armature_comp->PlayAnimation("mongus-jump", 1, 1.0f, 4.0f);
            velocity.y = 0.15f;
            ticks_since_jump = 0;
            //new_pos.y += 0.05f;
        }
        
        jump = false;
        
        vec3 trigger_pos = old_pos + vec3(0.0f, 1.0f, 0.0f);
        
        trigger_comp->SetLocation(trigger_pos + velocity);
        auto wall_collisions = trigger_comp->Poll();
        
        for (size_t i = 0; i < 3 && wall_collisions.size(); i++) {            
            Render::AddLineMarker(wall_collisions[0].point, Render::COLOR_WHITE);
            
            vec3 average_normal = wall_collisions[0].normal;
            
            auto v_dir = glm::normalize(velocity);
            auto s_dir = glm::normalize(average_normal);

            auto n_dir = glm::normalize(v_dir - s_dir);

            auto n_vel = n_dir * glm::length(velocity) * glm::dot(v_dir, n_dir);

            if (std::isnan(n_vel.x) || std::isnan(n_vel.z)) n_vel = vec3(0.0f, 0.0f, 0.0f);
            
            velocity = n_vel;
            
            trigger_comp->SetLocation(trigger_pos + velocity);
            wall_collisions = trigger_comp->Poll();
        }
        
        if (wall_collisions.size()) {
            velocity = {0.0f, 0.0f, 0.0f};
        }
        
        new_pos = old_pos + velocity;
        
        // if mongus is inside ground
        if (is_on_ground && new_pos.y < ground_height) {
            float push_ammount = ground_height - new_pos.y;
            
            if (push_ammount > 0.05f) {
                push_ammount = 0.05f;
            }
            
            // check if there is space for him to be pushed up
            trigger_comp->SetLocation(trigger_pos + push_ammount);
            if (trigger_comp->Poll().size() == 0) {
                 new_pos.y += push_ammount;
            }
        }
        
        // if mongus is a little bit above the ground
        if (is_on_ground && new_pos.y > ground_height && ticks_since_jump > 3) {
            float push_ammount = new_pos.y - ground_height;
            
            // check if there is space for him to be pushed up
            trigger_comp->SetLocation(trigger_pos - push_ammount);
            if (trigger_comp->Poll().size() == 0) {
                 new_pos.y -= push_ammount;
            }
        }
        
        parent->UpdateTransform(new_pos, new_rot);
        
        
        
        //std::cout << movdir_forward << " " << movdir_side << std::endl;
        
        // show animations
        
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
        
        
        auto shadow = Physics::Raycast(in_pos, in_pos - vec3(0.0f, 5.0f, 0.0f), Physics::COLL_WORLDOBJ);
        
        if (shadow.collider) {
            //Render::AddLineMarker(shadow.point, Render::COLOR_GREEN);
        }
        
        was_run = is_run;
        ticks_since_sway++;
        ticks_until_jump--;
        ticks_since_yeet++;
        ticks_since_jump++;
        ticks_since_left_ground++;
    }
    
    static void Update() {
        for (auto& m : PoolProxy<MongusComponent>::GetPool()) {
            m.Move();
            m.CheckIfYeeted();
        }
    }
    
    void CheckIfYeeted() {
        if (parent->GetLocation().y < -50.0f) {
            SwitchLevel("majas-ara");
        }
    }
    
    void YeetIntoAir () {
        if (ticks_since_yeet > 10) {
            parent->SetLocation(parent->GetLocation() + vec3 (0.0f, 0.1f, 0.0f));
            velocity += vec3 (0.1f, 0.3f, 0.1f);
            ticks_since_yeet = 0;
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
    int32_t ticks_since_yeet = 0;
    int32_t ticks_since_jump = 100;
    int32_t ticks_since_left_ground = 0;
    
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
    Mongus() : Entity("player") {}
    ~Mongus() = default;
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
        //render_comp->SetPose(armature_comp->GetPosePtr());
        render_comp->SetArmature(armature_comp.get());
        render_comp->Init();
        
        physics_comp->SetParent(this);
        physics_comp->SetKinematic(true);
        physics_comp->SetCollisionGroup(Physics::COLL_PLAYER);
        physics_comp->SetShape(Physics::CollisionShape::Cylinder(0.5, 0.5f));
        physics_comp->Init();
        
        trigger_comp->SetShape(Physics::CollisionShape::Capsule(0.4f, 0.2f));
        //rigger_comp->SetCollisionGroup(Physics::COLL_PLAYER);
        trigger_comp->SetCollisionMask(-1 ^ Physics::COLL_PLAYER);
        trigger_comp->Init();
        
        mongus_comp->SetArmatureComponent(armature_comp.get());
        mongus_comp->SetTriggerComponent(trigger_comp.get());
        
        physics_comp->SetActivation(true);
        
        this->is_loaded = true;
    }

    void Unload() {
        assert(false);
    }

    void Serialize() {
        assert(false);
    }

    void UpdateParameters() {
        render_comp->SetLocation(location);
        render_comp->SetRotation(rotation);
        //physics_comp->SetLocation(location + vec3(0.0f, 1.6f, 0.0f));
    }
    
    void SetParameters() {
        assert(is_loaded);
        UpdateParameters();
    }

    void MessageHandler(Message& msg) {
        if (is_loaded && msg.type == 420) {
            mongus_comp->YeetIntoAir();
        }
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