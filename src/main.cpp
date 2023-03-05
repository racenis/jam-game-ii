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
    
    
    Render::CAMERA_POSITION = {0.0f, 10.0f, 0.0f};
    Render::CAMERA_ROTATION = vec3 (-3.14f*0.5f, 0.0f, 0.0f);
    
    
    Render::Animation::Find("mongus-run")->LoadFromDisk();
    Render::Animation::Find("mongus-wag-tail")->LoadFromDisk();
    
    WorldCell* majas = WorldCell::Make("majas");
    majas->LoadFromDisk();
    majas->Load();



    Mongus* mongus = new Mongus;
    mongus->Load();
    mongus->SetLocation(vec3 (5.0f, 0.0f, 0.0f));
    mongus->SetRotation(vec3 (0.0f, 0.0f, 0.0f));
    
    while (!EXIT) {
        Core::Update();
        UI::Update();
        
        GUI::Begin();
        GUI::Text("Sulas glaaze pre-alpha do not redistribute", 1, GUI::TEXT_LEFT);
        GUI::End();
        GUI::Update();
        
        //Render::CAMERA_POSITION = mongus->GetLocation() + vec3(0.0f, 30.0f, 0.0f);
        //Render::CAMERA_ROTATION = LookAt (Render::CAMERA_POSITION, mongus->GetLocation());
        
        vec3 look_dir = glm::normalize(Render::CAMERA_POSITION - mongus->GetLocation());

        float camera_y = atan(look_dir.x/look_dir.z);
        
        if (look_dir.z < 0.0f) {
            camera_y -= 3.14f;
        }

        //float camera_x = (-1.0f + glm::dot(look_dir, glm::normalize(look_dir * vec3(1.0f, 0.0f, 1.0f))))*(3.14f/2.0f);
        float camera_x = -acos(glm::dot(look_dir, glm::normalize(look_dir * vec3(1.0f, 0.0f, 1.0f))));
        
        //float camera_x = -1.0f;
        
        //std::cout << camera_x << std::endl;

        quat camera_rot = vec3 (camera_x, camera_y, 0.0f);
    
        Render::CAMERA_ROTATION = camera_rot;
        Render::AddLine(mongus->GetLocation(), mongus->GetLocation() + (camera_rot * DIRECTION_FORWARD), Render::COLOR_CYAN);
        
        static vec3 fix_look_dir = look_dir;
        if (UI::PollKeyboardKey(UI::KEY_SPACE)) fix_look_dir = look_dir;
        Render::AddLine(mongus->GetLocation(), mongus->GetLocation() + fix_look_dir, Render::COLOR_PINK);
        
        Event::Dispatch();
        
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