#include <framework/ui.h>
#include <render/render.h>
#include "camera.h"
#include "mongus.h"

using namespace Core;

bool is_camera_locked = false;
bool is_camera_dynamic = false;

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
        
        if (is_camera_dynamic) {
            Render::SUN_DIRECTION = glm::normalize(glm::quat(vec3(0.0f, camera_y, 0.0f)) * vec3(-1.0f, 2.0f, 1.0f));
        }
        
        if (!is_camera_locked) {
            float mongus_distance = glm::distance(Render::CAMERA_POSITION, MAIN_MONGUS->GetLocation());
            float mongus_altitude = MAIN_MONGUS->GetLocation().y;
            
            if (mongus_distance > 7.5f && mongus_altitude > -5.0f) {
                Render::CAMERA_POSITION -= look_dir * (powf(mongus_distance - 7.5f, 2) * 0.01f);
            }
            
            if (Render::CAMERA_POSITION.y - mongus_altitude < 4.0f) {
                Render::CAMERA_POSITION.y += (powf(Render::CAMERA_POSITION.y - mongus_altitude - 4.0f, 2) * 0.01f);
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

void MongusCameraDynamic(bool camera) {
    is_camera_dynamic = camera;
    
    if (camera) {
        Render::SUN_DIRECTION = {-0.40f, 0.81f, -0.40f};
        Render::SUN_COLOR = {0.777f, 0.777f, 0.777f};
        Render::AMBIENT_COLOR = {0.666f, 0.666f, 0.666f};
    } else {
        Render::SUN_DIRECTION = {0.577f, 0.577f, 0.577f};
        Render::SUN_COLOR = {0.666f, 0.666f, 0.666f};
        Render::AMBIENT_COLOR = {0.444f, 0.444f, 0.444f};
    }
}