#include <jni.h>  
#include <string.h>  
#include <android/log.h>  
#include "GameRenderer.h"
#include "data/MotionDirection.h"

#define  LOG_TAG    "NDK_NativeRenderer"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

using namespace game;

namespace {
    GameRenderer* toGameRenderer(jlong gameRendererPtr) {
        return reinterpret_cast<GameRenderer*>(gameRendererPtr);
    }
}

extern "C" {

    JavaVM *g_jvm;

    JNIEXPORT jlong JNICALL Java_ru_tonybo_pacmanJNI_NativeRenderer_create(JNIEnv* env, jobject jparent) {
        GameRenderer* gameRenderer = new GameRenderer(env, jparent, g_jvm);
        return reinterpret_cast<jlong>(gameRenderer);
    }

    JNIEXPORT void JNICALL Java_ru_tonybo_pacmanJNI_NativeRenderer_destroy(JNIEnv* env, jobject thiz, jlong gameRendererPtr) {
        delete toGameRenderer(gameRendererPtr);
    }

    JNIEXPORT void JNICALL Java_ru_tonybo_pacmanJNI_NativeRenderer_onSurfaceCreated(JNIEnv* env, jobject thiz, jlong gameRendererPtr) {
        toGameRenderer(gameRendererPtr)->onSurfaceCreated();
    }
    
    JNIEXPORT void JNICALL Java_ru_tonybo_pacmanJNI_NativeRenderer_onDrawFrame(JNIEnv* env, jobject thiz, jlong gameRendererPtr) {
        toGameRenderer(gameRendererPtr)->onDrawFrame();
    }
    
    JNIEXPORT void JNICALL Java_ru_tonybo_pacmanJNI_NativeRenderer_onSurfaceChanged(JNIEnv* env, jobject thiz, jlong gameRendererPtr, jint width, jint height) {
        toGameRenderer(gameRendererPtr)->onSurfaceChanged(width, height);
    }
    
    JNIEXPORT void JNICALL Java_ru_tonybo_pacmanJNI_NativeRenderer_setMotionDirection(JNIEnv* env, jobject thiz, jlong gameRendererPtr, jint mdIndex) {
        switch (mdIndex) {
            case 0:
                toGameRenderer(gameRendererPtr)->setMotionDirection(MotionDirection::NONE);
                break;
            case 1:
                toGameRenderer(gameRendererPtr)->setMotionDirection(MotionDirection::LEFT);
                break;
            case 2:
                toGameRenderer(gameRendererPtr)->setMotionDirection(MotionDirection::RIGHT);
                break;
            case 3:
                toGameRenderer(gameRendererPtr)->setMotionDirection(MotionDirection::UP);
                break;
            case 4:
                toGameRenderer(gameRendererPtr)->setMotionDirection(MotionDirection::DOWN);
                break;
        }
        
    }
    
    JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *jvm, void *reserved) {
        g_jvm = jvm;  // cache the JavaVM pointer
        
        LOGI("JNI_OnLoad");

        return JNI_VERSION_1_6;
    }
}