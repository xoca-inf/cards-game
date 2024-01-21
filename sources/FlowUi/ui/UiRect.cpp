#include "UiRect.h"

namespace games::benice::ui
{
    uint UiRect::m_counter = 0;
    
    UiRect::UiRect (Offset left, Offset right, Offset top, Offset bottom) : m_left(left), m_right(right), m_top(top), m_bottom(bottom), m_value(math::rect(0, 0, 1, 1))
    {
        UiRect::m_counter++;
        m_id = "ui [" + std::to_string(m_counter) + "]";
    }
    
    UiRect::UiRect (const std::string &id, Offset left, Offset right, Offset top, Offset bottom) : m_id(id), m_left(left), m_right(right), m_top(top), m_bottom(bottom), m_value(math::rect(0, 0, 1, 1))
    {
    }
    
    void UiRect::initialize (Offset left, Offset right, Offset top, Offset bottom)
    {
        m_left = left;
        m_right = right;
        m_top = top;
        m_bottom = bottom;
    }
    
    UiRect::~UiRect ()
    {
        m_onChangedCallback = nullptr;
    }
    
    void UiRect::setId (std::string id)
    {
        m_id = id;
    }
    
    const std::string &UiRect::getId () const
    {
        return m_id;
    }
    
    void UiRect::setFillData (const FillData &data)
    {
        m_fillData = data;
    }
    
    const FillData &UiRect::getFillData () const
    {
        return m_fillData;
    }
    
    math::rect UiRect::getValue () const
    {
        return m_value;
    }
    
    
    Offset UiRect::getLeftOffset () const
    { return m_left; }
    
    Offset UiRect::getRightOffset () const
    { return m_right; }
    
    Offset UiRect::getTopOffset () const
    { return m_top; }
    
    Offset UiRect::getBottomOffset () const
    { return m_bottom; }
    
    FillType UiRect::getFillType () const
    { return m_fillData.m_fillType; }
    
    float UiRect::getAspect () const
    { return m_fillData.m_aspect; }
    
    HorizontalAlign UiRect::getHorizontalAlign () const
    { return m_fillData.m_horizontalAlign; }
    
    VerticalAlign UiRect::getVerticalAlign () const
    { return m_fillData.m_verticalAlign; }
    
    std::vector<sptr<UiRect>> UiRect::getChildren () const
    { return m_children; }
    
    sptr<UiRect> UiRect::findChild (const std::string &id)
    {
        if (m_id == id)
        {
            return shared_from_this();
        }
        
        for (const auto &child: m_children)
        {
            auto result = child->findChild(id);
            if (result != nullptr)
            {
                return result;
            }
        }
        
        return nullptr;
    }
    
    void UiRect::setFill (FillType fillType, float aspect)
    {
        m_fillData.m_fillType = fillType;
        m_fillData.m_aspect = aspect;
    }
    
    void UiRect::setAlign (HorizontalAlign horizontalAlign, VerticalAlign verticalAlign)
    {
        m_fillData.m_horizontalAlign = horizontalAlign;
        m_fillData.m_verticalAlign = verticalAlign;
    }
    
