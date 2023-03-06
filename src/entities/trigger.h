#ifndef JAM_GAME_II_ENTITIES_TRIGGER_H
#define JAM_GAME_II_ENTITIES_TRIGGER_H

#include <framework/entity.h>
#include <framework/serializeddata.h>
#include <components/triggercomponent.h>

using namespace Core;

extern name_t CURRENT_TRIGGER;

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
                 { FieldInfo::FIELD_STRING, "trigger-action" },
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