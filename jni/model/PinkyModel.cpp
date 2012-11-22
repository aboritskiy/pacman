#include <jni.h>  
#include <string.h>
#include <cstdlib> 
#include <android/log.h> 
#include <math.h> 

#include "model/PinkyModel.h"
#include "model/GhostModel.h"
#include "model/PacManModel.h"
#include "data/IntPosition.h"
  
#define DEBUG_TAG "NDK_PinkyModel"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,DEBUG_TAG,__VA_ARGS__)
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,DEBUG_TAG,__VA_ARGS__)
  
namespace game{
    PinkyModel::PinkyModel ( 
            PathArray &path,
            int permissionLevel,
            int defaultHomePermission,
            PacManModel* pacManModel,
            IntPosition* initialPosition,
            IntPosition* scatterPosition,
            int tabletsToLeaveHome
        ) : GhostModel (
            path,
            permissionLevel,
            defaultHomePermission,
            pacManModel,
            initialPosition,
            scatterPosition,
            tabletsToLeaveHome
        ) {
    }
    
    IntPosition* PinkyModel::calculateTargetPosition () {
		IntPosition* pacManPosition = pacManModel->getCurrentPosition();
		IntPosition* pacManPositionDisplacement = pacManModel->getPositionDisplacement();
		return new IntPosition (
            pacManPosition->getX() + 4 * pacManPositionDisplacement->getX(),
            pacManPosition->getY() + 4 * pacManPositionDisplacement->getY()
        );
	}
}