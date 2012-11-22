#include <jni.h>  
#include <string.h>
#include <cstdlib> 
#include <android/log.h> 
#include <math.h> 

#include "model/InkeyModel.h"
#include "model/GhostModel.h"
#include "model/PacManModel.h"
#include "data/IntPosition.h"
  
#define DEBUG_TAG "NDK_InkeyModel"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,DEBUG_TAG,__VA_ARGS__)
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,DEBUG_TAG,__VA_ARGS__)
  
namespace game{
    InkeyModel::InkeyModel ( 
            PathArray &path,
            int permissionLevel,
            int defaultHomePermission,
            PacManModel* pacManModel,
            IntPosition* initialPosition,
            IntPosition* scatterPosition,
            int tabletsToLeaveHome,
            GhostModel* blinkyModel
        ) : GhostModel (
            path,
            permissionLevel,
            defaultHomePermission,
            pacManModel,
            initialPosition,
            scatterPosition,
            tabletsToLeaveHome
        ) {
        
        this->blinkyModel = blinkyModel;
    }
    
    IntPosition* InkeyModel::calculateTargetPosition () {
		IntPosition* pacManPosition = pacManModel->getCurrentPosition();
		IntPosition* pacManPositionDisplacement = pacManModel->getPositionDisplacement();
        IntPosition* blinkyPosition = blinkyModel->getCurrentPosition();
		return new IntPosition (
            (pacManPosition->getX() + 2 * pacManPositionDisplacement->getX() - blinkyPosition->getX()) * 2 + blinkyPosition->getX(),
            (pacManPosition->getY() + 2 * pacManPositionDisplacement->getY() - blinkyPosition->getY()) * 2 + blinkyPosition->getY()
        );
	}
}