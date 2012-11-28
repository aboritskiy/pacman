#include <jni.h>  
#include <string.h>  
#include <android/log.h>
#include <math.h>

#include "GameRenderer.h"
#include "model/GameModel.h"
#include "view/GameView.h"
#include "data/MotionDirection.h"
#include "view/text/TextString.h"

#define  LOG_TAG    "NDK_GameRenderer"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

static void printGLString(const char *name, GLenum s) {
    const char *v = (const char *) glGetString(s);
    LOGI("GL %s = %s\n", name, v);
}

static void checkGlError(const char* op) {
    for (GLint error = glGetError(); error; error
            = glGetError()) {
        LOGI("after %s() glError (0x%x)\n", op, error);
    }
}
 
namespace game{

    JNIEnv* GameRenderer::env = 0;
    jobject GameRenderer::jparent = 0;
    JavaVM* GameRenderer::jvm = 0;

    GameRenderer::GameRenderer(JNIEnv* env, jobject jparent, JavaVM* jvm) {
        GameRenderer::env = env;
        GameRenderer::jparent = env->NewGlobalRef(jparent);
        GameRenderer::jvm = jvm;

        lastTime = 0L;
    }

    void GameRenderer::onSurfaceCreated() {
        gameModel = new GameModel ();
		gameView = new GameView (gameModel);
        
        mVMatrix = glm::lookAt(
            glm::vec3(0,0,2.2f),//camera
            glm::vec3(0,0,0),   // target
            glm::vec3(0,1,0)    // up-vector
        );
        
        gameModel->resetGame(getTime());
    }

    void GameRenderer::onDrawFrame() {
        glEnable (GL_BLEND);
    	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        checkGlError("glClearColor");
        glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        checkGlError("glClear");
        
        gameModel->step(getTime());
        gameView->draw(mProjMatrix, mVMatrix);
    }
    
    void GameRenderer::onSurfaceChanged(int width, int height) {
        LOGD("onSurfaceChanged");
        
        glViewport(0, 0, width, height);
        float ratio = (float) width / height;
       	if (ratio > 1) {
            mProjMatrix = glm::frustum( -ratio, ratio, -1.1f, 0.9f, 2.0f, 4.0f );
       	} else {
            mProjMatrix = glm::frustum( -1.0f, 1.0f, -1.1f/ratio, 0.9f/ratio, 2.0f, 4.0f );
       	}
    }
    
    void GameRenderer::setMotionDirection( MotionDirection* md ) {
        gameModel->setMotionDirection( md );
    }
    
    void GameRenderer::createGlTextureFromResource (int resourceId, int textureId) {
        JNIEnv *env = NULL;
        if (jvm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
            LOGE("Can not get the Java Env pointer");
        }
    
        /**
        * TODO: get rid of resourceId and replace to resource name if it is possible
        */
        jclass jclassname = env->GetObjectClass(jparent);
        jmethodID mid = env->GetMethodID(jclassname, "createGLTextureFromResource", "(II)V");
        env->CallVoidMethod(jparent, mid, resourceId, textureId);
    }
    
    JavaVM* GameRenderer::getJVM () {
        return jvm;
    }
    
    jobject GameRenderer::getJParent () {
        return jparent;
    }
    
    long GameRenderer::getTime() {
        gettimeofday( &timeValue, NULL );
        return floor(timeValue.tv_usec / 1000) + timeValue.tv_sec * 1000;
    }
}
