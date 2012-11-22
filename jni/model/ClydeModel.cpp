#include <jni.h>  
#include <string.h>
#include <cstdlib> 
#include <android/log.h> 
#include <math.h> 

#include "model/ClydeModel.h"
#include "model/GhostModel.h"
#include "model/PacManModel.h"
#include "data/IntPosition.h"
  
#define DEBUG_TAG "NDK_ClydeModel"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,DEBUG_TAG,__VA_ARGS__)
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,DEBUG_TAG,__VA_ARGS__)
  
namespace game{
    ClydeModel::ClydeModel ( 
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
    
    IntPosition* ClydeModel::calculateTargetPosition () {
		IntPosition* pacManPosition = pacManModel->getCurrentPosition();
		IntPosition* pacManPositionDisplacement = pacManModel->getPositionDisplacement();
		return new IntPosition (pacManPosition->getX() + pacManPositionDisplacement->getX(), pacManPosition->getY() + pacManPositionDisplacement->getY());
		/** 
		 * TODO: calculate distance to pacman if greater than eight - use default get away target point else use pacman as target
		 */
	}
}