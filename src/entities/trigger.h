#ifndef JAM_GAME_II_ENTITIES_TRIGGER_H
#define JAM_GAME_II_ENTITIES_TRIGGER_H

#include <framework/entity.h>
#include <framework/message.h>
#include <framework/serializeddata.h>
#include <components/triggercomponent.h>

using namespace tram;

extern name_t CURRENT_TRIGGER;

const message_t TRIGGER_ACTIVATE = 100;
const message_t TRIGGER_DEACTIVATE = 101;

class Trigger : public Entity {
public:
    Trigger (std::string_view& str);

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
        Field<name_t> trigger_target;

        void ToString(std::string& str) {
            radius.ToString(str);
            trigger_action.ToString(str);
            trigger_target.ToString(str);
        }

        void FromString(std::string_view& str) {
            radius.FromString(str);
            trigger_action.FromString(str);
            trigger_target.FromString(str);
        }
        
        std::vector<FieldInfo> GetFieldInfo() {
            return std::vector<FieldInfo> {
                 { FieldInfo::FIELD_FLOAT, "radius" },
                 { FieldInfo::FIELD_STRING, "trigger-action" },
                 { FieldInfo::FIELD_STRING, "trigger-target" },
            };
        }
        
        char const* GetType() {
            return "trigger";
        }
    };
protected:
    Component<TriggerComponent> triggercomponent;
    SerializedData<Data> serializeddata;
};

#endif // JAM_GAME_II_ENTITIES_TRIGGER_H