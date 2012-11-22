#include <jni.h>  
#include <string.h>  
#include <android/log.h>  

#include "GLESUtils.h"
  
#define DEBUG_TAG "NDK_NativeRenderer"

#define  LOG_TAG    "NDK_GameRenderer"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
 
namespace game{

    int GLESUtils::loadShader(int shaderType, const char* source) {
        int shader = glCreateShader(shaderType);
        if (shader != 0) {
            glShaderSource(shader, 1, &source, NULL);
            glCompileShader(shader);
            int compiled = 0;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
            if (compiled == 0) {
                LOGI("Could not compile shader: %i", shaderType);
                char str[100] = "empty                                                                                              ";
                int length = 0;
                glGetShaderInfoLog(shader, 100, &length, str);
                LOGI(str);
                glDeleteShader(shader);
                shader = 0;
            }
        }
        return shader;
    }
    
    int GLESUtils::createProgram(const char* vertexSource, const char* fragmentSource) {
        int vertexShader = loadShader(GL_VERTEX_SHADER, vertexSource);
        if (vertexShader == 0) {
            return 0;
        }

        int pixelShader = loadShader(GL_FRAGMENT_SHADER, fragmentSource);
        if (pixelShader == 0) {
            return 0;
        }

        int program = glCreateProgram();
        if (program != 0) {
            glAttachShader(program, vertexShader);
            checkGlError("glAttachShader");
            glAttachShader(program, pixelShader);
            checkGlError("glAttachShader");
            glLinkProgram(program);
            int linkStatus = 0;
            glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
            if (linkStatus != GL_TRUE) {
                //Log.e(TAG, "Could not link program: ");
                //Log.e(TAG, glGetProgramInfoLog(program));
                glDeleteProgram(program);
                program = 0;
            }
        }
        return program;
    }
    
    void GLESUtils::checkGlError(const char* op) {
        for (GLint error = glGetError(); error; error
                = glGetError()) {
            LOGI("after %s() glError (0x%x)\n", op, error);
        }
    }
    
    void GLESUtils::printGLString(const char *name, GLenum s) {
        const char *v = (const char *) glGetString(s);
        LOGI("GL %s = %s\n", name, v);
    }
}