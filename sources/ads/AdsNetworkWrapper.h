#pragma once

#include "W4Common.h"

namespace w4::network::ads
{
    typedef void (*BoolCallback)(void *, bool);
    typedef void (*FloatCallback)(void *, float );

    void start();
    void complete();
    void close();
    void useCustomCloseButton(bool flag);
    bool isCustomCloseButtonAvalible();
    void CTA();

    void onVisibleChangeHandler(void * w4Self, BoolCallback callback);
    void onAudioVolumeChangeHandler(void * w4Self, FloatCallback callback);
}