#include <jni.h>  
#include <string.h>  
#include <android/log.h>  

#include "GameView.h"

#define DEBUG_TAG "NDK_GameView"

#include "model/GameModel.h"
#include "view/FieldView.h"
#include "view/GhostView.h"
#include "view/TabletsView.h"
#include "view/PacManView.h"
#include "view/EnergizerView.h"
#include "view/ScoreView.h"
//#include "view/LivesView.h"
//#include "view/GameStateView.h"
#include "data/IntPosition.h"
#include "data/GLColor.h"
  
namespace game{

    static GLColor BLINKY_COLOR = { 0.87f, 0.0f, 0.0f, 1.0f };
    static GLColor PINKY_COLOR = { 1.0f, 0.6f, 0.6f, 1.0f };
    static GLColor INKEY_COLOR = { 0.4f, 1.0f, 1.0f, 1.0f };
    static GLColor CLYDE_COLOR = { 1.0f, 0.6f, 0.0f, 1.0f };

    GameView::GameView(GameModel* gameModel) {
        this->gameModel = gameModel;
        
        fieldView = new FieldView();
        tabletsView = new TabletsView( gameModel->getTabletPath() );
        pacManView = new PacManView( gameModel->getPacManModel() );
        
        blinkyView = new GhostView( gameModel->getBlinkyModel(), BLINKY_COLOR );
        pinkyView = new GhostView( gameModel->getPinkyModel(), PINKY_COLOR );
        inkeyView = new GhostView( gameModel->getInkeyModel(), INKEY_COLOR );
        clydeView = new GhostView( gameModel->getClydeModel(), CLYDE_COLOR );
        
        scoreView = new ScoreView(gameModel); 
		//livesView = new LivesView(gameModel);
		
		//gameStateView = new GameStateView(gameModel);
        
        PathArray &energizerPath = gameModel->getEnergizerPath();
        int k = 0;
        for (int j = 0; j < FieldSize::WIDTH; j++) {
			for (int i = 0; i < FieldSize::HEIGHT; i++) {
				if (energizerPath[j][i] > 0)
					energizerViews[k++] = new EnergizerView( energizerPath, new IntPosition(i,j));
			}
		}
    }
    
    void GameView::draw(glm::mat4 mProjMatrix, glm::mat4 mVMatrix) {
        fieldView->draw(mProjMatrix, mVMatrix);
        tabletsView->draw(mProjMatrix, mVMatrix);
        for (int i = 0; i < 4; i++) {
			energizerViews[i]->draw(mProjMatrix, mVMatrix);
		}
        pacManView->draw(mProjMatrix, mVMatrix);
        
        blinkyView->draw(mProjMatrix, mVMatrix);
        pinkyView->draw(mProjMatrix, mVMatrix);
        inkeyView->draw(mProjMatrix, mVMatrix);
        clydeView->draw(mProjMatrix, mVMatrix);
        
        scoreView->draw(mProjMatrix, mVMatrix);
		//livesView->draw(mProjMatrix, mVMatrix);
		
		//gameStateView->draw(mProjMatrix, mVMatrix);
    }
}