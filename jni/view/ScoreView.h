#ifndef SCORE_VIEW
#define SCORE_VIEW

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include "data/GLColor.h"

namespace game{
    class GameModel;
    class TextString;
    
    class ScoreView {
        GameModel* gameModel;
        TextString* textString;

        static const int SCORE_LENGTH = 6;
        char* scoreBuffer;

        public:
        ScoreView( GameModel* gameModel );
        void draw(glm::mat4 mProjMatrix, glm::mat4 mVMatrix);
    };
}

#endif
