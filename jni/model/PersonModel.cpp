#include <jni.h>  
#include <string.h>  
#include <android/log.h>  

#include "PersonModel.h"
#include "data/IntPosition.h"
#include "data/MotionDirection.h"
  
#define DEBUG_TAG "NDK_PersonModel"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,DEBUG_TAG,__VA_ARGS__)
  
namespace game{

    PersonModel::PersonModel( PathArray &aPath, int aPermissionLevel ) :
        path(aPath),
        permissionLevel(aPermissionLevel)
    { 
        reset(0L);
    }
    
    IntPosition* PersonModel::getCurrentPosition () {
        return currentPosition;
    }
    
    IntPosition* PersonModel::getNextPosition () { 
        return nextPosition;
    }
    
    void PersonModel::setMotionDirection ( MotionDirection* md ) {
        desiredMotionDirection = md;
        
    }
    
    void PersonModel::step(long time) {
    }
    
    void PersonModel::reset(long time) {
    }
}