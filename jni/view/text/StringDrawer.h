#ifndef STRING_DRAWER
#define STRING_DRAWER

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include "data/GLColor.h"

namespace game{
    class TextureRegion;
    
    class StringDrawer {
        static const int FLOAT_SIZE_BYTES = 4;
        static const int TRIANGLE_VERTICES_DATA_STRIDE_BYTES = 5 * FLOAT_SIZE_BYTES;
        static const int TRIANGLE_VERTICES_DATA_POS_OFFSET = 0;
        static const int TRIANGLE_VERTICES_DATA_UV_OFFSET = 3;

        glm::mat4 mMMatrix;
        glm::mat4 mMVPMatrix;

        int mProgram;
        int muMVPMatrixHandle;
        int maPositionHandle;
        int maTextureHandle;
        int colorHandle;
        
        GLColor color;
    
        public:
        StringDrawer();
        void setColor (float &red, float &green, float &blue, float &alpha);
        void setColor (const GLColor &color);
        void draw (glm::mat4 mProjMatrix, glm::mat4 mVMatrix, float x, float y, float width, float height, TextureRegion* textureRegion);
        void draw (glm::mat4 mProjMatrix, glm::mat4 mVMatrix);
    };
}

#endif 