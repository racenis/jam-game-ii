#include <components/triggercomponent.h>
#include "levelswitch.h"

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