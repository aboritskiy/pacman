#include <string.h>  
#include <android/log.h>  
#include <sys/time.h>
#include <math.h>

#include "GameRenderer.h"
#include "GLESUtils.h"
#include "GhostView.h"
#include "ViewConstants.h"
#include "data/IntPosition.h"
#include "data/FloatPosition.h"
#include "model/GhostModel.h"

#define  LOG_TAG    "NDK_GhostView"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
  
namespace game{
    static const GLfloat BASE_PACMAN_SIZE = 0.06f;
    
    static const int FLOAT_SIZE_BYTES = 4;
    static const int TRIANGLE_VERTICES_DATA_STRIDE_BYTES = 5 * FLOAT_SIZE_BYTES;
    static const int TRIANGLE_VERTICES_DATA_POS_OFFSET = 0;
    static const int TRIANGLE_VERTICES_DATA_UV_OFFSET = 3;

    static const float BASE_SIZE = 0.06f;
           
    static const float BASE_EYE_SIZE = 0.025f;
    static const float BASE_EYE_DISP_X = 0.01f;
           
    static const float EYE_DISP_X = 0.023f;
    static const float EYE_DISP_Y = 0.02f;
           
    static const float FRAME_STEP = 1.0f/6.0f;
    
    static const GLfloat defaultTriangleVerticesData[] = {
	    // X, Y, Z, U, V
    		
    	// normal, frame0
	    -BASE_SIZE, 3 * BASE_SIZE, 0, 0.0f, -1.0f,
	    -BASE_SIZE, -BASE_SIZE, 0, 0.0f, 1.0f,
	    3 * BASE_SIZE, -BASE_SIZE, 0, FRAME_STEP, 1.0f,
	    
	    // normal, frame1
	    -BASE_SIZE, 3 * BASE_SIZE, 0, FRAME_STEP, -1.0f,
	    -BASE_SIZE, -BASE_SIZE, 0, FRAME_STEP, 1.0f,
	    3 * BASE_SIZE, -BASE_SIZE, 0, 2*FRAME_STEP, 1.0f,
	    
		 // frightened, frame0
		-BASE_SIZE, 3 * BASE_SIZE, 0, 2*FRAME_STEP, -1.0f,
	    -BASE_SIZE, -BASE_SIZE, 0, 2*FRAME_STEP, 1.0f,
	    3 * BASE_SIZE, -BASE_SIZE, 0, 3*FRAME_STEP, 1.0f,
	    
	    // frightened, frame1
	    -BASE_SIZE, 3 * BASE_SIZE, 0, 3*FRAME_STEP, -1.0f,
	    -BASE_SIZE, -BASE_SIZE, 0, 3*FRAME_STEP, 1.0f,
	    3 * BASE_SIZE, -BASE_SIZE, 0, 4*FRAME_STEP, 1.0f,
	    
	    // frightened-blink, frame0
		-BASE_SIZE, 3 * BASE_SIZE, 0, 4*FRAME_STEP, -1.0f,
	    -BASE_SIZE, -BASE_SIZE, 0, 4*FRAME_STEP, 1.0f,
	    3 * BASE_SIZE, -BASE_SIZE, 0, 5*FRAME_STEP, 1.0f,
	    
	    // frightened-blink, frame1
	    -BASE_SIZE, 3 * BASE_SIZE, 0, 5*FRAME_STEP, -1.0f,
	    -BASE_SIZE, -BASE_SIZE, 0, 5*FRAME_STEP, 1.0f,
	    3 * BASE_SIZE, -BASE_SIZE, 0, 1.0f, 1.0f,
	    
	    // eye
	    BASE_EYE_DISP_X - BASE_EYE_SIZE, 3 * BASE_EYE_SIZE, 0, 0.0f, -1.0f,
	    BASE_EYE_DISP_X - BASE_EYE_SIZE, -BASE_EYE_SIZE, 0, 0.0f, 1.0f,
	    BASE_EYE_DISP_X + 3 * BASE_EYE_SIZE, -BASE_EYE_SIZE, 0, 2.0f, 1.0f,
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
                "uniform vec4 basicColor;\n"
                "uniform vec4 targetColor;\n"
                "void main() {\n"
                "  vec4 col = texture2D(sTexture, vTextureCoord);\n"
                "  if (col == basicColor)\n"
                "    col = targetColor;\n"
                "  gl_FragColor = col;\n"
                "}\n";
    GhostView::GhostView( GhostModel* ghostModel, GLColor &ghostColor ) : ghostModel(ghostModel), GHOST_COLOR(ghostColor) {
        phase = 0.0f;
        forwardAnimation = 1;
        
        mProgram            = -1;
        mTextureID          = -1;
        eyeTextureID        = -1;
        muMVPMatrixHandle   = -1;
        maPositionHandle    = -1;
        maTextureHandle     = -1;
        basicColorHandle    = -1;
        targetColorHandle   = -1;
        
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
        
        basicColorHandle = glGetUniformLocation(mProgram, "basicColor");
        GLESUtils::checkGlError("glGetUniformLocation basicColor");
        if (basicColorHandle == -1) {
            return;
        }
        
        targetColorHandle = glGetUniformLocation(mProgram, "targetColor");
        GLESUtils::checkGlError("glGetUniformLocation targetColor");
        if (targetColorHandle == -1) {
            return;
        }

        /*
         * Create our texture. This has to be done each time the
         * surface is created.
         */

        GLuint textures[2] = {0, 0};
        glGenTextures(2, &textures[0]);

        mTextureID = textures[0];
        glBindTexture(GL_TEXTURE_2D, mTextureID);

        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        // resource id is from auto generated java file "R.java"
        GameRenderer::createGlTextureFromResource(0x7f040000, mTextureID);
        
        eyeTextureID = textures[1];
        glBindTexture(GL_TEXTURE_2D, eyeTextureID);

        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        // resource id is from auto generated java file "R.java"
        GameRenderer::createGlTextureFromResource(0x7f040001, eyeTextureID);
    }
    
    void GhostView::handleGhostEaten (IntPosition* ghostPosition, int score) {
		/*ghostEatenTime = SystemClock.uptimeMillis();
		ghostEatenScore = score;
		ghostEatenPosition = ghostPosition;*/
	}
    
    void GhostView::draw(glm::mat4 mProjMatrix, glm::mat4 mVMatrix) {
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
        float currentMillis = floor(now->tv_usec / 1000.0f);
        long time2 = fmod(currentMillis, 640L);
        long time = fmod(time2, 160L); 
        
        int modeOffset = 0;
        int frameOffset = 0;
        char drawEyes = true;
        char drawBody = true;
        
        if (ghostModel->getIsFrightened()) {
        	long timeLeft = ghostModel->timeInFrightenStateLeft(currentMillis);
        	if ((timeLeft > 0) && (timeLeft < FRIGHTEN_BLINK_DURATION) && (time2 < 320)) {
        		modeOffset = 12;
        	} else {
        		modeOffset = 6;
        	}
        	drawEyes = false;
        } else if (ghostModel->getIsReturning()) {
        	drawBody = false;
        }
        
        if (time > 80) {
        	frameOffset = 3;
        }
        
        float moveAnimationPhase = ghostModel->getMoveAnimationPhase();
        IntPosition* currentPathPosition = ghostModel->getCurrentPosition();
        IntPosition* positionDisplacement = ghostModel->getPositionDisplacement();
        FloatPosition* homeDisp = ghostModel->getHomeDisp();
        
        GLfloat positionX = ViewConstants::LEFT_TOP_CORNER->getX() + ViewConstants::PATH_CELL_SIZE_X * ((float)currentPathPosition->getX() - 1.0f + homeDisp->getX() + (float)positionDisplacement->getX() * moveAnimationPhase);
        GLfloat positionY = ViewConstants::LEFT_TOP_CORNER->getY() - ViewConstants::PATH_CELL_SIZE_Y * ((float)currentPathPosition->getY() - 1.0f + homeDisp->getY() + (float)positionDisplacement->getY() * moveAnimationPhase);
        if (drawBody) {
	        mMMatrix = glm::mat4(1.0f);
            mMMatrix = glm::translate(
                mMMatrix,
                glm::vec3(
                    positionX,
                    positionY,
                    0
                )
            );

	        mMVPMatrix = mProjMatrix * mVMatrix * mMMatrix;
	        
	        glUniformMatrix4fv(muMVPMatrixHandle, 1, false, glm::value_ptr(mMVPMatrix));
	        glUniform4f(basicColorHandle, 1.0f, 0.0f, 0.0f, 1.0f);
	        glUniform4fv(targetColorHandle, 1, GHOST_COLOR);
	        glDrawArrays(GL_TRIANGLES, modeOffset + frameOffset, 3);
	        GLESUtils::checkGlError("glDrawArrays");
        }
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        if (drawEyes) {
	        glBindTexture(GL_TEXTURE_2D, eyeTextureID);
	        
	        mMMatrix = glm::mat4(1.0f);
            mMMatrix = glm::translate(
                mMMatrix,
                glm::vec3(
                    positionX + EYE_DISP_X,
                    positionY + EYE_DISP_Y,
                    0
                )
            );
            mMMatrix = glm::rotate(
                mMMatrix,
                ghostModel->getAngle(),
                glm::vec3(
                    0,
                    0,
                    1
                )
            );
	        mMVPMatrix = mProjMatrix * mVMatrix * mMMatrix;
	        
	        glUniformMatrix4fv(muMVPMatrixHandle, 1, false, glm::value_ptr(mMVPMatrix));
	        glDrawArrays(GL_TRIANGLES, 18, 3);
	        GLESUtils::checkGlError("glDrawArrays");
	        /////////////////////////////////////////////////////////////////////////////////////////////////////
	        mMMatrix = glm::mat4(1.0f);
            mMMatrix = glm::translate(
                mMMatrix,
                glm::vec3(
                    positionX - EYE_DISP_X,
                    positionY + EYE_DISP_Y,
                    0
                )
            );
            mMMatrix = glm::rotate(
                mMMatrix,
                ghostModel->getAngle(),
                glm::vec3(
                    0,
                    0,
                    1
                )
            );
	        mMVPMatrix = mProjMatrix * mVMatrix * mMMatrix;
	        
	        glUniformMatrix4fv(muMVPMatrixHandle, 1, false, glm::value_ptr(mMVPMatrix));
	        glDrawArrays(GL_TRIANGLES, 18, 3);
	        GLESUtils::checkGlError("glDrawArrays");
        }
        /*long delta = currentMillis - ghostEatenTime;
        if (delta < GHOST_EATEN_DISPLAY_TIME) {
        	ghostEatenTextString.draw(
    			"" + ghostEatenScore,
    			ViewConstants::LEFT_TOP_CORNER->getX() + ViewConstants::PATH_CELL_SIZE_X * ((float)ghostEatenPosition->getX() - 1.5f),
    			ViewConstants::LEFT_TOP_CORNER->getY() - ViewConstants::PATH_CELL_SIZE_Y * ((float)ghostEatenPosition->getY() - 0.5f),
    			mProjMatrix,
    			mVMatrix
        	);
        }*/
    }
}