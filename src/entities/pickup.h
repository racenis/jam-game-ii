#ifndef JAM_GAME_II_ENTITIES_PICKUP_H
#define JAM_GAME_II_ENTITIES_PICKUP_H

#include <framework/entity.h>
#include <framework/serializeddata.h>
#include <components/rendercomponent.h>
#include <components/armaturecomponent.h>
#include <components/triggercomponent.h>

using namespace tram;

class Pickup : public Entity {
public:
    Pickup (std::string_view& str);

    void UpdateParameters();
    void SetParameters();
    void Load();
    void Unload();
    void Serialize();
    void MessageHandler(Message& msg);

    class Data: public SerializedEntityData {
    public:
        Field<name_t> pickup_model;

        void ToString(std::string& str) {
            pickup_model.ToString(str);;
        }

        void FromString(std::string_view& str) {
            pickup_model.FromString(str);
        }
        
        std::vector<FieldInfo> GetFieldInfo() {
            return std::vector<FieldInfo> {
                 { FieldInfo::FIELD_MODELNAME, "model" },
            };
        }
        
        char const* GetType() {
            return "pickup";
        }
    };
protected:
    Component<RenderComponent> render_component;
    Component<ArmatureComponent> armature_component;
    Component<TriggerComponent> trigger_component;
    SerializedData<Data> serializeddata;
};


#endif // JAM_GAME_II_ENTITIES_PICKUP_H