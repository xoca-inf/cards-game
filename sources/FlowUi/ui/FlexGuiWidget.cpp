#include "FlexGuiWidget.h"

FlexGuiWidget::FlexGuiWidget(std::string id, sptr<UiRect> uiRect, sptr<Widget> widget) : m_id(id),m_rect(uiRect), m_widget(widget)
{
    m_guiResolution = getVirtualResolution();
    m_widget->setSizePolicy(SizePolicy::Fixed,SizePolicy::Fixed);
    m_widget->onTap([this](){
        onTap();
    });
    m_rect->setOnChangeHandler(
            [this](math::rect newRect) {
                m_guiResolution = getVirtualResolution();
                size screenSize = Render::getSize();

                ivec2 pos = ivec2( newRect.position.x * m_guiResolution.w / screenSize.w, (screenSize.h - newRect.position.y) * m_guiResolution.h / screenSize.h );
                ivec2 sz = ivec2( newRect.size.x * m_guiResolution.w / screenSize.w, ( newRect.size.y) * m_guiResolution.h / screenSize.h  ) ;

                m_widget->setPosition(pos);
                m_widget->setSize(sz);
            }

            );
}

FlexGuiWidget::~FlexGuiWidget()
{
    m_rect = nullptr;
    if (m_widget != nullptr)
    {
        m_widget->setVisible(false);
        m_widget = nullptr;
    }
}

const std::string& FlexGuiWidget::getId() const
{
    return m_id;
}

sptr<UiRect> FlexGuiWidget::getRect() const
{
    return m_rect;
}

sptr<Widget> FlexGuiWidget::getWidget() const
{
    return m_widget;
}

void FlexGuiWidget::setClickable(bool flag)
{
    m_isClickable = flag;
}

void FlexGuiWidget::onTap ()
{
    if (m_clickCallback!= nullptr)
    {
        m_clickCallback();
    }
}
void FlexGuiWidget::setClickCallback(std::function<void()> callback)
{
    m_clickCallback = callback;
}