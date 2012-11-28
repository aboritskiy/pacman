#ifndef GAME_RENDERER
#define GAME_RENDERER

#include <jni.h>
#include <time.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

namespace game{
    class GameModel;
    class GameView;
    class MotionDirection;
    class TextString;
    
    class GameRenderer {
            static JNIEnv*      env;
            static jobject      jparent;
            static JavaVM*      jvm;
            
            GameModel*  gameModel;
            GameView*   gameView;
            
            TextString* textString;
            
            glm::mat4 mProjMatrix;
            glm::mat4 mVMatrix;
            
            long getTime();
            timespec time;
            struct timeval timeValue;

            long lastTime;

        public:
            GameRenderer(
                JNIEnv* env,
                jobject jparent,
                JavaVM* jvm
            );
            
            void onSurfaceCreated();
            void onDrawFrame();
            void onSurfaceChanged(int width, int height);
            void setMotionDirection( MotionDirection* md );
            
            static void createGlTextureFromResource (int resourceId, int textureId);
            static JavaVM* getJVM();
            static jobject getJParent();
    };
}

#endif 
