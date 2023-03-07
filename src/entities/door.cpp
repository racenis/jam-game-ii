#include "door.h"
#include "trigger.h"


Door::Door(std::string_view& str) : Entity(str) {
    serializeddata.make();
    serializeddata->FromString(str);
}


void Door::UpdateParameters() {
    if (!isloaded) return;
    rendercomponent->UpdateLocation(location);
    rendercomponent->UpdateRotation(rotation);
}

void Door::SetParameters() {
    if (!isloaded) return;
    UpdateParameters();
}

void Door::Load(){
    rendercomponent.make();
    armaturecomponent.make();
    
    rendercomponent->SetParent(this);
    rendercomponent->SetModel(serializeddata->model);

    armaturecomponent->SetParent(this);
    armaturecomponent->SetModel(serializeddata->model);

    serializeddata.clear();

    rendercomponent->Init();
    armaturecomponent->Init();
    
    rendercomponent->SetPose(armaturecomponent->GetPosePtr());

    isloaded = true;

    UpdateParameters();
}

void Door::Unload() {
    isloaded = false;

    Serialize();

    rendercomponent.clear();
    armaturecomponent.clear();
}

void Door::Serialize() {
    serializeddata.make();

    serializeddata->model = rendercomponent->GetModel();
}

void Door::MessageHandler(Message& msg){
    if (!isloaded) return;
    if (msg.type == TRIGGER_ACTIVATE) {
        if (armaturecomponent->IsPlayingAnimation("door-close-cw")) {
            armaturecomponent->StopAnimation("door-close-cw");
        }
        
        armaturecomponent->PlayAnimation("door-open-cw", 1.0f, 1.0f, 1.0f, true, true);
        
        
        std::cout << "DOOR OPEN" << std::endl;
    } else if (msg.type == TRIGGER_DEACTIVATE) {
        if (armaturecomponent->IsPlayingAnimation("door-open-cw")) {
            armaturecomponent->StopAnimation("door-open-cw");
        }
        
        armaturecomponent->PlayAnimation("door-close-cw", 1.0f, 1.0f, 1.0f);
        
        std::cout << "DOOR CLOSE" << std::endl;
    } else {
        std::cout << "DOOR WHAT?" << std::endl;
    }
}

