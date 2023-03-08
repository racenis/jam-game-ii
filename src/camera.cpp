#include <framework/ui.h>
#include <render/render.h>
#include "camera.h"
#include "mongus.h"

using namespace Core;

bool is_camera_locked = false;

void MongusCameraUpdate() {
    if (UI::INPUT_STATE == UI::STATE_DEFAULT) {    
        vec3 look_dir = glm::normalize(Render::CAMERA_POSITION - (MAIN_MONGUS->GetLocation() + vec3(0.0f, 1.0f, 0.0f)));

        float camera_y = atan(look_dir.x/look_dir.z);
        
        if (look_dir.z < 0.0f) {
            camera_y -= 3.14f;
        }

        float camera_x = -acos(glm::dot(look_dir, glm::normalize(look_dir * vec3(1.0f, 0.0f, 1.0f))));
        
        quat camera_rot = vec3 (camera_x, camera_y, 0.0f);
    
        Render::CAMERA_ROTATION = camera_rot;
        
        if (!is_camera_locked) {
            float mongus_distance = glm::distance(Render::CAMERA_POSITION, MAIN_MONGUS->GetLocation());
            
            if (mongus_distance > 7.5f) {
                Render::CAMERA_POSITION -= look_dir * (mongus_distance > 15.0f ? 1.0f : 0.05f);
            }
            
            if ((Render::CAMERA_POSITION - MAIN_MONGUS->GetLocation()).y < 4.0f) {
                Render::CAMERA_POSITION.y += 0.01f;
            }
        }
    }
}

void MongusCameraMove(vec3 pos) {
    Render::CAMERA_POSITION = pos;
}

void MongusCameraLock(bool lock) {
    is_camera_locked = lock;
}