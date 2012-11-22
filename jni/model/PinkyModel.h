#ifndef PINKY_MODEL
#define PINKY_MODEL

#include "model/GhostModel.h"
#include "data/FieldSize.h"

namespace game{
    class IntPosition;
    class PacManModel;
    class TileType;
 
    class PinkyModel : public GhostModel{
        private:
        IntPosition* calculateTargetPosition ();
    
        public:
        PinkyModel ( 
            PathArray &path,
            int permissionLevel,
            int defaultHomePermission,
            PacManModel* pacManModel,
            IntPosition* initialPosition,
            IntPosition* scatterPosition,
            int tabletsToLeaveHome
        );
    };
}

#endif 