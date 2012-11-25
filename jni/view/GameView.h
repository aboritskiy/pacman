#ifndef GAME_VIEW
#define GAME_VIEW

#include <glm/glm.hpp>

namespace game{
    class GameModel;
    class FieldView;
    class TabletsView;
    class EnergizerView;
    class PacManView;
    class GhostView;
    class ScoreView;
    class LivesView;
    //class GameStateView;
    
    class GameView {
        GameModel* gameModel;
        
        FieldView* fieldView;
        TabletsView* tabletsView;
        EnergizerView* energizerViews[4];
        PacManView* pacManView;
        
        GhostView* blinkyView;
        GhostView* pinkyView;
        GhostView* inkeyView;
        GhostView* clydeView;
        
        ScoreView* scoreView;
        LivesView* livesView;
        
        //GameStateView* gameStateView;
        
        public:
        GameView(GameModel* gameModel);
        void draw(glm::mat4 mProjMatrix, glm::mat4 mVMatrix);
    };
}

#endif 
