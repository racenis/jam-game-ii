#include <components/triggercomponent.h>
#include "levelswitch.h"
#include "camera.h"
#include "mongus.h"

WorldCell* SELECTED_LEVEL = nullptr;

WorldCell* HOME_LEVEL = nullptr;
WorldCell* HOME_INTERIOR_LEVEL = nullptr;

WorldCell* FIRST_LEVEL = nullptr;



void InitLevelSwitch() {
    HOME_LEVEL = WorldCell::Make("majas");
    HOME_LEVEL->LoadFromDisk();
    HOME_INTERIOR_LEVEL = WorldCell::Make("majas-ieksa");
    HOME_INTERIOR_LEVEL->LoadFromDisk();
}

void LoadHomeLevel() {
    SELECTED_LEVEL = HOME_LEVEL;
    SELECTED_LEVEL->Load();
}

void SwitchLevel(name_t level_name) {
    SELECTED_LEVEL->Unload();
    SELECTED_LEVEL = nullptr;
    
    if (level_name == UID("majas-ieksa")) {
        SELECTED_LEVEL = HOME_INTERIOR_LEVEL;
        MongusCameraLock(true);
        MongusCameraMove(Entity::FindByName("majas-ieksa-kamera")->GetLocation());
    }
    
    if (level_name == UID("majas-ara")) {
        SELECTED_LEVEL = HOME_LEVEL;
    }
    
    MAIN_MONGUS->MongusPause();
    
    assert(SELECTED_LEVEL);
    SELECTED_LEVEL->Load();
}