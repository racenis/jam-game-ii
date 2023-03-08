#include <framework/core.h>
#include <framework/ui.h>
#include <render/render.h>
#include <framework/logging.h>
#include <framework/gui.h>
#include <framework/message.h>
#include <framework/language.h>
#include <framework/worldcell.h>
#include <physics/physics.h>

#include <components/rendercomponent.h>
//#include <framework/worldcell.h>

#include <entities/staticworldobject.h>
#include <entities/marker.h>

#include <extensions/menu/menu.h>


#include "mongus.h"
#include "camera.h"
#include "levelswitch.h"
#include "entities/trigger.h"
#include "entities/door.h"

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
    Entity::Register("trigger", [](std::string_view& params) -> Entity* {return new Trigger(params);});
    Entity::Register("marker", [](std::string_view& params) -> Entity* {return new Marker(params);});
    Entity::Register("door", [](std::string_view& params) -> Entity* {return new Door(params);});
    
    Language::Load("data/lv.lang");
    
    Render::Material::LoadMaterialInfo("data/material.list");
        
    
    Render::CAMERA_POSITION = {0.0f, 10.0f, 0.0f};
    Render::CAMERA_ROTATION = vec3 (-3.14f*0.5f, 0.0f, 0.0f);
    
    
    Render::Animation::Find("mongus-run")->LoadFromDisk();
    Render::Animation::Find("mongus-sway")->LoadFromDisk();
    Render::Animation::Find("mongus-jump")->LoadFromDisk();
    Render::Animation::Find("mongus-wag-tail")->LoadFromDisk();
    
    Render::Animation::Find("door-open-cw")->LoadFromDisk();
    Render::Animation::Find("door-open-ccw")->LoadFromDisk();
    
    Render::Animation::Find("door-close-cw")->LoadFromDisk();
    Render::Animation::Find("door-close-ccw")->LoadFromDisk();

    MAIN_MONGUS = new Mongus;
    MAIN_MONGUS->Load();
    MAIN_MONGUS->SetLocation(vec3 (5.0f, 0.0f, 0.0f));
    MAIN_MONGUS->SetRotation(vec3 (0.0f, 0.0f, 0.0f));

    InitLevelSwitch();
    LoadHomeLevel();
    
    while (!EXIT) {
        Core::Update();
        UI::Update();
        
        GUI::Begin();
        GUI::Text("Sulas glaaze pre-alpha do not redistribute", 1, GUI::TEXT_LEFT);
        
        /*GUI::Frame(GUI::FRAME_BOTTOM, 100.0f);
        if (CURRENT_TRIGGER) {
            GUI::Text(Language::Get("trigger-activate"), 1, GUI::TEXT_CENTER); GUI::FrameBreakLine();
            GUI::Text(Language::Get(CURRENT_TRIGGER), 1, GUI::TEXT_CENTER);
        }
        GUI::EndFrame();*/
        
        if (IsLoaderLoading()) {
            GUI::Frame(GUI::FRAME_BOTTOM, 100.0f);
                GUI::Text(Language::Get("loading"), 1, GUI::TEXT_CENTER);
            GUI::EndFrame();
            
            GetLoaderProgress();
        }
        
        Ext::Menu::DebugMenu();
        Ext::Menu::EscapeMenu();
        GUI::End();
        GUI::Update();
        
        //Render::CAMERA_POSITION = mongus->GetLocation() + vec3(0.0f, 30.0f, 0.0f);
        //Render::CAMERA_ROTATION = LookAt (Render::CAMERA_POSITION, mongus->GetLocation());
    

        MongusCameraUpdate();


        Event::Dispatch();
        Message::Dispatch();
        
        MongusComponent::Update();
        
        //UI::INPUT_STATE = UI::STATE_FLYING;
        
        Physics::Update();
        
        Async::ResourceLoader2ndStage();
        Async::FinishResource();
        
        ArmatureComponent::Update();
        
        Render::Render();
        UI::EndFrame();
    }
    
    Async::Yeet();
    
    return 0;
}