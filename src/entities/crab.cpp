#include <templates/macros.h>

#include "crab.h"
#include "trigger.h"

TRAM_SDK_IMPLEMENT_POOL(CrabComponent, "crabcomppool", 50)

Crab::Crab(std::string_view& str) : Entity(str) {
    serializeddata.make();
    serializeddata->FromString(str);
}


void Crab::UpdateParameters() {
    if (!isloaded) return;
    rendercomponent->UpdateLocation(location);
    rendercomponent->UpdateRotation(rotation);
}

void Crab::SetParameters() {
    if (!isloaded) return;
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
    triggercomponent->SetShape(Physics::CollisionShape::Sphere(0.4f));
    
    physicscomponent->SetParent(this);
    physicscomponent->SetShape(Physics::CollisionShape::Sphere(0.6f));
    physicscomponent->SetKinematic();
    physicscomponent->SetCollisionGroup(Physics::COLL_VEHICLE);
    physicscomponent->SetCollisionMask(Physics::COLL_VEHICLE);
    
    crabcomponent->SetParent(this);
    crabcomponent->SetTriggerComponent(triggercomponent.get());
    crabcomponent->SetArmatureComponent(armaturecomponent.get());

    serializeddata.clear();

    rendercomponent->Init();
    armaturecomponent->Init();
    triggercomponent->Init();
    physicscomponent->Init();
    crabcomponent->Init();
    
    rendercomponent->SetPose(armaturecomponent->GetPosePtr());
    physicscomponent->SetActivation(true);

    isloaded = true;

    UpdateParameters();
}

void Crab::Unload() {
    isloaded = false;

    Serialize();

    rendercomponent.clear();
    armaturecomponent.clear();
}

void Crab::Serialize() {
    serializeddata.make();

    serializeddata->model = rendercomponent->GetModel();
}

void Crab::MessageHandler(Message& msg){

}

