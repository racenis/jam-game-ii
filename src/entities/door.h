#ifndef JAM_GAME_II_ENTITIES_DOOR_H
#define JAM_GAME_II_ENTITIES_DOOR_H

#include <framework/entity.h>
#include <components/rendercomponent.h>
#include <components/armaturecomponent.h>
#include <framework/serializeddata.h>

using namespace Core;

class Door : public Entity {
public:
    Door(std::string_view& str);

    void UpdateParameters();
    void SetParameters();
    void Load();
    void Unload();
    void Serialize();
    void MessageHandler(Message& msg);

    class Data: public SerializedEntityData {
    public:
        Field<name_t> model;

        void ToString(std::string& str) {
            model.ToString(str);
        }

        void FromString(std::string_view& str) {
            model.FromString(str);
        }
        
        std::vector<FieldInfo> GetFieldInfo() {
            return std::vector<FieldInfo> {
                 { FieldInfo::FIELD_MODELNAME, "model" },
            };
        }
        
        char const* GetType() {
            return "door";
        }
    };
protected:
    Component<RenderComponent> rendercomponent;
    Component<ArmatureComponent> armaturecomponent;
    SerializedData<Data> serializeddata;
};


#endif // JAM_GAME_II_ENTITIES_DOOR_H