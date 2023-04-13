#ifndef JAM_GAME_II_ENTITIES_SWITCH_H
#define JAM_GAME_II_ENTITIES_SWITCH_H

#include <framework/entity.h>
#include <framework/serializeddata.h>

#include <components/rendercomponent.h>
#include <components/armaturecomponent.h>
#include <components/triggercomponent.h>

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
    
    void FlipIt();

    class Data: public SerializedEntityData {
    public:
        Field<float> radius;
        Field<name_t> yeet_target;
        Field<name_t> model;

        void ToString(std::string& str) {
            radius.ToString(str);
            yeet_target.ToString(str);
            model.ToString(str);
        }

        void FromString(std::string_view& str) {
            radius.FromString(str);
            yeet_target.FromString(str);
            model.FromString(str);
        }
        
        std::vector<FieldInfo> GetFieldInfo() {
            return std::vector<FieldInfo> {
                {FieldInfo::FIELD_FLOAT, "radius"},
                {FieldInfo::FIELD_STRING, "yeet-target"},
                {FieldInfo::FIELD_MODELNAME, "model"},
            };
        }
        
        char const* GetType() {
            return "switch";
        }
    };
protected:
    Component<RenderComponent> rendercomponent;
    Component<ArmatureComponent> armaturecomponent;
    Component<TriggerComponent> triggercomponent;
    SerializedData<Data> serializeddata;
    
    bool flipped = false;
};


#endif // JAM_GAME_II_ENTITIES_SWITCH_H