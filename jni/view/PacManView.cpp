#include <string.h>  
#include <android/log.h>  
#include <sys/time.h>
#include <math.h>

#include "GameRenderer.h"
#include "GLESUtils.h"
#include "PacManView.h"
#include "ViewConstants.h"
#include "data/IntPosition.h"
#include "data/FloatPosition.h"
#include "model/PacManModel.h"

#define  LOG_TAG    "NDK_PacManView"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
  
namespace game{
    static const GLfloat BASE_PACMAN_SIZE = 0.06f;
    
    static const GLfloat defaultTriangleVerticesData[] = {
            // X, Y, Z, U, V
            0.0f, 0.0f, 0, 0.5f, 0.5f,
            0.0f, BASE_PACMAN_SIZE, 0, 0.5f, 1.0f,
            -BASE_PACMAN_SIZE, 0.0f, 0, 0.0f, 0.5f,
            
            0.0f, -BASE_PACMAN_SIZE, 0, 0.5f, 0.0f,
            0.0f, 0.0f, 0, 0.5f, 0.5f,
            -BASE_PACMAN_SIZE, 0.0f, 0, 0.0f, 0.5f,
            
            BASE_PACMAN_SIZE, 0.0f, 0, 1.0f, 0.5f,
            0.0f, 0.0f, 0, 0.5f, 0.5f,
            0.0f, -BASE_PACMAN_SIZE, 0, 0.5f, 0.0f,
            
            0.0f, BASE_PACMAN_SIZE, 0, 0.5f, 1.0f,
            0.0f, 0.0f, 0, 0.5f, 0.5f,
            BASE_PACMAN_SIZE, 0.0f, 0, 1.0f, 0.5f
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

    PacManView::PacManView( PacManModel* pacManModel ) {
        this->pacManModel = pacManModel;
        
        phase = 0.0f;
        forwardAnimation = 1;
        
        mProgram            = -1;
        mTextureID          = -1;
        muMVPMatrixHandle   = -1;
        maPositionHandle    = -1;
        maTextureHandle     = -1;
        
        memcpy(&triangleVerticesData[0], &defaultTriangleVerticesData[0], sizeof(defaultTriangleVerticesData));
    
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
        GameRenderer::createGlTextureFromResource(0x7f040002, mTextureID);
    }
    
    void PacManView::draw(glm::mat4 mProjMatrix, glm::mat4 mVMatrix) {
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
            &triangleVerticesData[0]
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
            &triangleVerticesData[3]
        );
        GLESUtils::checkGlError("glVertexAttribPointer maTextureHandle");
        glEnableVertexAttribArray(maTextureHandle);
        GLESUtils::checkGlError("glEnableVertexAttribArray maTextureHandle");
        
        struct timeval* now = new timeval();
        gettimeofday(now, NULL);
        float time = floor(now->tv_usec / 1000.0f);
        time = fmod(time, 100);
        float newPhase = time / 100;
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
        
        GLfloat vX = animPhase * BASE_PACMAN_SIZE;
        GLfloat vY = cAnimPhase * BASE_PACMAN_SIZE;
        
        GLfloat tX = 0.5f + animPhase * 0.5f;
        GLfloat tY1 = 0.5f - cAnimPhase * 0.5f;
        GLfloat tY2 = 0.5f + cAnimPhase * 0.5f;
        
        triangleVerticesData[30] = vX;
        triangleVerticesData[31] = -vY;
        triangleVerticesData[33] = tX;
        triangleVerticesData[34] = tY1;
        triangleVerticesData[55] = vX;
        triangleVerticesData[56] = vY;
        triangleVerticesData[58] = tX;
        triangleVerticesData[59] = tY2;

        float moveAnimationPhase = pacManModel->getMoveAnimationPhase();
        IntPosition* currentPathPosition = pacManModel->getCurrentPosition();
        IntPosition* positionDisplacement = pacManModel->getPositionDisplacement();

        mMMatrix = glm::mat4(1.0f);
        mMMatrix = glm::translate(
            mMMatrix,
            glm::vec3(
                ViewConstants::LEFT_TOP_CORNER->getX() + ViewConstants::PATH_CELL_SIZE_X * ((float)currentPathPosition->getX() - 1.0f + (float)positionDisplacement->getX() * moveAnimationPhase),
                ViewConstants::LEFT_TOP_CORNER->getY() - ViewConstants::PATH_CELL_SIZE_Y * ((float)currentPathPosition->getY() - 1.0f + (float)positionDisplacement->getY() * moveAnimationPhase),
                0
            )
        );
        mMMatrix = glm::rotate(mMMatrix, pacManModel->getAngle(), glm::vec3(0, 0, 1));
        mMVPMatrix = mProjMatrix * mVMatrix * mMMatrix;

        glUniformMatrix4fv(muMVPMatrixHandle, 1, false, glm::value_ptr(mMVPMatrix));
        glDrawArrays(GL_TRIANGLES, 0, 3);
        GLESUtils::checkGlError("glDrawArrays1");
        glDrawArrays(GL_TRIANGLES, 3, 3);
        GLESUtils::checkGlError("glDrawArrays2");
        glDrawArrays(GL_TRIANGLES, 6, 3);
        GLESUtils::checkGlError("glDrawArrays3");
        glDrawArrays(GL_TRIANGLES, 9, 3);
        GLESUtils::checkGlError("glDrawArrays4");
    }
}