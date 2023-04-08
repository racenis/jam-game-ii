#include <templates/macros.h>

#include "crab.h"
#include "trigger.h"

TRAM_SDK_IMPLEMENT_POOL(CrabComponent, "crabcomppool", 50)

Crab::Crab(std::string_view& str) : Entity(str) {
    serializeddata.make();
    serializeddata->FromString(str);
}


void Crab::UpdateParameters() {
    if (!is_loaded) return;
    rendercomponent->SetLocation(location);
    rendercomponent->SetRotation(rotation);
}

void Crab::SetParameters() {
    if (!is_loaded) return;
    UpdateParameters();
}

void Crab::Load(){
    rendercomponent.make();
    armaturecomponent.make();
    triggercomponent.make();
    physicscomponent.make();
    crabcomponent.make();
    
    rendercomponent->SetParent(this);
    rendercomponent->SetModel("krabis");

    armaturecomponent->SetParent(this);
    armaturecomponent->SetModel("krabis");
    
    triggercomponent->SetParent(this);
    triggercomponent->SetCollisionMask(Physics::COLL_PLAYER | Physics::COLL_WORLDOBJ);
    triggercomponent->SetShape(Physics::CollisionShape::Sphere(0.4f));
    
    physicscomponent->SetParent(this);
    physicscomponent->SetShape(Physics::CollisionShape::Cylinder(1.0f, 0.5f));
    physicscomponent->SetKinematic(true);
    physicscomponent->SetCollisionGroup(Physics::COLL_VEHICLE);
    //physicscomponent->SetCollisionMask(Physics::COLL_VEHICLE);
    
    crabcomponent->SetParent(this);
    crabcomponent->SetTriggerComponent(triggercomponent.get());
    crabcomponent->SetArmatureComponent(armaturecomponent.get());

    serializeddata.clear();

    rendercomponent->Init();
    armaturecomponent->Init();
    triggercomponent->Init();
    physicscomponent->Init();
    crabcomponent->Init();
    
    //rendercomponent->SetPose(armaturecomponent->GetPosePtr());
    rendercomponent->SetArmature(armaturecomponent.get());
    physicscomponent->SetActivation(true);

    is_loaded = true;

    UpdateParameters();
}

void Crab::Unload() {
    std::cout << "crab is yeeted out of level" << std::endl;
    
    is_loaded = false;

    Serialize();

    rendercomponent.clear();
    armaturecomponent.clear();
    triggercomponent.clear();
    physicscomponent.clear();
    crabcomponent.clear();
}

void Crab::Serialize() {
    serializeddata.make();

    serializeddata->model = rendercomponent->GetModel();
}

void Crab::MessageHandler(Message& msg){
    if (is_loaded) {
        if (msg.type == 420) {
            crabcomponent->YeetIntoAir();
        }
    }
}

