#ifndef CLYDE_MODEL
#define CLYDE_MODEL

#include "model/GhostModel.h"
#include "data/FieldSize.h"

namespace game{
    class IntPosition;
    class PacManModel;
    class TileType;
 
    class ClydeModel : public GhostModel{
        private:
        IntPosition* calculateTargetPosition ();
    
        public:
        ClydeModel ( 
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