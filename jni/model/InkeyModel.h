#ifndef INKEY_MODEL
#define INKEY_MODEL

#include "model/GhostModel.h"
#include "data/FieldSize.h"

namespace game{
    class IntPosition;
    class PacManModel;
    class TileType;
 
    class InkeyModel : public GhostModel{
        private:
        IntPosition* calculateTargetPosition ();
        GhostModel* blinkyModel;
    
        public:
        InkeyModel ( 
            PathArray &path,
            int permissionLevel,
            int defaultHomePermission,
            PacManModel* pacManModel,
            IntPosition* initialPosition,
            IntPosition* scatterPosition,
            int tabletsToLeaveHome,
            GhostModel* blinkyModel
        );
    };
}

#endif 