#include <framework/message.h>
#include <framework/worldcell.h>

#include "trigger.h"

const message_t TRIGGER_ACTIVATE = 100;
const message_t TRIGGER_DEACTIVATE = 101;

name_t CURRENT_TRIGGER;

Trigger::Trigger(std::string_view& str) : Entity(str) {
    serializeddata.make();
    serializeddata->FromString(str);
}

void Trigger::UpdateParameters() {
    if (!isloaded) return;
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
    triggercomponent->SetActivationCallback([](TriggerComponent* comp){
        Message msg;
        
        msg.type = TRIGGER_ACTIVATE;
        msg.sender = comp->GetParent()->GetID();
        msg.receiver = comp->GetParent()->GetID();
        
        Message::Send(msg);
    });
    triggercomponent->SetDectivationCallback([](TriggerComponent* comp){
        Message msg;
        
        msg.type = TRIGGER_DEACTIVATE;
        msg.sender = comp->GetParent()->GetID();
        msg.receiver = comp->GetParent()->GetID();
        
        Message::Send(msg);
    });
    
    triggercomponent->Init();
    isloaded = true;

    UpdateParameters();
}

void Trigger::Unload() {
    isloaded = false;

    return;

    triggercomponent.clear();
}

void Trigger::Serialize() {
    return;
}

void Trigger::MessageHandler(Message& msg){
    if (msg.type == TRIGGER_ACTIVATE) {
        std::cout << "ACTIVATED!" << std::endl;
        CURRENT_TRIGGER = serializeddata->trigger_action;
    }
    
    if (msg.type == TRIGGER_DEACTIVATE) {
        std::cout << "DEACTIVATED!" << std::endl;
        CURRENT_TRIGGER = UID();
    }
}

