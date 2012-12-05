#ifndef GL_ES_UTILS
#define GL_ES_UTILS

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <time.h>

namespace game{
    class GLESUtils {


        public:
       
            static int loadShader(int shaderType, const char* source);
            static int createProgram(const char* vertexSource, const char* fragmentSource);
            
            static void checkGlError(const char* op);
            static void printGLString(const char *name, GLenum s);
            static long getTime();
            
            /*static void loadFont(
                const char* fontName,
                int height,
                int padX,
                int padY,
                float &fontHeight,
                float &fontAscent,
                float &fontDescent,
                );*/
    };
}

#endif 
