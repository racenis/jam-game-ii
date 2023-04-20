#include <framework/core.h>
#include <framework/ui.h>
#include <render/render.h>
#include <framework/logging.h>
#include <framework/gui.h>
#include <framework/message.h>
#include <framework/language.h>
#include <framework/worldcell.h>
#include <audio/audio.h>
#include <physics/physics.h>

#include <components/rendercomponent.h>

#include <entities/staticworldobject.h>
#include <entities/marker.h>

#include <extensions/menu/menu.h>

#include "mongus.h"
#include "camera.h"
#include "score.h"
#include "sounds.h"
#include "levelswitch.h"
#include "entities/trigger.h"
#include "entities/switch.h"
#include "entities/door.h"
#include "entities/pickup.h"
#include "entities/crab.h"
#include "entities/frog.h"

using namespace tram;

static bool update_camera = true;

void mainloop();

int main () {
    SetSystemLoggingSeverity(System::SYSTEM_PLATFORM, SEVERITY_WARNING);
    SetSystemLoggingSeverity(System::SYSTEM_AUDIO, SEVERITY_WARNING);
    
    Core::Init();
    UI::Init();
    Render::Init();
    Physics::Init();
    Audio::Init();
    GUI::Init();
    Async::Init(0);

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

    LoadAllSounds();

    MAIN_MONGUS = new Mongus;
    MAIN_MONGUS->Load();
    MAIN_MONGUS->SetLocation(vec3 (5.0f, 0.0f, 0.0f));
    MAIN_MONGUS->SetRotation(vec3 (0.0f, 0.0f, 0.0f));

    InitLevelSwitch();
    LoadHomeLevel();
    
    Entity::Find("majas-ieksa-sula-1")->SetAutoLoad(false);
    Entity::Find("majas-ieksa-sula-2")->SetAutoLoad(false);
    Entity::Find("majas-ieksa-sula-3")->SetAutoLoad(false);
    
    Entity::Find("majas-ieksa-kuka")->SetAutoLoad(false);
    
    Event::AddListener (Event::KEYDOWN, [](Event& event) {
        if (event.subtype == UI::KEY_ACTION_LEFT) MongusCameraNudgeLeft(true);
        if (event.subtype == UI::KEY_ACTION_RIGHT) MongusCameraNudgeRight(true);
    });
    
    Event::AddListener (Event::KEYUP, [](Event& event) {
        if (event.subtype == UI::KEY_ACTION_LEFT) MongusCameraNudgeLeft(false);
        if (event.subtype == UI::KEY_ACTION_RIGHT) MongusCameraNudgeRight(false);
    });
    
    //UI::BindKeyboardKey(UI::KEY_F9, UI::KeyBinding {});
    
    
    UI::BindKeyboardKey(UI::KEY_APOSTROPHE, UI::KeyBinding { .type = UI::KeyBinding::SPECIAL_OPTION, .special_option = [](){
        LoadHomeLevel();
    }});
    
    UI::BindKeyboardKey(UI::KEY_F10, UI::KeyBinding { .type = UI::KeyBinding::SPECIAL_OPTION, .special_option = [](){
        update_camera = !update_camera;
    }});
    
#ifdef __EMSCRIPTEN__
    UI::SetWebMainLoop(mainloop);
#else
    while (!EXIT) {
        mainloop();
    }

    Async::Yeet();
    Audio::Uninit();
    UI::Uninit();
    
    return 0;    
#endif
}

void mainloop() {
    Core::Update();
    UI::Update();
    
    GUI::Begin();
    GUI::Text("Sulas glaaze 0.2", 1, GUI::TEXT_LEFT);
    
    GUI::Frame(GUI::FRAME_TOP, 20.0f);
        GUI::Text(std::to_string(GetScore()).c_str(), 2, GUI::TEXT_CENTER);
    GUI::EndFrame();
    
    if (IsLoaderLoading()) {
        std::string loading_text = Language::Get("loading");
        
        loading_text += " ";
        loading_text += std::to_string(GetLoaderProgress() * 100.0f);
        loading_text += "%";
        
        GUI::Frame(GUI::FRAME_BOTTOM, 100.0f);
            GUI::Text(loading_text.c_str(), 1, GUI::TEXT_CENTER);
        GUI::EndFrame();
    }
    
    if (IsWin()) {
        GUI::Frame(GUI::FRAME_BOTTOM, 80.0f);
            GUI::Text(Language::Get("winner"), 1, GUI::TEXT_CENTER);
        GUI::EndFrame();
    }
    
    Ext::Menu::DebugMenu();
    Ext::Menu::EscapeMenu();
    GUI::End();
    GUI::Update();

    if (update_camera) MongusCameraUpdate();

    //if (UI::PollKeyboardKey(tram::UI::KEY_E)) MAIN_MONGUS->SetLocation(MAIN_MONGUS->GetLocation()+vec3(0.0f, 1.0f, 0.0f));

    Event::Dispatch();
    Message::Dispatch();
    
    MongusComponent::Update();
    CrabComponent::Update();
    
    //UI::INPUT_STATE = UI::STATE_FLYING;
    
    Physics::Update();
    
#ifdef __EMSCRIPTEN__
    Async::ResourceLoader1stStage();
#endif
    
    Async::ResourceLoader2ndStage();
    Async::FinishResource();
    
    ArmatureComponent::Update();
    
    Render::Render();
    UI::EndFrame();

}