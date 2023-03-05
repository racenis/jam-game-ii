#include <framework/core.h>
#include <framework/ui.h>
#include <render/render.h>
#include <framework/logging.h>
#include <framework/gui.h>
#include <framework/worldcell.h>
#include <physics/physics.h>

#include <components/rendercomponent.h>
//#include <framework/worldcell.h>

#include <entities/staticworldobject.h>

#include <extensions/menu/menu.h>


#include "mongus.h"

using namespace Core;

int main () {
    
    Core::Init();
    UI::Init();
    Render::Init();
    Physics::Init();
    GUI::Init();
    Async::Init();

    Ext::Menu::Init();
    
    Entity::Register("staticwobj", [](std::string_view& params) -> Entity* {return new StaticWorldObject(params);});
    
    Render::Material::LoadMaterialInfo("data/material.list");
    
    Render::AMBIENT_COLOR = {0.5f, 0.5f, 0.5f};
    
    
    Render::CAMERA_POSITION = {0.0f, 30.0f, 0.0f};
    Render::CAMERA_ROTATION = vec3 (-3.14f*0.5f, 0.0f, 0.0f);
    
    WorldCell* majas = WorldCell::Make("majas");
    majas->LoadFromDisk();
    majas->Load();


    Mongus* mongus = new Mongus;
    mongus->Load();
    mongus->SetLocation(vec3 (5.0f, 0.0f, 0.0f));
    mongus->SetRotation(vec3 (0.0f, 0.0f, 0.0f));
    
    while (!EXIT) {
        UI::Update();
        
        GUI::Begin();
        GUI::Text("Sulas glaaze pre-alpha do not redistribute", 1, GUI::TEXT_LEFT);
        GUI::End();
        GUI::Update();
        
        Render::CAMERA_POSITION = mongus->GetLocation() + vec3(0.0f, 30.0f, 0.0f);
        
        Event::Dispatch();
        
        MongusComponent::Update();
        
        //UI::INPUT_STATE = UI::STATE_FLYING;
        
        Physics::Update();
        
        Async::ResourceLoader2ndStage();
        Async::FinishResource();
        
        Render::Render();
        UI::EndFrame();
    }
    
    Async::Yeet();
    
    return 0;
}