#include <framework/entitycomponent.h>
#include <framework/entity.h>
#include <framework/event.h>
#include <framework/ui.h>

#include <components/rendercomponent.h>
#include <components/triggercomponent.h>
#include <components/armaturecomponent.h>

using namespace Core;

inline quat LookAt(vec3 sourcePoint, vec3 destPoint)
{
    vec3 forwardVector = glm::normalize(destPoint - sourcePoint);

    float dot = glm::dot(DIRECTION_FORWARD, forwardVector);

    if (abs(dot - (-1.0f)) < 0.000001f)
    {
        return quat (DIRECTION_UP.x, DIRECTION_UP.y, DIRECTION_UP.z, 3.1415926535897932f);
    }
    if (abs(dot - (1.0f)) < 0.000001f)
    {
        return quat (0.0f, 0.0f, 0.0f, 1.0f);
    }

    float rotAngle = (float)acos(dot);
    vec3 rotAxis = glm::cross(DIRECTION_FORWARD, forwardVector);
    rotAxis = glm::normalize(rotAxis);
    
    float halfAngle = rotAngle * .5f;
    float s = (float)sin(halfAngle);
    quat q;
    q.x = rotAxis.x * s;
    q.y = rotAxis.y * s;
    q.z = rotAxis.z * s;
    q.w = (float)cos(halfAngle);
    
    return q;
}

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
            
            //direction = glm::length(direction) > 0.0f ? glm::normalize(direction) : vec3(0.0f, 0.0f, 0.0f);
        }
    }
    
    void Move() {
        vec3 direction_forward = glm::normalize(Render::CAMERA_ROTATION * DIRECTION_FORWARD);
        vec3 direction_side = glm::normalize(Render::CAMERA_ROTATION * DIRECTION_SIDE);
        
        direction_forward.y = 0.0f;
        direction_side.y = 0.0f;
        
        direction = direction_forward * (float) movdir_forward;
        direction += direction_side * (float) movdir_side;
        
        direction = glm::normalize(direction);
        
        if (std::isnan(direction.x), std::isnan(direction.y), std::isnan(direction.z)) {
            direction = vec3(0.0f, 0.0f, 0.0f);
        }
        
        Render::AddLine(parent->GetLocation(), parent->GetLocation() + direction, Render::COLOR_BLUE);
        
        vec3 new_pos = parent->GetLocation() + (direction *0.1f);
        quat new_rot = vec3(0.0f, 3.14f + (atan(direction.x/direction.z) - (direction.z < 0.0f ? 3.14f : 0.0f)), 0.0f);
        
        parent->UpdateTransform(new_pos, new_rot);
        
        bool is_run = movdir_forward || movdir_side;
        
        //std::cout << movdir_forward << " " << movdir_side << std::endl;
        
        if (is_run && !was_run) {
            //armature_comp->StopAnimation("mongus-wag-tail");
            armature_comp->PlayAnimation("mongus-wag-tail", 100, 1.0f, 1.0f);
            std::cout << "starting anim" << std::endl;
        }
        
        if (!is_run && was_run) {
            armature_comp->StopAnimation("mongus-wag-tail");
            //armature_comp->PlayAnimation("mongus-wag-tail", 100, 1.0f, 1.0f);
            std::cout << "ending anim" << std::endl;
        }
        
        was_run = is_run;
    }
    
    static void Update() {
        for (auto& m : PoolProxy<MongusComponent>::GetPool()) {
            m.Move();
        }
    }
    
    void SetArmatureComponent (ArmatureComponent* armature_comp) {
        this->armature_comp = armature_comp;
    }
    
    ArmatureComponent* armature_comp = nullptr;
    
    char movdir_forward = 0; char movdir_side = 0;
    
    bool was_run = false;
    
    vec3 direction = DIRECTION_FORWARD;
    
    EventListener<Event::KEYDOWN, MongusComponent> keydown;
    EventListener<Event::KEYUP, MongusComponent> keyup;
};

class Mongus : public Entity {
public:
    void Load() {
        mongus_comp.make();
        render_comp.make();
        //physics_comp.make();
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
        //physics_comp->SetKinematic();
        //physics_comp->SetShape(Physics::CollisionShape::Cylinder(0.5, 0.5f));
        //physics_comp->Init();
        
        trigger_comp->SetShape(Physics::CollisionShape::Cylinder(0.5, 0.5f));
        trigger_comp->Init();
        
        mongus_comp->SetArmatureComponent(armature_comp.get());
        
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
    }
    
    void SetParameters() {
        assert(isloaded);
        UpdateParameters();
    }

    void MessageHandler(Message& msg) {
        
    }
    
private:
    Component<MongusComponent> mongus_comp;
    Component<RenderComponent> render_comp;
    Component<PhysicsComponent> physics_comp;
    Component<TriggerComponent> trigger_comp;
    Component<ArmatureComponent> armature_comp;
};

