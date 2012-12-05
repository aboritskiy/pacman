#include <string.h>  
#include <android/log.h>  

#include "GameStateView.h"
#include "model/GameModel.h"
#include "view/text/TextString.h"
#include "data/GameState.h"

#define  LOG_TAG    "NDK_GameStateView"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
  
namespace game{
    static GLColor INTRO_COLOR = { 1.0f, 1.0f, 1.0f, 1.0f };
    static GLColor VALUE_COLOR = { 1.0f, 1.0f, 0.0f, 1.0f };
    static GLColor GAME_OVER_COLOR = { 1.0f, 0.0f, 0.0f, 1.0f };

    GameStateView::GameStateView( GameModel* gameModel ) : gameModel ( gameModel ) {
        textString = new TextString();
		textString->loadFont("typodermic_foo_regular.ttf", 50, 2, 2);
		textString->setScale(0.003f);

		lvlBuffer = new char[LVL_BUFFER_LENGTH];
    }
    
    void GameStateView::draw(glm::mat4 mProjMatrix, glm::mat4 mVMatrix) {
		GameState* gameState = gameModel->getGameState();
		if (gameState == GameState::GAME) {
			return;
		} else if (gameState == GameState::LEVEL_INTRO) {
			textString->setColor(INTRO_COLOR);
			textString->draw("GET READY!", -0.34f, -0.2f, mProjMatrix, mVMatrix);
		} else if (gameState == GameState::LEVEL_COMPLETED) {
			textString->setColor(INTRO_COLOR);
			textString->draw("Level", -0.25f, -0.2f, mProjMatrix, mVMatrix);
			snprintf ( lvlBuffer, LVL_BUFFER_LENGTH, "%02d", gameModel->getLevel() + 1);
			textString->setColor(VALUE_COLOR);
			textString->draw(lvlBuffer, 0.1f, -0.2f, mProjMatrix, mVMatrix);
		} else if (gameState == GameState::GAME_OVER) {
			textString->setColor(GAME_OVER_COLOR);
			textString->draw("GAME OVER", -0.34f, -0.2f, mProjMatrix, mVMatrix);
		}
    }
}
