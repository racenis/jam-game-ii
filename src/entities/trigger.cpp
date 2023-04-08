#include <framework/worldcell.h>

#include "trigger.h"
#include "../levelswitch.h"

name_t CURRENT_TRIGGER;

Trigger::Trigger(std::string_view& str) : Entity(str) {
    serializeddata.make();
    serializeddata->FromString(str);
}

void Trigger::UpdateParameters() {
    if (!is_loaded) return;
    triggercomponent->SetLocation(location);
    triggercomponent->SetRotation(rotation);
}

void Trigger::SetParameters() {
    UpdateParameters();
}

void Trigger::Load(){
    triggercomponent.make();
    
    triggercomponent->SetParent(this);
    triggercomponent->SetShape(Physics::CollisionShape::Box({
        serializeddata->radius, 
        serializeddata->radius, 
        serializeddata->radius
    }));
    triggercomponent->SetCollisionMask(Physics::COLL_PLAYER);
    triggercomponent->SetActivationCallback([](TriggerComponent* comp, Physics::Collision){
        Message msg;
        
        msg.type = TRIGGER_ACTIVATE;
        msg.sender = comp->GetParent()->GetID();
        msg.receiver = comp->GetParent()->GetID();
        
        Message::Send(msg);
    });
    triggercomponent->SetDectivationCallback([](TriggerComponent* comp, Physics::Collision){
        Message msg;
        
        msg.type = TRIGGER_DEACTIVATE;
        msg.sender = comp->GetParent()->GetID();
        msg.receiver = comp->GetParent()->GetID();
        
        Message::Send(msg);
    });
    
    triggercomponent->Init();
    is_loaded = true;

    UpdateParameters();
}

void Trigger::Unload() {
    is_loaded = false;
    std::cout << "TRIGGER IS UNLOADED " << std::endl;

    triggercomponent.clear();
}

void Trigger::Serialize() {
    return;
}

void Trigger::MessageHandler(Message& msg){
    if (msg.type == TRIGGER_ACTIVATE) {
        if (serializeddata->trigger_target != UID("none") && 
            serializeddata->trigger_action == UID("activate")) {
            Message::Send({
                .type = TRIGGER_ACTIVATE,
                .receiver = Entity::Find(serializeddata->trigger_target)->GetID()
            });
        }
        
        if (serializeddata->trigger_action == UID("enter")) {
            SwitchLevel(serializeddata->trigger_target);
        }
        
        
        CURRENT_TRIGGER = serializeddata->trigger_action;
    }
    
    if (msg.type == TRIGGER_DEACTIVATE) {
        if (serializeddata->trigger_target != UID("none") && 
            serializeddata->trigger_action == UID("activate")) {
            Message::Send({
                .type = TRIGGER_DEACTIVATE,
                .receiver = Entity::Find(serializeddata->trigger_target)->GetID()
            });
        }
        
        CURRENT_TRIGGER = UID();
    }
}

