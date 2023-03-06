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
    Entity::Register("door", [](std::string_view& params) -> Entity* {return new Door(params);});
    
    Language::Load("data/lv.lang");
    
    Render::Material::LoadMaterialInfo("data/material.list");
    
    
    Render::AMBIENT_COLOR = {0.5f, 0.5f, 0.5f};
    
    
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

    InitLevelSwitch();
    LoadHomeLevel();


    Mongus* mongus = new Mongus;
    mongus->Load();
    mongus->SetLocation(vec3 (5.0f, 0.0f, 0.0f));
    mongus->SetRotation(vec3 (0.0f, 0.0f, 0.0f));
    
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
        
        Ext::Menu::DebugMenu();
        Ext::Menu::EscapeMenu();
        GUI::End();
        GUI::Update();
        
        //Render::CAMERA_POSITION = mongus->GetLocation() + vec3(0.0f, 30.0f, 0.0f);
        //Render::CAMERA_ROTATION = LookAt (Render::CAMERA_POSITION, mongus->GetLocation());
    

        if (UI::INPUT_STATE == UI::STATE_DEFAULT) {    
            vec3 look_dir = glm::normalize(Render::CAMERA_POSITION - mongus->GetLocation());

            float camera_y = atan(look_dir.x/look_dir.z);
            
            if (look_dir.z < 0.0f) {
                camera_y -= 3.14f;
            }

            float camera_x = -acos(glm::dot(look_dir, glm::normalize(look_dir * vec3(1.0f, 0.0f, 1.0f))));
            
            quat camera_rot = vec3 (camera_x, camera_y, 0.0f);
        
            Render::CAMERA_ROTATION = camera_rot;
            
            if (glm::distance(Render::CAMERA_POSITION, mongus->GetLocation()) > 7.5f) {
                Render::CAMERA_POSITION -= look_dir * 0.05f;
            }
            
            if ((Render::CAMERA_POSITION - mongus->GetLocation()).y < 4.0f) {
                Render::CAMERA_POSITION.y += 0.01f;
            }
        }



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