#include <framework/entitycomponent.h>
#include <framework/entity.h>
#include <framework/event.h>
#include <framework/ui.h>

#include <components/rendercomponent.h>
#include <components/triggercomponent.h>
#include <components/armaturecomponent.h>

using namespace Core;

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
                case UI::KEY_ACTION_FORWARD: movdir_x += dir; break;
                case UI::KEY_ACTION_BACKWARD: movdir_x -= dir; break;
                case UI::KEY_ACTION_LEFT: movdir_y += dir; break;
                case UI::KEY_ACTION_RIGHT: movdir_y -= dir; break;
            }
        }
    }
    
    static void Update() {
        for (auto& m : PoolProxy<MongusComponent>::GetPool()) {
            vec3 mdir = {(float) m.movdir_x, 0.0f, (float) m.movdir_y};
            mdir = glm::normalize(mdir);
            if (std::isnan(mdir.x) || std::isnan(mdir.y) || std::isnan(mdir.z)) {
                mdir = {0.0f, 0.0f, 0.0f};
            }
            mdir *= 0.1f;
            m.parent->UpdateTransform(m.parent->GetLocation() + mdir, m.parent->GetRotation());
        }
    }
    
    char movdir_x = 0; char movdir_y = 0;
    
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

