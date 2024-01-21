#pragma once

#include "W4Math.h"

/**
 *  https://github.com/Calinou/fov source of math
 */
namespace games::benice::utils
{
    class AdaptiveFov
    {
    public:
        static constexpr float DEFAULT_HFOV = 60.0f;
        static constexpr float DEFAULT_VFOV = 60.0f;
        
        static constexpr float HALF_DEG2RAD = w4::math::PI / 360.0f;
        static constexpr float DOUBLE_RAD2DEG = 360.0f / w4::math::PI;

        static double roundUp(double value, int decimal_places = 10) {
            const double multiplier = std::pow(10.0, decimal_places);
            return std::ceil(value * multiplier) / multiplier;
        }

        static float getVerticalFov(float horizontalFov, float aspect)
        {
            return  roundUp(atan(tan(horizontalFov*HALF_DEG2RAD)* 1/aspect)) * DOUBLE_RAD2DEG;
        }

        static float getAdaptiveFov (float aspect)
        {
            //return 45.0f;
            if (aspect <= 1.0f)
            {
                return DEFAULT_HFOV;
            }
            
            float hFov = roundUp(atan(tan(DEFAULT_VFOV * HALF_DEG2RAD))) * DOUBLE_RAD2DEG;
            float hFovNew = roundUp(atan(tan(hFov * HALF_DEG2RAD) * aspect)) * DOUBLE_RAD2DEG;
            return hFovNew;
        }
        
        static float getAdaptiveFov (w4::math::size sz)
        {
            //return 45.0f;
            return getAdaptiveFov(roundUp((float) sz.w / (float) sz.h, 3));
        }
        
        static float getAdaptiveFov (w4::math::ivec2 sz)
        {
            //return 45.0f;
            return getAdaptiveFov(roundUp((float) sz.w / (float) sz.h, 3));
        }
    };
}