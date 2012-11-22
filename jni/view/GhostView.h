#ifndef GHOST_VIEW
#define GHOST_VIEW

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include "data/GLColor.h"

namespace game{
    class GhostModel;
    class IntPosition;

    class GhostView {
        static const int FLOAT_SIZE_BYTES = 4;
        static const int TRIANGLE_VERTICES_DATA_STRIDE_BYTES = 5 * FLOAT_SIZE_BYTES;
        static const int TRIANGLE_VERTICES_DATA_POS_OFFSET = 0;
        static const int TRIANGLE_VERTICES_DATA_UV_OFFSET = 3;
        
        static const long FRIGHTEN_BLINK_DURATION = 3000L;
        
        GLfloat triangleVerticesData[240];

        glm::mat4 mMMatrix;
        glm::mat4 mMVPMatrix;

        int mProgram;
        int mTextureID;
        int eyeTextureID;
        int muMVPMatrixHandle;
        int maPositionHandle;
        int maTextureHandle;
        int basicColorHandle;
        int targetColorHandle;
        
        GhostModel* ghostModel;
        GLColor &GHOST_COLOR;
        
        int GHOST_EATEN_DISPLAY_TIME;
        float GHOST_EATEN_COLOR[];
        
        float phase;
        char forwardAnimation;
    
        public:
        GhostView( GhostModel* ghostModel, GLColor &ghostColor );
        void handleGhostEaten (IntPosition* ghostPosition, int score);
        void draw(glm::mat4 mProjMatrix, glm::mat4 mVMatrix);
    };
}

#endif 