    void UiRect::parentSize (const math::rect &parentRect)
    {
        float left = getHorizontalPos(parentRect, m_left);
        float right = getHorizontalPos(parentRect, m_right);
        float top = getVerticalPos(parentRect, m_top);
        float bottom = getVerticalPos(parentRect, m_bottom);
        
        m_value.size.x = right - left;
        m_value.size.y = top - bottom;
        
        m_value.position = vec2(left + m_value.size.x / 2.0f, bottom + m_value.size.y / 2.0);
        
        vec2 scale = m_value.size;
        switch (m_fillData.m_fillType)
        {
            case FillType::IGNORE_ASPECT:
                break;
            case FillType::FIT_HORIZONTAL:
                scale = vec2(1, 1 / m_fillData.m_aspect) * m_value.size.w;
                break;
            case FillType::FIT_VERTICAL:
                scale = vec2(m_fillData.m_aspect, 1) * m_value.size.h;
                break;
            case FillType::INSIDE:
                if (m_value.size.w / m_value.size.h > m_fillData.m_aspect)
                {
                    scale = vec2(m_fillData.m_aspect, 1) * m_value.size.h;
                }
                else
                {
                    scale = vec2(1, 1 / m_fillData.m_aspect) * m_value.size.w;
                }
                break;
            case FillType::OUTSIDE:
                if (m_value.size.w / m_value.size.h > m_fillData.m_aspect)
                {
                    scale = vec2(1, 1 / m_fillData.m_aspect) * m_value.size.w;
                }
                else
                {
                    scale = vec2(m_fillData.m_aspect, 1) * m_value.size.h;
                }
                break;
            default:
                W4_LOG_ERROR("[UiRect] error m_fillType %d", (int) m_fillData.m_fillType);
                break;
        }
        
        vec2 pos = vec2(0, 0);
        switch (m_fillData.m_horizontalAlign)
        {
            case HorizontalAlign::Center:
                pos.x = m_value.position.x;
                break;
            case HorizontalAlign::Left:
                pos.x = m_value.position.x - m_value.size.w / 2.0 + scale.x / 2.0;
                break;
            case HorizontalAlign::Right:
                pos.x = m_value.position.x + m_value.size.w / 2.0 - scale.x / 2.0;
                break;
        }
        
        switch (m_fillData.m_verticalAlign)
        {
            case VerticalAlign::Center:
                pos.y = m_value.position.y;
                break;
            case VerticalAlign::Top:
                pos.y = m_value.position.y + m_value.size.h / 2.0 - scale.y / 2.0;
                break;
            case VerticalAlign::Bottom:
                pos.y = m_value.position.y - m_value.size.h / 2.0 + scale.y / 2.0;
                break;
        }
        
        m_value.size = scale;
        m_value.position = pos;
        pushChange();
        for (auto &child: m_children)
        {
            child->parentSize(m_value);
        }
    }
    
    void UiRect::pushChange ()
    {
        if (m_onChangedCallback != nullptr)
        {
            m_onChangedCallback(m_value);
        }
    }
    
    
    void UiRect::addChild (sptr<UiRect> child)
    {
        m_children.push_back(child);
        child->parentSize(getValue());
    }
    
    void UiRect::removeChild (sptr<UiRect> child)
    {
        std::remove(m_children.begin(), m_children.end(), child);
    }
    
    float UiRect::getHorizontalPos (const math::rect &parentRect, const Offset &offet)
    {
        float result = 0;
        
        switch (offet.pivot)
        {
            case PivotAt::LEFT:
            case PivotAt::RIGHT:
            case PivotAt::CENTER:
                result = offet.calcDimension(parentRect.position.x - parentRect.size.w / 2.0, parentRect.position.x + parentRect.size.w / 2.0);
                break;
            default:
                W4_LOG_ERROR("[UiRect] cant use PivotAt type in horizontal offset %d", (int) offet.pivot);
                break;
        }
        return result;
    }
    
    float UiRect::getVerticalPos (const math::rect &parentRect, const Offset &offet)
    {
        float result = 0.0f;
        switch (offet.pivot)
        {
            case PivotAt::TOP:
            case PivotAt::BOTTOM:
            case PivotAt::CENTER:
                result = offet.calcDimension(parentRect.position.y - parentRect.size.h / 2.0, parentRect.position.y + parentRect.size.h / 2.0);
                break;
            default:
                W4_LOG_ERROR("[UiRect] cant use PivotAt type in vertical offset %d", (int) offet.pivot);
                break;
        }
        return result;
    }
    
    void UiRect::setOnChangeHandler (ChangeCallback callback)
    {
        if (m_onChangedCallback != nullptr)
        {
            W4_LOG_WARNING("[UiRect] reset m_onChangedCallback!");
        }
        m_onChangedCallback = callback;
    }
    
    void UiRect::removeOnChangeHandler ()
    {
        m_onChangedCallback = nullptr;
    }
}