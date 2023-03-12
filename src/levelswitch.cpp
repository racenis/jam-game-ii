#include <components/triggercomponent.h>
#include <framework/async.h>
#include <framework/ui.h>
#include "levelswitch.h"
#include "camera.h"
#include "mongus.h"

WorldCell* SELECTED_LEVEL = nullptr;

WorldCell* HOME_LEVEL = nullptr;
WorldCell* HOME_INTERIOR_LEVEL = nullptr;

WorldCell* FIRST_LEVEL = nullptr;

size_t loader_goal = 0;
bool is_loader_loading = false;

void StartLoader() {
    loader_goal = Async::GetWaitingResources();
    is_loader_loading = true;
    MAIN_MONGUS->MongusPause();
}
    
float GetLoaderProgress() {
    size_t loader_progress = Async::GetWaitingResources();
    
    if (loader_progress == 0) {
        MAIN_MONGUS->MongusResume();
        is_loader_loading = false;
        return 1.0f;
    }
    
    return (float) loader_progress / (float) loader_goal;
}

bool IsLoaderLoading() {
    return is_loader_loading;
}

void InitLevelSwitch() {
    HOME_LEVEL = WorldCell::Make("majas");
    HOME_LEVEL->LoadFromDisk();
    HOME_INTERIOR_LEVEL = WorldCell::Make("majas-ieksa");
    HOME_INTERIOR_LEVEL->LoadFromDisk();
    FIRST_LEVEL = WorldCell::Make("limenis-1");
    FIRST_LEVEL->LoadFromDisk();
}

void LoadHomeLevel() {
    ///SwitchLevel(UID("majas-ara"));
    SwitchLevel(UID("limenis-1"));
}

void SwitchLevel(name_t level_name) {
    std::cout << "SWITCH LEVEL! " << level_name << std::endl;
    if (SELECTED_LEVEL) SELECTED_LEVEL->Unload();
    SELECTED_LEVEL = nullptr;
    
    if (level_name == UID("majas-ieksa")) {
        SELECTED_LEVEL = HOME_INTERIOR_LEVEL;
        MAIN_MONGUS->SetLocation(Entity::FindByName("majas-ieksa-ienacenis")->GetLocation());
        MongusCameraLock(true);
        MongusCameraMove(Entity::FindByName("majas-ieksa-kamera")->GetLocation());

    }
    
    if (level_name == UID("majas-ara")) {
        SELECTED_LEVEL = HOME_LEVEL;
        MAIN_MONGUS->SetLocation(Entity::FindByName("majas-ara-ienacenis")->GetLocation());
        MongusCameraLock(false);
        MongusCameraMove(Entity::FindByName("majas-ara-kamera")->GetLocation());
        MongusCameraDynamic(true);
    }
    
    if (level_name == UID("limenis-1")) {
        SELECTED_LEVEL = FIRST_LEVEL;
        MAIN_MONGUS->SetLocation(Entity::FindByName("limenis-1-ienacenis")->GetLocation());
        MongusCameraLock(false);
        MongusCameraDynamic(true);
    }
    
    MAIN_MONGUS->MongusPause();
    
    assert(SELECTED_LEVEL);
    SELECTED_LEVEL->Load();
    
    StartLoader();
}