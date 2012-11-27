#ifndef GAME_STATE_VIEW
#define GAME_STATE_VIEW

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include "data/GLColor.h"

namespace game{
    class GameModel;
    class TextString;
    
    class GameStateView {
        GameModel* gameModel;
        TextString* textString;

        static const int LVL_BUFFER_LENGTH = 6;
        char* lvlBuffer;

        public:
        GameStateView( GameModel* gameModel );
        void draw(glm::mat4 mProjMatrix, glm::mat4 mVMatrix);
    };
}

#endif
