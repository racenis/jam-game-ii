#include <framework/message.h>
#include <framework/worldcell.h>

#include "pickup.h"



Pickup::Pickup(std::string_view& str) : Entity(str) {
    serializeddata.make();
    serializeddata->FromString(str);
}

void Pickup::UpdateParameters() {
    if (!isloaded) return;
    render_component->UpdateLocation(location);
    render_component->UpdateRotation(rotation);
    trigger_component->SetLocation(location);
    trigger_component->SetRotation(rotation);
    return;
}

void Pickup::SetParameters() {
    if (!isloaded) return;
    UpdateParameters();
}

void Pickup::Load(){
    render_component.make();
    armature_component.make();
    trigger_component.make();

    render_component->SetParent(this);
    render_component->SetModel(serializeddata->pickup_model);
        
    armature_component->SetParent(this);
    armature_component->SetModel(serializeddata->pickup_model);

    trigger_component->SetParent(this);
    trigger_component->SetShape(Physics::CollisionShape::Sphere(0.5f));
    trigger_component->SetCollisionMask(Physics::COLL_PLAYER);
    trigger_component->SetActivationCallback([](TriggerComponent* component){
        Message msg;
        
        msg.type = Message::ACTIVATE;
        msg.sender = component->GetParent()->GetID();
        msg.receiver = component->GetParent()->GetID();
        
        Message::Send(msg);
    });
    
    render_component->Init();
    armature_component->Init();
    trigger_component->Init();
    
    render_component->SetPose(armature_component->GetPosePtr());

    armature_component->PlayAnimation("pickup-spin", 1000.0f, 1.0f, 1.0f);

    isloaded = true;

    UpdateParameters();
}

void Pickup::Unload() {
    isloaded = false;

    render_component.clear();
    armature_component.clear();
    trigger_component.clear();
    
    return;
}

void Pickup::Serialize() {
    return;
}

void Pickup::MessageHandler(Message& msg){
    if (!isloaded) return;
    
    if (msg.type == Message::ACTIVATE) {
        Unload();
    }
}
