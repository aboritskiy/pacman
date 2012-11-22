#ifndef TEXT_STRING
#define TEXT_STRING

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include "data/GLColor.h"

namespace game{
    class TextureRegion;
    class StringDrawer;
    
    class TextString {
        static const int CHAR_START = 32;
        static const int CHAR_END = 126;
        static const int CHAR_CNT = ( ( ( CHAR_END - CHAR_START ) + 1 ) + 1 );
        
        static const int CHAR_NONE = 32;           
        static const int CHAR_UNKNOWN = ( CHAR_CNT - 1 );  
        
        static const int FONT_SIZE_MIN = 6;
        static const int FONT_SIZE_MAX = 180;
        
        static const int CHAR_BATCH_SIZE = 100;
        
        float fontHeight;
        float fontAscent;
        float fontDescent;
        
        int textureId;
        int textureSize;
        TextureRegion* textureRgn;
        
        float charWidthMax;
        float charHeight;
        float charWidths[CHAR_CNT];
        TextureRegion* charRgn[CHAR_CNT];
        int cellWidth;
        int cellHeight;
        int rowCnt;
        int colCnt;
        
        float scaleX;
        float scaleY;
        float spaceX;
        
        int padX;
        int padY;
        
        StringDrawer* stringDrawer;
        
        public:
        TextString();
        char loadFont (const char* fileName, const int height, const int padX, const int padY);
        void setColor (float &red, float &green, float &blue, float &alpha);
        void setColor (const GLColor &color);
        void setScale (float scale);
        void draw(const char* text, const float x, const float y, const glm::mat4 mProjMatrix, const glm::mat4 mVMatrix);
    };
}

#endif
