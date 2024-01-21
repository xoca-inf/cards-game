#pragma once

#include "W4Framework.h"

W4_USE_UNSTRICT_INTERFACE


namespace games::benice::ui::utils {
    math::rect getScreenWorldRect(sptr<Camera> camera, float distance);
    float getVerticalFov(float horizontalFov, float aspect);
}