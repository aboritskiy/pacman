#ifndef GAME_STATE
#define GAME_STATE

namespace game{
    class GameState {
        private:
        long duration;
        int nextStateIndex;
    
        public:
        GameState( long duration, int nextStateIndex );
        
        long getDuration();
        GameState* getNextState();
        
        static GameState* LEVEL_INTRO;
        static GameState* GAME;
        static GameState* LEVEL_COMPLETED;
        static GameState* GAME_OVER;
        
        static GameState* POSSIBLE_GAME_STATES[4];
    };
}

#endif 