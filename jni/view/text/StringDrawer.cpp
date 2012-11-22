#include <string.h>  
#include <android/log.h>  

#include "StringDrawer.h"
#include "GLESUtils.h"
#include "view/text/TextureRegion.h"

#define  LOG_TAG    "NDK_StringDrawer"
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
        "uniform vec4 vColor;\n"
        "uniform sampler2D sTexture;\n"
        "void main() {\n"
        "  gl_FragColor = vColor;\n"
        "  gl_FragColor.a = gl_FragColor.a * texture2D(sTexture, vTextureCoord).a;\n"
        "}\n";

    StringDrawer::StringDrawer()  {
        color[0] = 1.0f;
        color[1] = 1.0f;
        color[2] = 1.0f;
        color[3] = 1.0f;
        
        mProgram            = -1;
        muMVPMatrixHandle   = -1;
        maPositionHandle    = -1;
        maTextureHandle     = -1;
        colorHandle         = -1;
    
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
        
        colorHandle = glGetUniformLocation(mProgram, "vColor");
        GLESUtils::checkGlError("glGetUniformLocation vColor");
        if (colorHandle == -1) {
            return;
        }
    }
    
    void StringDrawer::setColor (float &red, float &green, float &blue, float &alpha) {
        color[0] = red;
		color[1] = green;
		color[2] = blue;
		color[3] = alpha;
    }
    
    void StringDrawer::setColor (const GLColor &color) {
		this->color[0] = color[0];
		this->color[1] = color[1];
		this->color[2] = color[2];
		this->color[3] = color[3];
	}
    
    void StringDrawer::draw (glm::mat4 mProjMatrix, glm::mat4 mVMatrix, float x, float y, float width, float height, TextureRegion* textureRegion) {
		glUseProgram(mProgram);

		glActiveTexture(GL_TEXTURE0);

        glVertexAttribPointer(
			maPositionHandle,
			3,
			GL_FLOAT,
			false,
			TRIANGLE_VERTICES_DATA_STRIDE_BYTES,
			&mTriangleVerticesData[0]
        );

        glEnableVertexAttribArray(maPositionHandle);
        glVertexAttribPointer(
        	maTextureHandle,
        	2,
        	GL_FLOAT,
        	false,
        	TRIANGLE_VERTICES_DATA_STRIDE_BYTES,
        	&mTriangleVerticesData[3]
        );
        glEnableVertexAttribArray(maTextureHandle);
        
        mMMatrix = glm::mat4(1.0f);
        mMMatrix = glm::translate(
            mMMatrix,
            glm::vec3(
                x,
                y,
                0
            )
        );
        mMVPMatrix = mProjMatrix * (mVMatrix * mMMatrix);

        glUniformMatrix4fv(muMVPMatrixHandle, 1, false, glm::value_ptr(mMVPMatrix));
        glUniform4f(colorHandle, color[0], color[1], color[2], color[3]);
        
        float halfWidth = width / 2.0f;  
        float halfHeight = height / 2.0f;
        float x1 = -halfWidth;           
        float y1 = -halfHeight;          
        float x2 = halfWidth;            
        float y2 = halfHeight;           
        
        mTriangleVerticesData[0] = x1;
        mTriangleVerticesData[1] = y1;
        mTriangleVerticesData[3] = textureRegion->u1;
        mTriangleVerticesData[4] = textureRegion->v2;
        mTriangleVerticesData[5] = x2;
        mTriangleVerticesData[6] = y1;
        mTriangleVerticesData[8] = textureRegion->u2;
        mTriangleVerticesData[9] = textureRegion->v2;
        mTriangleVerticesData[10] = x2;
        mTriangleVerticesData[11] = y2;
        mTriangleVerticesData[13] = textureRegion->u2;
        mTriangleVerticesData[14] = textureRegion->v1;
        mTriangleVerticesData[15] = x1;
        mTriangleVerticesData[16] = y2;
        mTriangleVerticesData[18] = textureRegion->u1;
        mTriangleVerticesData[19] = textureRegion->v1;
        mTriangleVerticesData[20] = x1;
        mTriangleVerticesData[21] = y1;
        mTriangleVerticesData[23] = textureRegion->u1;
        mTriangleVerticesData[24] = textureRegion->v2;

        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawArrays(GL_TRIANGLES, 2, 3);
	}
}
