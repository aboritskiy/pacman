#include <string.h>  
#include <android/log.h>  

#include "ScoreView.h"
#include "model/GameModel.h"
#include "view/text/TextString.h"

#define  LOG_TAG    "NDK_ScoreView"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
  
namespace game{
    static GLColor LABEL_COLOR = { 1.0f, 1.0f, 1.0f, 1.0f };
    static GLColor VALUE_COLOR = { 1.0f, 1.0f, 0.0f, 1.0f };

    ScoreView::ScoreView( GameModel* gameModel ) : gameModel ( gameModel ) {
        textString = new TextString();
		textString->loadFont("typodermic_foo_regular.ttf", 50, 2, 2);
		textString->setScale(0.003f);

		scoreBuffer = new char[SCORE_LENGTH];
    }
    
    void ScoreView::draw(glm::mat4 mProjMatrix, glm::mat4 mVMatrix) {
        textString->setColor(LABEL_COLOR);
		textString->draw("Score", -1.0f, -1.2f, mProjMatrix, mVMatrix);
		textString->setColor(VALUE_COLOR);
		snprintf ( scoreBuffer, SCORE_LENGTH, "%d", gameModel->getScore());
		textString->draw(scoreBuffer, -0.55f, -1.2f, mProjMatrix, mVMatrix);
    }
}
