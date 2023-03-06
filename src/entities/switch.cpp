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
    if (!isloaded) return;
    physicscomponent->SetLocation(location);
    physicscomponent->SetRotation(rotation);
}

void Switch::Load(){
    physicscomponent.make();
    
    physicscomponent->SetParent(this);
    physicscomponent->SetShape(Physics::CollisionShape::Box({
        serializeddata->radius, 
        serializeddata->radius, 
        serializeddata->radius
    }));
    physicscomponent->SetCollisionGroup(Physics::COLL_MONGUS);
    physicscomponent->SetMass(0.0f);
    
    physicscomponent->Init();
    isloaded = true;

    UpdateParameters();
}

void Switch::Unload() {
    isloaded = false;

    return;

    physicscomponent.clear();
}

void Switch::Serialize() {
    return;
}

void Switch::MessageHandler(Message& msg){
    if (msg.type == Message::ACTIVATE) {
        std::cout << "ACTIVATED!" << std::endl;
    }
}

