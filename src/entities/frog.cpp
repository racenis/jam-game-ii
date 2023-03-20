#include "frog.h"
#include "trigger.h"

Frog::Frog(std::string_view& str) : Entity(str) {
    serializeddata.make();
    serializeddata->FromString(str);
}


void Frog::UpdateParameters() {
    if (!isloaded) return;
    rendercomponent->UpdateLocation(location);
    rendercomponent->UpdateRotation(rotation);
}

void Frog::SetParameters() {
    if (!isloaded) return;
    UpdateParameters();
}

void Frog::Load(){
    rendercomponent.make();
    armaturecomponent.make();
    triggercomponent.make();
    
    rendercomponent->SetParent(this);
    rendercomponent->SetModel("vardite");

    armaturecomponent->SetParent(this);
    armaturecomponent->SetModel("vardite");
    
    triggercomponent->SetParent(this);
    triggercomponent->SetShape(Physics::CollisionShape::Cylinder(0.4f, 1.0f));
    triggercomponent->SetLocation(location /*+ vec3(0.0f, 1.1f, 0.0f)*/);
    triggercomponent->SetRotation(vec3(0.0f, 0.0f, 0.0f));
    triggercomponent->SetCollisionMask(-1 ^ Physics::COLL_WORLDOBJ);
    triggercomponent->SetCollisionGroup(-1 ^ Physics::COLL_WORLDOBJ);
    triggercomponent->Init();

    serializeddata.clear();

    rendercomponent->Init();
    armaturecomponent->Init();
    
    //rendercomponent->SetPose(armaturecomponent->GetPosePtr());
    rendercomponent->SetArmature(armaturecomponent.get());
    
    triggercomponent->SetActivationCallback([](TriggerComponent* comp, Physics::Collision coll) {
        std::cout << "frog is activete!" << std::endl;
        if (!coll.collider) return;
        
        std::cout << "frog is collide!" << std::endl;
        auto ent = coll.collider->GetParent();
        
        if (!ent) return;

        std::cout << "frog is sending!" << std::endl;
        Message::Send({
            .type = 420,
            .receiver = ent->GetID(),
            .sender = comp->GetParent()->GetID()
        });
    });

    isloaded = true;

    UpdateParameters();
}

void Frog::Unload() {
    isloaded = false;

    Serialize();

    rendercomponent.clear();
    armaturecomponent.clear();
}

void Frog::Serialize() {
    serializeddata.make();

    serializeddata->model = rendercomponent->GetModel();
}

void Frog::MessageHandler(Message& msg){

}

