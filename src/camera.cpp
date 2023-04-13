#include <framework/ui.h>
#include <render/render.h>
#include <render/api.h>
#include "camera.h"
#include "mongus.h"

using namespace tram;

static bool is_camera_locked = false;
static bool is_camera_dynamic = false;

static vec3 camera_position = {0.0f, 0.0f, 0.0f};
static quat camera_rotation = {1.0f, 0.0f, 0.0f, 0.0f};

static float camera_velocity = 0.0f;

static bool nudge_left = false;
static bool nudge_right = false;


float MongusOcclusion(vec3 mongus_middle, vec3 camera_position) {
    auto camera_to_mongus = Physics::Raycast(camera_position, mongus_middle, Physics::COLL_WORLDOBJ);
    auto mongus_to_camera = Physics::Raycast(mongus_middle, camera_position, Physics::COLL_WORLDOBJ);
    
    if (camera_to_mongus.collider && mongus_to_camera.collider) {
        //Render::AddLineMarker(camera_to_mongus.point, Render::COLOR_BLUE);
        //Render::AddLineMarker(mongus_to_camera.point, Render::COLOR_RED);
        return glm::distance(camera_to_mongus.point, mongus_to_camera.point);
    } else {
        return 0.0f;
    }
}

vec3 CameraRotateAroundMongus(vec3 mongus_middle, vec3 camera_position, float rotation_ammount) {
    return (quat(vec3(0.0f, rotation_ammount, 0.0f)) * (camera_position - mongus_middle)) + mongus_middle;
}

void MongusCameraUpdate() {
    if (UI::GetInputState() == UI::STATE_DEFAULT) {
        vec3 mongus_middle = MAIN_MONGUS->GetLocation() + vec3(0.0f, 1.0f, 0.0f);
        vec3 look_dir = glm::normalize(camera_position - mongus_middle);

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
            
            // if mongus is too far, move closer
            if (mongus_distance > 7.5f && mongus_altitude > -5.0f) {
                camera_position -= look_dir * (powf(mongus_distance - 7.5f, 2) * 0.01f);
            }
            
            // if mongus is too close, move farther
            if (mongus_distance < 1.0f && mongus_altitude > -5.0f) {
                camera_position += look_dir * (powf(1.0f - mongus_distance, 2) * 0.01f);
            }
            
            // if mongus is too high
            if (camera_position.y - mongus_altitude < 4.0f) {
                camera_position.y += (powf(camera_position.y - mongus_altitude - 4.0f, 2) * 0.01f);
            }
            
            // if mongus is behind something
            if (MongusOcclusion(mongus_middle, camera_position) != 0.0f) {
                vec3 pos_a = CameraRotateAroundMongus(mongus_middle, camera_position, 0.02f);
                vec3 pos_b = CameraRotateAroundMongus(mongus_middle, camera_position, -0.02f);
                
                float pos_a_occlusion = MongusOcclusion(mongus_middle, pos_a);
                float pos_b_occlusion = MongusOcclusion(mongus_middle, pos_b);
                
                if (pos_a_occlusion < pos_b_occlusion) {
                    camera_velocity += 0.001f;
                } else {
                    camera_velocity -= 0.001f;
                }
                
                //std::cout << "pos a: " << pos_a_occlusion << "\tpos b: " << pos_b_occlusion << "\t dif: " << abs(pos_a_occlusion-pos_b_occlusion) << std::endl;
            } else {
                camera_velocity *= 0.95f;
            }
            
            if (camera_velocity != 0.0f) {
                camera_position = CameraRotateAroundMongus(mongus_middle, camera_position, camera_velocity);
            }
            
            if (nudge_left) {
                camera_velocity -= 0.0025f;
            }
            
            if (nudge_right) {
                camera_velocity += 0.0025f;
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
    
    Render::SetScreenClear({0.0f, 0.74f, 1.0f}, true);
}

void MongusCameraNudgeLeft (bool nudge) {
    nudge_left = nudge;
}

void MongusCameraNudgeRight (bool nudge) {
    nudge_right = nudge;
}