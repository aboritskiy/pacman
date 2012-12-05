#ifndef IGHOST_EATEN_HANDLER
#define IGHOST_EATEN_HANDLER

namespace game{
	class IntPosition;
 
    class IGhostEatenHandler {
        public:
    	//virtual ~IGhostEatenHandler();

    	virtual void handleGhostEaten (IntPosition* ghostPosition, int score);
    };
}

#endif 
