#include "Config.h"
#include "Settings.h"
#include "Render.h"

ConfigData Config::m_data;

W4_JS_IMPORT int getWindowSizeWidth();
W4_JS_IMPORT int getWindowSizeHeight();

void Config::reload()
{
    ConfigData oldData = Config::getData();

    size sz = Render::getSize();
    
    //W4_LOG_DEBUG("screen size {%d, %d}", sz.w, sz.h);

    /*int w = getWindowSizeWidth();
    int h = getWindowSizeHeight();*/

    int w = sz.w;
    int h = sz.h;
    //W4_LOG_DEBUG("win size {%d, %d}", w, h);

    auto autoDetectBuild = [&w, &h]() {
        std::string build;
        if (w > h)
        {
            build = "landscape";
        }
        else
        {
            build = "portrait";
        }
        return build;
    };

    std::string build = "portrait";;
    if (Settings::instance()->hasString("force_orientation")) {
        build = Settings::instance()->getString("force_orientation");
        if (build != "portrait" && build != "landscape") {
            W4_LOG_ERROR("WRONG WINDOW ORIENTATION BUILD");
            build = autoDetectBuild();
        }
    } else {
        build = autoDetectBuild();
    }

    if (build == "portrait") {
        m_data.aspect = size(h, w);
        m_data.guiResolution = size(h, w);
    }
    else if (build == "landscape")
    {
        m_data.aspect = size(w, h);
        m_data.guiResolution = size(w, h);
    }
    
    m_data.orientation = build;

    SettingsNode* orientationSetting = OrientationSettings::instance().get();

    if (Settings::instance()->hasInt("is_aspect_from_orientation")
        && Settings::instance()->getInt("is_aspect_from_orientation")) 
    {
        //W4_LOG_DEBUG("is_aspect_from_orientation");
        const vec2& aspect = orientationSetting->getVec2("aspect");
        m_data.aspect = size(aspect.x, aspect.y);
    }

    if (Settings::instance()->hasInt("is_resolution_from_orientation")
        && Settings::instance()->getInt("is_resolution_from_orientation")) 
    {
        //W4_LOG_DEBUG("is_resolution_from_orientation");
        const vec2& gui_resolution = orientationSetting->getVec2("resolution");
        m_data.guiResolution = size(gui_resolution.x, gui_resolution.y);
    }

    //Render::getScreenCamera()->setFov(90);
    //const math::size prev_resolution = w4::gui::getVirtualResolution();
    //const float prev_aspect = Render::getAspect();

    /*if (oldData.aspect != m_data.aspect) 
    {
        W4_LOG_DEBUG("changed win aspect {%d, %d}", m_data.aspect.w, m_data.aspect.h);
        w4::platform::Platform::setWindowAspect(m_data.aspect.w, m_data.aspect.h);
    }

    if (oldData.orientation != m_data.orientation) 
    {
        W4_LOG_DEBUG("changed win orientation {%s}", m_data.orientation.c_str());
        w4::platform::Platform::setWindowStyle(m_data.orientation.c_str());
    }

    if (oldData.guiResolution != m_data.guiResolution) 
    {
        W4_LOG_DEBUG("changed win resolution {%d, %d}", m_data.guiResolution.w, m_data.guiResolution.h);
        w4::gui::setVirtualResolution(m_data.guiResolution);
    }*/
}

const ConfigData& Config::getData() {
    return m_data;
}