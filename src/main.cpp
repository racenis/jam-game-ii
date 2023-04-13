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

#include <entities/staticworldobject.h>
#include <entities/marker.h>

#include <extensions/menu/menu.h>

#include "mongus.h"
#include "camera.h"
#include "score.h"
#include "levelswitch.h"
#include "entities/trigger.h"
#include "entities/switch.h"
#include "entities/door.h"
#include "entities/pickup.h"
#include "entities/crab.h"
#include "entities/frog.h"

using namespace tram;

int main () {
    Core::Init();
    UI::Init();
    Render::Init();
    Physics::Init();
    GUI::Init();
    Async::Init();

    Ext::Menu::Init();
    
    Entity::RegisterType("staticwobj", [](std::string_view& params) -> Entity* {return new StaticWorldObject(params);});
    Entity::RegisterType("trigger", [](std::string_view& params) -> Entity* {return new Trigger(params);});
    Entity::RegisterType("switch", [](std::string_view& params) -> Entity* {return new Switch(params);});
    Entity::RegisterType("marker", [](std::string_view& params) -> Entity* {return new Marker(params);});
    Entity::RegisterType("pickup", [](std::string_view& params) -> Entity* {return new Pickup(params);});
    Entity::RegisterType("door", [](std::string_view& params) -> Entity* {return new Door(params);});
    Entity::RegisterType("crab", [](std::string_view& params) -> Entity* {return new Crab(params);});
    Entity::RegisterType("frog", [](std::string_view& params) -> Entity* {return new Frog(params);});
    
    Language::Load("lv");
    
    Render::Material::LoadMaterialInfo("material");    
    
    Render::Animation::Find("mongus-run")->LoadFromDisk();
    Render::Animation::Find("mongus-sway")->LoadFromDisk();
    Render::Animation::Find("mongus-jump")->LoadFromDisk();
    Render::Animation::Find("mongus-wag-tail")->LoadFromDisk();
    
    Render::Animation::Find("door-open-cw")->LoadFromDisk();
    Render::Animation::Find("door-open-ccw")->LoadFromDisk();
    
    Render::Animation::Find("door-close-cw")->LoadFromDisk();
    Render::Animation::Find("door-close-ccw")->LoadFromDisk();
    
    Render::Animation::Find("throw-switch")->LoadFromDisk();
    
    Render::Animation::Find("pickup-spin")->LoadFromDisk();
    Render::Animation::Find("pickup-pickup")->LoadFromDisk();
    
    Render::Animation::Find("krabis-walk")->LoadFromDisk();
    Render::Animation::Find("krabis-snibetisnab")->LoadFromDisk();
    
    Render::Animation::Find("vardite-catapult")->LoadFromDisk();


    MAIN_MONGUS = new Mongus;
    MAIN_MONGUS->Load();
    MAIN_MONGUS->SetLocation(vec3 (5.0f, 0.0f, 0.0f));
    MAIN_MONGUS->SetRotation(vec3 (0.0f, 0.0f, 0.0f));

    InitLevelSwitch();
    LoadHomeLevel();
    
    Event::AddListener (Event::KEYDOWN, [](Event& event) {
        if (event.subtype == UI::KEY_ACTION_LEFT) MongusCameraNudgeLeft(true);
        if (event.subtype == UI::KEY_ACTION_RIGHT) MongusCameraNudgeRight(true);
    });
    
    Event::AddListener (Event::KEYUP, [](Event& event) {
        if (event.subtype == UI::KEY_ACTION_LEFT) MongusCameraNudgeLeft(false);
        if (event.subtype == UI::KEY_ACTION_RIGHT) MongusCameraNudgeRight(false);
    });
    
    while (!EXIT) {
        Core::Update();
        UI::Update();
        
        GUI::Begin();
        GUI::Text("Sulas glaaze 0.2 pre-alpha", 1, GUI::TEXT_LEFT);
        
        /*GUI::Frame(GUI::FRAME_BOTTOM, 100.0f);
        if (CURRENT_TRIGGER) {
            GUI::Text(Language::Get("trigger-activate"), 1, GUI::TEXT_CENTER); GUI::FrameBreakLine();
            GUI::Text(Language::Get(CURRENT_TRIGGER), 1, GUI::TEXT_CENTER);
        }
        GUI::EndFrame();*/
        
        GUI::Frame(GUI::FRAME_TOP, 20.0f);
            //GUI::Text(Language::Get("trigger-activate"), 1, GUI::TEXT_CENTER);
            GUI::Text(std::to_string(GetScore()).c_str(), 2, GUI::TEXT_CENTER);
        GUI::EndFrame();
        
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

        if (UI::PollKeyboardKey(tram::UI::KEY_E)) MAIN_MONGUS->SetLocation(MAIN_MONGUS->GetLocation()+vec3(0.0f, 1.0f, 0.0f));

        Event::Dispatch();
        Message::Dispatch();
        
        MongusComponent::Update();
        CrabComponent::Update();
        
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