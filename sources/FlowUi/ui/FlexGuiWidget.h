#pragma once

#include "W4Framework.h"
#include "UiRect.h"

W4_USE_UNSTRICT_INTERFACE

using namespace games::benice::ui;

class FlexGuiWidget
{
public:
    FlexGuiWidget(std::string id,sptr<UiRect> uiRect, sptr<Widget> widget);
    ~FlexGuiWidget();
    const std::string & getId () const;
    sptr<UiRect> getRect() const;
    sptr<Widget> getWidget() const;
    void setClickable(bool flag);
    void setClickCallback( std::function<void()> callback);
private:
    void onTap();
    std::string m_id;
    sptr<UiRect> m_rect;
    sptr<Widget> m_widget;
    size m_guiResolution;
    std::function<void()> m_clickCallback;
    bool m_isClickable;
};
