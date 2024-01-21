#pragma once

#include "W4Framework.h"
//#include "UiPoint.h"
#include "data/Offset.h"
#include "data/FillData.h"

W4_USE_UNSTRICT_INTERFACE

namespace games::benice::ui
{
    typedef std::function<void(math::rect)> ChangeCallback;
    
    using namespace games::benice::ui::data;
    using games::benice::ui::data::FillData;
    using games::benice::ui::data::FillType;
    
    class UiRect : public w4::enable_from_this<UiRect>
    {
    public:
        UiRect () = default;
        
        UiRect (const std::string &id, Offset left, Offset right, Offset top, Offset bottom);
        
        UiRect (Offset left, Offset right, Offset top, Offset bottom);
        
        ~UiRect ();
        
        void setId (std::string id);
        
        const std::string &getId () const;
        
        void initialize (Offset left, Offset right, Offset top, Offset bottom);
        
        void setFillData (const FillData &data);
        
        const FillData &getFillData () const;
        
        void setFill (FillType fillType, float aspect);
        
        void setAlign (HorizontalAlign horizontalAlign, VerticalAlign verticalAlign);
        
        void parentSize (const math::rect &parentRect);
        
        sptr<UiRect> findChild (const std::string &name);
        
        math::rect getValue () const;
        
        void addChild (sptr<UiRect> child);
        
        void removeChild (sptr<UiRect> child);
        
        Offset getLeftOffset () const;
        
        Offset getRightOffset () const;
        
        Offset getTopOffset () const;
        
        Offset getBottomOffset () const;
        
        FillType getFillType () const;
        
        float getAspect () const;
        
        HorizontalAlign getHorizontalAlign () const;
        
        VerticalAlign getVerticalAlign () const;
        
        std::vector<sptr<UiRect>> getChildren () const;
        
        void setOnChangeHandler( ChangeCallback callback );
        void removeOnChangeHandler();
    
    private:
        static uint m_counter;
        std::string m_id;
        
        Offset m_left;
        Offset m_right;
        Offset m_top;
        Offset m_bottom;
        
        FillData m_fillData;
        
        float getHorizontalPos (const math::rect &parentRect, const Offset &offet);
        
        float getVerticalPos (const math::rect &parentRect, const Offset &offet);
        void pushChange();
        
        math::rect m_value;
        std::vector<sptr<UiRect>> m_children;
        ChangeCallback m_onChangedCallback;
    };
}


namespace nlohmann
{
    using games::benice::ui::UiRect;
    
    template<>
    struct adl_serializer<UiRect>
    {
        static void to_json (json &j, const UiRect &obj)
        {
            j["id"] = obj.getId();
            j["left"] = obj.getLeftOffset();
            j["right"] = obj.getRightOffset();
            j["top"] = obj.getTopOffset();
            j["bottom"] = obj.getBottomOffset();
            
            j["fill"] = obj.getFillData();
            j["children"] = obj.getChildren();
        }
        
        static void from_json (const json &j, UiRect &obj)
        {
            obj.setId(j["id"]);
            obj.initialize(j["left"], j["right"], j["top"], j["bottom"]);
            obj.setFillData(j["fill"]);
            auto children = j["children"];
            for (auto child: children)
            {
                auto item = make::sptr<UiRect>();
                from_json(child, *(item));
                obj.addChild(item);
            }
        }
    };
}
