#ifndef JAM_GAME_II_ENTITIES_SWITCH_H
#define JAM_GAME_II_ENTITIES_SWITCH_H

#include <framework/entity.h>
#include <framework/serializeddata.h>
#include <components/physicscomponent.h>

using namespace tram;

class Switch : public Entity {
public:
    Switch (std::string_view& str);

    void UpdateParameters();
    void SetParameters();
    void Load();
    void Unload();
    void Serialize();
    void MessageHandler(Message& msg);

    class Data: public SerializedEntityData {
    public:
        Field<float> radius;
        Field<name_t> trigger_action;

        void ToString(std::string& str) {
            radius.ToString(str);
            trigger_action.ToString(str);
        }

        void FromString(std::string_view& str) {
            radius.FromString(str);
            trigger_action.FromString(str);
        }
        
        std::vector<FieldInfo> GetFieldInfo() {
            return std::vector<FieldInfo> {
                 { FieldInfo::FIELD_FLOAT, "radius" },
                 { FieldInfo::FIELD_STRING, "switch-action" },
            };
        }
        
        char const* GetType() {
            return "switch";
        }
    };
protected:
    Component<PhysicsComponent> physicscomponent;
    SerializedData<Data> serializeddata;
};


#endif // JAM_GAME_II_ENTITIES_SWITCH_H