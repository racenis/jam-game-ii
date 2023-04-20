#include <framework/message.h>
#include <framework/worldcell.h>

#include "switch.h"



Switch::Switch(std::string_view& str) : Entity(str) {
    serializeddata.make();
    serializeddata->FromString(str);
}

void Switch::UpdateParameters() {
    return;
}

void Switch::SetParameters() {
    if (!is_loaded) return;
    
    triggercomponent->SetLocation(location);
    triggercomponent->SetRotation(rotation);
    
    rendercomponent->SetLocation(location);
    rendercomponent->SetRotation(rotation);
}

void Switch::Load(){
    rendercomponent.make();
    armaturecomponent.make();
    triggercomponent.make();
    
    rendercomponent->SetParent(this);
    rendercomponent->SetModel("svira");
    
    armaturecomponent->SetParent(this);
    armaturecomponent->SetModel("svira");
    
    triggercomponent->SetParent(this);
    triggercomponent->SetShape(Physics::CollisionShape::Sphere(serializeddata->radius));
    triggercomponent->SetCollisionMask(-1 ^ Physics::COLL_WORLDOBJ);
    triggercomponent->SetCollisionGroup(-1 ^ Physics::COLL_WORLDOBJ);
    triggercomponent->SetActivationCallback([](TriggerComponent* comp, Physics::Collision) {
        dynamic_cast<Switch*>(comp->GetParent())->FlipIt();
    });
    
    rendercomponent->Init();
    armaturecomponent->Init();
    triggercomponent->Init();
    
    rendercomponent->SetArmature(armaturecomponent.get());
    
    is_loaded = true;

    SetParameters();
}

void Switch::Unload() {
    is_loaded = false;

    rendercomponent.clear();
    armaturecomponent.clear();
    triggercomponent.clear();
}

void Switch::Serialize() {
    return;
}

void Switch::MessageHandler(Message& msg) {
    
}

void Switch::FlipIt() {
    if (flipped) return;

    armaturecomponent->PlayAnimation("throw-switch", 1, 1.0f, 1.0f, true, true);

    Entity* target = Entity::Find(serializeddata->yeet_target);
    
    if (!target) {
        std::cout << "Can't find the target for " << name << std::endl;
    } else {
        delete target;
    }
    
    flipped = true;
}

