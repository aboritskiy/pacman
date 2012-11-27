#include <string.h>  
#include <android/log.h>  

#include "GameRenderer.h"
#include "GLESUtils.h"
#include "EnergizerView.h"
#include "data/FloatPosition.h"
#include "data/IntPosition.h"
#include "view/ViewConstants.h"

#define  LOG_TAG    "NDK_EnergizerView"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
  
namespace game{
    static const GLfloat BASE_TABLET_SIZE = 0.02f;
    static const GLfloat TRIANGLE_SIDE = BASE_TABLET_SIZE * (sqrt(2) + 1);
    static const GLfloat TEXTURE_BASE_DISP = (1/sqrt(2));
    static const GLfloat MAX_SCALE = 2.0f;

    static GLfloat mTriangleVerticesData[] = {
        // X, Y, Z, U, V
        -BASE_TABLET_SIZE, TRIANGLE_SIDE, 0, 0.0f, -TEXTURE_BASE_DISP,
        -BASE_TABLET_SIZE, -BASE_TABLET_SIZE, 0, 0.0f, 1.0f,
        TRIANGLE_SIDE, -BASE_TABLET_SIZE, 0, TEXTURE_BASE_DISP + 1.0f, 1.0f
    };
                
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

    EnergizerView::EnergizerView( PathArray &tabletPath, IntPosition* modelPosition ) : tabletPath ( tabletPath ) {
        this->modelPosition = modelPosition;
        mProgram            = -1;
        mTextureID          = -1;
        muMVPMatrixHandle   = -1;
        maPositionHandle    = -1;
        maTextureHandle     = -1;
        
        phase = 0.0f;
        forwardAnimation = 1;
    
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
        GameRenderer::createGlTextureFromResource(0x7f040005, mTextureID);
    }
    
    void EnergizerView::draw(glm::mat4 mProjMatrix, glm::mat4 mVMatrix) {
    	if (tabletPath[modelPosition->getY()][modelPosition->getX()] == 0)
    	    return;

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
        
        struct timeval* now = new timeval();
        gettimeofday(now, NULL);
        float time = floor(now->tv_usec / 1000.0f);
        time = fmod(time, 400);
        float newPhase = time / 400;
        float animPhase;
        float cAnimPhase;
        if (newPhase < phase) {
        	forwardAnimation = !forwardAnimation;
        }
        if (forwardAnimation) {
    		animPhase = 1.0f - 0.5f * newPhase;
    	} else {
    		animPhase = 0.5f + 0.5f * newPhase;
    	}
        cAnimPhase = 1.0f - animPhase;
        
        phase = newPhase;
        
        mMMatrix = glm::mat4(1.0f);
        mMMatrix = glm::translate(
            mMMatrix,
            glm::vec3(
                ViewConstants::LEFT_TOP_CORNER->getX() + ViewConstants::PATH_CELL_SIZE_X * (modelPosition->getX() - 1),
                ViewConstants::LEFT_TOP_CORNER->getY() - ViewConstants::PATH_CELL_SIZE_Y * (modelPosition->getY() - 1),
                0
            )
        );
        mMMatrix = glm::scale(
            mMMatrix,
            glm::vec3((MAX_SCALE - 1) * cAnimPhase + 1)
        );

        mMVPMatrix = mProjMatrix * mVMatrix * mMMatrix;
        glUniformMatrix4fv(muMVPMatrixHandle, 1, false, glm::value_ptr(mMVPMatrix));
        glDrawArrays(GL_TRIANGLES, 0, 3);
        GLESUtils::checkGlError("glDrawArrays");
    }
}
