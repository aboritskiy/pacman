#include <string.h>  
#include <android/log.h>  

#include "GameRenderer.h"
#include "GLESUtils.h"
#include "FieldView.h"

#define  LOG_TAG    "NDK_FieldView"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
  
namespace game{
    static GLfloat mTriangleVerticesData[] = {
		// X, Y, Z, U, V
		-1.0f, -1.0f, 0, 0.0f, 1.0f,
		1.0f, -1.0f, 0, 1.0f, 1.0f,
		1.0f, 1.0f, 0, 1.0f, 0.0f,
		-1.0f, 1.0f, 0, 0.0f, 0.0f,
		-1.0f, -1.0f, 0, 0.0f, 1.0f };
                
    static const char mVertexShader[] =
		"uniform mat4 uMVPMatrix;\n"
		"attribute vec4 aPosition;\n"
		"attribute vec2 aTextureCoord;\n"
		"varying vec2 vTextureCoord;\n"
		"void main() {\n"
		"  gl_Position = uMVPMatrix * aPosition;\n"
		"  vTextureCoord = aTextureCoord;\n"
		"}\n";
        
    static const char mFragmentShader[] =
		"precision mediump float;\n"
		"varying vec2 vTextureCoord;\n"
		"uniform sampler2D sTexture;\n"
		"void main() {\n"
		"  gl_FragColor = texture2D(sTexture, vTextureCoord);\n"
		"}\n";

    FieldView::FieldView() {
        mProgram            = -1;
        mTextureID          = -1;
        muMVPMatrixHandle   = -1;
        maPositionHandle    = -1;
        maTextureHandle     = -1;
    
        mProgram = GLESUtils::createProgram(mVertexShader, mFragmentShader);
        GLESUtils::checkGlError("createProgram mProgram");
        if (mProgram == 0) {
            LOGI("program not created exiting");
            return;
        }
        maPositionHandle = glGetAttribLocation(mProgram, "aPosition");
        GLESUtils::checkGlError("glGetAttribLocation aPosition");
        if (maPositionHandle == -1) {
            return;
        }
        maTextureHandle = glGetAttribLocation(mProgram, "aTextureCoord");
        GLESUtils::checkGlError("glGetAttribLocation aTextureCoord");
        if (maTextureHandle == -1) {
            return;
        }

        muMVPMatrixHandle = glGetUniformLocation(mProgram, "uMVPMatrix");
        GLESUtils::checkGlError("glGetUniformLocation uMVPMatrix");
        if (muMVPMatrixHandle == -1) {
            return;
        }

        /*
         * Create our texture. This has to be done each time the
         * surface is created.
         */

        GLuint textures[] = {0};
        glGenTextures(1, &textures[0]);

        mTextureID = textures[0];
        glBindTexture(GL_TEXTURE_2D, mTextureID);

        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        // resource id is from auto generated java file "R.java"
        LOGI("createGlTextureFromResource");
        GameRenderer::createGlTextureFromResource(0x7f040004, mTextureID);
    }
    
    void FieldView::draw(glm::mat4 mProjMatrix, glm::mat4 mVMatrix) {
        glUseProgram(mProgram);
        GLESUtils::checkGlError("glUseProgram");

        glActiveTexture(GL_TEXTURE0);
        GLESUtils::checkGlError("glActiveTexture");
        
        glBindTexture(GL_TEXTURE_2D, mTextureID);
        GLESUtils::checkGlError("glBindTexture");

        glVertexAttribPointer(
            maPositionHandle,
            3,
            GL_FLOAT,
            GL_FALSE,
            TRIANGLE_VERTICES_DATA_STRIDE_BYTES,
            &mTriangleVerticesData[0]
        );
        GLESUtils::checkGlError("glVertexAttribPointer maPositionHandle");
        glEnableVertexAttribArray(maPositionHandle);
        GLESUtils::checkGlError("glEnableVertexAttribArray maPositionHandle");
        glVertexAttribPointer(
            maTextureHandle, 
            2,
            GL_FLOAT,
            GL_FALSE,
            TRIANGLE_VERTICES_DATA_STRIDE_BYTES,
            &mTriangleVerticesData[3]
        );
        GLESUtils::checkGlError("glVertexAttribPointer maTextureHandle");
        glEnableVertexAttribArray(maTextureHandle);
        GLESUtils::checkGlError("glEnableVertexAttribArray maTextureHandle");

        mMMatrix = glm::mat4(1.0f);
        mMVPMatrix = mProjMatrix * mVMatrix * mMMatrix;

        glUniformMatrix4fv(muMVPMatrixHandle, 1, false, glm::value_ptr(mMVPMatrix));
        glDrawArrays(GL_TRIANGLES, 0, 3);
        GLESUtils::checkGlError("glDrawArrays1");
        glDrawArrays(GL_TRIANGLES, 2, 3);
        GLESUtils::checkGlError("glDrawArrays2");
    }
}
