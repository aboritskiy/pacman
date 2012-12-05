#ifndef IGAME_OVER_HANDLER
#define IGAME_OVER_HANDLER

namespace game{
	class IntPosition;
 
    class IGameOverHandler {
        public:
    	//virtual ~IGameOverHandler();

    	virtual void handleGameOver ();
    };
}

#endif 
