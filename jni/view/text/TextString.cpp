#include <string.h>  
#include <android/log.h>
#include <jni.h>

#include "TextString.h"
#include "GLESUtils.h"
#include "TextureRegion.h"
#include "StringDrawer.h"
#include "GameRenderer.h"

#define  LOG_TAG    "NDK_TextString"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
  
namespace game{

    TextString::TextString() {
        LOGI("TextString constructor");
		padX = 0;
		padY = 0;
	
		fontHeight = 0.0f;
		fontAscent = 0.0f;
		fontDescent = 0.0f;
	
		textureId = -1;
		textureSize = 0;
	
		charWidthMax = 0;
		charHeight = 0;
	
		cellWidth = 0;
		cellHeight = 0;
		rowCnt = 0;
		colCnt = 0;
	
		scaleX = 1.0f;
		scaleY = 1.0f;
		spaceX = 0.0f;
		
		stringDrawer = new StringDrawer();
    }
    
    /**
    * this method needs Android JDK to work normally
    * and can be a very bad surprise to the one who
    * decided to port this code.
    * Therefore some indication or abstration layer is needed!
    */
    char TextString::loadFont (const char* fileName, const int height, const int padX, const int padY) {
        LOGI("loadFont begin");
        this->padX = padX;
        this->padY = padY;
        
        JNIEnv *env = NULL;
        jobject jparent = GameRenderer::getJParent();
        if (GameRenderer::getJVM()->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
            LOGI("Can not get the Java Env pointer");
        }
        
        jstring jsFileName = env->NewStringUTF( fileName );
        
        jmethodID mid = env->GetMethodID(env->GetObjectClass(jparent), "loadFont", "(Ljava/lang/String;III)Lru/tonybo/pacmanJNI/game/FontParameters;");
        
        jobject jFontParameters = env->CallObjectMethod(jparent, mid, jsFileName, height, padX, padY);
        LOGI("loadFont end");

    	/*LOGI("Begin Java call");
        JNIEnv *env = NULL;
        jobject jparent = GameRenderer::getJParent();
        if (GameRenderer::getJVM()->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
            LOGI("Can not get the Java Env pointer");
        }
        jmethodID mid = env->GetMethodID(env->GetObjectClass(jparent), "loadFont", "()I");
        jint jTextureId = env->CallIntMethod(jparent, mid);
        this->textureId = jTextureId;
        LOGI("textureId %i", textureId);
        */
        
        jclass jcFontParameters = env->GetObjectClass(jFontParameters);

        this->fontHeight = env->GetFloatField( jFontParameters, env->GetFieldID( jcFontParameters, "fontHeight", "F"));
        this->fontAscent = env->GetFloatField( jFontParameters, env->GetFieldID( jcFontParameters, "fontAscent", "F"));
        this->fontDescent = env->GetFloatField( jFontParameters, env->GetFieldID( jcFontParameters, "fontDescent", "F"));
//        this->charHeight = env->GetFloatField( jFontParameters, env->GetFieldID( jcFontParameters, "charHeight", "F"));
        
        this->textureId = env->GetIntField( jFontParameters, env->GetFieldID( jcFontParameters, "textureId", "I"));
        this->textureSize = env->GetIntField( jFontParameters, env->GetFieldID( jcFontParameters, "textureSize", "I"));
        this->cellWidth = env->GetIntField( jFontParameters, env->GetFieldID( jcFontParameters, "cellWidth", "I"));
        this->cellHeight = env->GetIntField( jFontParameters, env->GetFieldID( jcFontParameters, "cellHeight", "I"));
        
        LOGI("fontHeight %f", fontHeight);
        LOGI("fontAscent %f", fontAscent);
        LOGI("fontDescent %f", fontDescent);
        
        LOGI("textureId %i", textureId);
        LOGI("textureSize %i", textureSize);
        LOGI("cellWidth %i", cellWidth);
        LOGI("cellHeight %i", cellHeight);
        
        jfloatArray jfaCharWidths = (jfloatArray)env->GetObjectField( jFontParameters, env->GetFieldID( jcFontParameters, "charWidths", "[F"));
        jboolean isCopy = JNI_TRUE;
        jfloat* jfCharWidths = env->GetFloatArrayElements(jfaCharWidths, &isCopy);
        for (int i = 0; i < CHAR_END; i++) {
            charWidths[i] = jfCharWidths[i];
            LOGI("charWidth[%i]: %f", i, charWidths[i]);
        }
        env->ReleaseFloatArrayElements( jfaCharWidths, jfCharWidths, JNI_ABORT);

        float x = 0;
        float y = 0;
        for ( int c = 0; c < CHAR_CNT; c++ ){
            LOGI("charRgn %i:",c);
			charRgn[c] = new TextureRegion( textureSize, textureSize, x, y, cellWidth-1, cellHeight-1 );
		 	x += cellWidth;
		 	if ( (x + cellWidth) > textureSize ){
		 		x = 0;
		 		y += cellHeight;
		 	}
		}
        
        return 0;
    }
    
    void TextString::setColor (float &red, float &green, float &blue, float &alpha) {
        stringDrawer->setColor(red, green, blue, alpha);
    }
    
    void TextString::setColor (const GLColor &color) {
		stringDrawer->setColor(color);
	}
    
    void TextString::setScale (float scale) {
		scaleX = scaleY = scale;
	}
    
    void TextString::draw(const char* text, float x, float y, const glm::mat4 mProjMatrix, const glm::mat4 mVMatrix) {
        glBindTexture(GL_TEXTURE_2D, textureId);
        
	    float chrHeight = cellHeight * scaleY;
	    float chrWidth = cellWidth * scaleX;  
	    x += ( chrWidth / 2.0f ) - ( padX * scaleX );  
	    y += ( chrHeight / 2.0f ) - ( padY * scaleY );
        int i = 0;
        while ((text[i] != '\0') && (i < 255)) {
            int c = (int)text[i] - CHAR_START;
            if ( c < 0 || c >= CHAR_CNT )                
                c = CHAR_UNKNOWN;
            stringDrawer->draw(mProjMatrix, mVMatrix, x, y, chrWidth, chrHeight, charRgn[c]);
            x += ( charWidths[c] + spaceX ) * scaleX;
            i++;
	    }
    }
}
