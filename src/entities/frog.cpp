#include "frog.h"
#include "trigger.h"

Frog::Frog(std::string_view& str) : Entity(str) {
    serializeddata.make();
    serializeddata->FromString(str);
}


void Frog::UpdateParameters() {
    if (!is_loaded) return;
    rendercomponent->SetLocation(location);
    rendercomponent->SetRotation(rotation);
}

void Frog::SetParameters() {
    if (!is_loaded) return;
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
        if (!coll.collider) return;
        
        dynamic_cast<Frog*>(comp->GetParent())->YeetIntoAir(coll.collider->GetParent());
    });

    is_loaded = true;

    UpdateParameters();
}

void Frog::Unload() {
    is_loaded = false;

    Serialize();

    rendercomponent.clear();
    armaturecomponent.clear();
    triggercomponent.clear();
}

void Frog::Serialize() {
    serializeddata.make();

    serializeddata->model = rendercomponent->GetModel();
}

void Frog::MessageHandler(Message& msg){

}


void Frog::YeetIntoAir(Entity* yeetable) {
    if (!yeetable) return;

    armaturecomponent->PlayAnimation("vardite-catapult", 1, 1.0f, 1.0f);

    Message::Send({
        .type = 420,
        .receiver = yeetable->GetID(),
        .sender = this->id
    });
}

