#include <framework/ui.h>
#include <render/render.h>
#include "camera.h"
#include "mongus.h"

using namespace tram;

static bool is_camera_locked = false;
static bool is_camera_dynamic = false;

static vec3 camera_position = {0.0f, 0.0f, 0.0f};
static quat camera_rotation = {1.0f, 0.0f, 0.0f, 0.0f};


void MongusCameraUpdate() {
    if (UI::INPUT_STATE == UI::STATE_DEFAULT) {
        vec3 look_dir = glm::normalize(camera_position - (MAIN_MONGUS->GetLocation() + vec3(0.0f, 1.0f, 0.0f)));

        float camera_y = atan(look_dir.x/look_dir.z);
        
        if (look_dir.z < 0.0f) {
            camera_y -= 3.14f;
        }

        float camera_x = -acos(glm::dot(look_dir, glm::normalize(look_dir * vec3(1.0f, 0.0f, 1.0f))));
        
        quat camera_rot = vec3 (camera_x, camera_y, 0.0f);
    
        camera_rotation = camera_rot;
        
        if (is_camera_dynamic) {
            Render::SetSunDirection(glm::normalize(glm::quat(vec3(0.0f, camera_y, 0.0f)) * vec3(-1.0f, 2.0f, 1.0f)), 0);
        }
        
        if (!is_camera_locked) {
            float mongus_distance = glm::distance(camera_position, MAIN_MONGUS->GetLocation());
            float mongus_altitude = MAIN_MONGUS->GetLocation().y;
            
            if (mongus_distance > 7.5f && mongus_altitude > -5.0f) {
                camera_position -= look_dir * (powf(mongus_distance - 7.5f, 2) * 0.01f);
            }
            
            if (camera_position.y - mongus_altitude < 4.0f) {
                camera_position.y += (powf(camera_position.y - mongus_altitude - 4.0f, 2) * 0.01f);
            }
        }
        
        Render::SetCameraPosition(camera_position, 0);
        Render::SetCameraRotation(camera_rotation, 0);
    }
}

void MongusCameraMove(vec3 pos) {
    camera_position = pos;
}

void MongusCameraLock(bool lock) {
    is_camera_locked = lock;
}

void MongusCameraDynamic(bool camera) {
    is_camera_dynamic = camera;
    
    if (camera) {
        Render::SetSunDirection({-0.40f, 0.81f, -0.40f}, 0);
        Render::SetSunColor({0.777f, 0.777f, 0.777f}, 0);
        Render::SetAmbientColor({0.666f, 0.666f, 0.666f}, 0);
    } else {
        Render::SetSunDirection({0.577f, 0.577f, 0.577f}, 0);
        Render::SetSunColor({0.666f, 0.666f, 0.666f}, 0);
        Render::SetAmbientColor({0.444f, 0.444f, 0.444f}, 0);
    }
}