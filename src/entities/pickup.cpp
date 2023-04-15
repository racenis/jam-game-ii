#include <framework/message.h>
#include <framework/logging.h>
#include <framework/worldcell.h>

#include "pickup.h"
#include "../score.h"



Pickup::Pickup(std::string_view& str) : Entity(str) {
    serializeddata.make();
    serializeddata->FromString(str);
}

void Pickup::UpdateParameters() {
    if (!is_loaded) return;
    render_component->SetLocation(location);
    render_component->SetRotation(rotation);
    trigger_component->SetLocation(location);
    trigger_component->SetRotation(rotation);
    return;
}

void Pickup::SetParameters() {
    if (!is_loaded) return;
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
    trigger_component->SetShape(Physics::CollisionShape::Sphere((name_t) serializeddata->pickup_model == "pickup/sula" ? 1.0f : 0.5f));
    trigger_component->SetCollisionMask(Physics::COLL_PLAYER);
    trigger_component->SetActivationCallback([](TriggerComponent* component, Physics::Collision) {
        dynamic_cast<Pickup*>(component->GetParent())->PickedUp();
    });
    
    render_component->Init();
    armature_component->Init();
    trigger_component->Init();
    
    //render_component->SetPose(armature_component->GetPosePtr());
    render_component->SetArmature(armature_component.get());

    armature_component->PlayAnimation("pickup-spin", 1000.0f, 1.0f, 1.0f);

    is_loaded = true;

    UpdateParameters();
}

void Pickup::Unload() {
    is_loaded = false;

    render_component.clear();
    armature_component.clear();
    trigger_component.clear();
    
    return;
}

void Pickup::Serialize() {
    return;
}

void Pickup::PickedUp() {
    if (picked_up) return;
    
    auto model = render_component->GetModel();
    
    if (model == "pickup/kirsis") {
        AddScore(10);
    } else if (model == "pickup/vinoga") {
        AddScore(20);
    } else if (model == "pickup/pepelsins") {
        AddScore(50);
    } else if (model == "pickup/kivi") {
        AddScore(100);
    } else if (model == "pickup/banans") {
        AddScore(250);
    } else if (model == "pickup/puke") {
        AddScore(1000);
    } else if (model == "pickup/sula") {
        AddScore(500);
        JuiceCollect();
    } else {
        AddScore(10);
        Log ("Unrecognized pickup type {}!", model);
    }
    
    armature_component->PlayAnimation("pickup-pickup", 1, 1.0f, 1.0f, true, true);
    
    picked_up = true;
    
    Message::Send(Message {.type = Message::ACTIVATE, .receiver = id, .sender = id}, 60);
}

void Pickup::MessageHandler (Message& msg) {
    if (!is_loaded) return;
    
    if (msg.type == Message::ACTIVATE) {
        delete this;
    }
}

