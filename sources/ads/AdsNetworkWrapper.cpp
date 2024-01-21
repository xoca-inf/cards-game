#include "AdsNetworkWrapper.h"

//using namespace w4::network::ads;

#ifndef __EMSCRIPTEN__
    #include <stdexcept>
#endif

#ifdef __EMSCRIPTEN__
W4_JS_IMPORT
{
    void w4AdsStart();
    void w4AdsComplete();
    void w4AdsClose();
    void w4AdsVisibleChangeHandler(void * w4Self, w4::network::ads::BoolCallback callback);
    void w4AdsAudioVolumeChangeHandler(void * w4Self, w4::network::ads::FloatCallback callback);
    void w4AdsUseCustomCloseButton(bool flag);
    bool w4AdsIsCustomButtonAvalible();
    void w4AdsCallCTA();
}

void w4::network::ads::start() { w4AdsStart(); };
void w4::network::ads::complete() { w4AdsComplete(); }
void w4::network::ads::close() { w4AdsClose(); };
void w4::network::ads::CTA() { w4AdsCallCTA(); };
void w4::network::ads::onVisibleChangeHandler(void * w4Self, w4::network::ads::BoolCallback callback) { w4AdsVisibleChangeHandler(w4Self,callback); }
void w4::network::ads::onAudioVolumeChangeHandler(void *w4Self, FloatCallback callback) { w4AdsAudioVolumeChangeHandler(w4Self,callback);}
void w4::network::ads::useCustomCloseButton(bool flag) { w4AdsUseCustomCloseButton(flag); }
bool w4::network::ads::isCustomCloseButtonAvalible() { return w4AdsIsCustomButtonAvalible();};

#else

void w4::network::ads::start() {};
    void w4::network::ads::complete() {};
    void w4::network::ads::close() {};
    void w4::network::ads::CTA() {};
    void w4::network::ads::onVisibleChangeHandler(void * w4Self, w4::network::ads::BoolCallback callback){};
    void w4::network::ads::onAudioVolumeChangeHandler(void *w4Self, FloatCallback callback) { };
    void w4::network::ads::useCustomCloseButton(bool flag) {};
    bool w4::network::ads::isCustomCloseButtonAvalible() { return false;};

#endif