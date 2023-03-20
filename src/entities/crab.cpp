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

    isloaded = true;

    UpdateParameters();
}

void Crab::Unload() {
    std::cout << "crab is yeeted out of level" << std::endl;
    
    isloaded = false;

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
    std::cout << "CRAB MESSAGE" << std::endl;
    if (isloaded) {
        if (msg.type == 420) {
            std::cout << "CRAB MESSAGE INSIDE" << std::endl;
            crabcomponent->YeetIntoAir();
        }
    }
}

