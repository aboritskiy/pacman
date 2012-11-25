#include <string.h>  
#include <android/log.h>  

#include "LivesView.h"
#include "model/GameModel.h"
#include "view/text/TextString.h"

#include "GameRenderer.h"
#include "GLESUtils.h"
#include "FieldView.h"
#include "data/FloatPosition.h"

#define  LOG_TAG    "NDK_LivesView"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
  
namespace game{
    static GLColor LABEL_COLOR = { 1.0f, 1.0f, 1.0f, 1.0f };
    static GLColor VALUE_COLOR = { 1.0f, 1.0f, 0.0f, 1.0f };

	static FloatPosition* BASE_SIZE = new FloatPosition(0.1f, 0.1f);
	static FloatPosition* BASE_POSITION = new FloatPosition(0.35f, -1.16f);
	static FloatPosition* OFFSET = new FloatPosition(BASE_SIZE->getX() * 1.1f, BASE_SIZE->getY());

    static GLfloat mTriangleVerticesData[] = {
		// X, Y, Z, U, V
		0.0f, BASE_SIZE->getY(), 0, 0.0f, 0.0f,
		0.0f, 0.0f, 0, 0.0f, 1.0f,
		BASE_SIZE->getX(), 0.0f, 0, 1.0f, 1.0f,
		BASE_SIZE->getX(), BASE_SIZE->getY(), 0, 1.0f, 0.0f,
		0.0f, BASE_SIZE->getY(), 0, 0.0f, 0.0f
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

    LivesView::LivesView( GameModel* gameModel ) : gameModel ( gameModel ) {
        textString = new TextString();
		textString->loadFont("typodermic_foo_regular.ttf", 50, 2, 2);
		textString->setScale(0.003f);

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
		GameRenderer::createGlTextureFromResource(0x7f040003, mTextureID);
    }
    
    void LivesView::draw(glm::mat4 mProjMatrix, glm::mat4 mVMatrix) {
        textString->setColor(LABEL_COLOR);
		textString->draw("Lives", 0, -1.2f, mProjMatrix, mVMatrix);

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

	    int lives = gameModel->getLives();

		for (int i = 0; i < lives; i++) {
			mMMatrix = glm::mat4(1.0f);
			mMMatrix = glm::translate(
				mMMatrix,
				glm::vec3(
					BASE_POSITION->getX() + i * OFFSET->getX(),
					BASE_POSITION->getY(),
					0
				)
			);
			mMVPMatrix = mProjMatrix * mVMatrix * mMMatrix;

			glUniformMatrix4fv(muMVPMatrixHandle, 1, false, glm::value_ptr(mMVPMatrix));
			glDrawArrays(GL_TRIANGLES, 0, 3);
			GLESUtils::checkGlError("glDrawArrays1");
			glDrawArrays(GL_TRIANGLES, 2, 3);
			GLESUtils::checkGlError("glDrawArrays2");
		}
    }
}
