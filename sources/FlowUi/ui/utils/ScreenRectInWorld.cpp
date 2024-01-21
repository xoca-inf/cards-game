#include "ScreenRectInWorld.h"

namespace games::benice::ui::utils {

    static constexpr float HALF_DEG2RAD = w4::math::PI / 360.0f;
    static constexpr float DOUBLE_RAD2DEG = 360.0f / w4::math::PI;


    float getVerticalFov(float horizontalFov, float aspect) {
        return atan(tan(horizontalFov * HALF_DEG2RAD) * 1 / aspect) * DOUBLE_RAD2DEG;
    }

    math::rect getScreenWorldRect(sptr<Camera> camera, float distance) {
        math::rect result;

        vec3 cameraPos = camera->getWorldTranslation();
        float fowAngle = camera->getFov() / 2.0;

        float aspect = Render::getAspect();

        float wAngle = (camera->getFov() / 2.0) * math::DEG2RAD;
        float hAngle = (getVerticalFov(camera->getFov(), aspect) / 2.0f) * math::DEG2RAD;//fowAngle/aspect*math::DEG2RAD;

        vec3 camToLeftLimitDirection = camera->getWorldForward() * Rotator(vec3(0.0f, wAngle, 0.0f));
        vec3 camToRightLimitDirection = camera->getWorldForward() * Rotator(vec3(0.0f, -wAngle, 0.0f));

        vec3 camToLeftLimit = cameraPos + camToLeftLimitDirection * distance * (1.0f / cos(wAngle));
        vec3 camToRightLimit = cameraPos + camToRightLimitDirection * distance * (1.0f / cos(wAngle));

        vec3 camToTopLimitDirection = camera->getWorldForward() * Rotator(vec3(hAngle, 0.0f, 0.0f));
        vec3 camToDownLimitDirection = camera->getWorldForward() * Rotator(vec3(-hAngle, 0.0f, 0.0f));

        vec3 camToTopLimit = cameraPos + camToTopLimitDirection * distance * (1.0f / cos(hAngle));
        vec3 camToDownLimit = cameraPos + camToDownLimitDirection * distance * (1.0 / cos(hAngle));

        vec3 centerPoint = camToTopLimit + (camToDownLimit - camToTopLimit) * 0.5f;

        float w = (camToLeftLimit - camToRightLimit).length();
        float h = (camToTopLimit - camToDownLimit).length();

        result = math::rect(centerPoint.x, centerPoint.y, w, h);

        //W4_LOG_DEBUG("scren rect {%f, %f, %f, %f}", result.position.x, result.position.y, result.size.w, result.size.h);

        return result;
    }
}