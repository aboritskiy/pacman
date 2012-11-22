#include <string.h>  
#include <android/log.h>  

#include "TextureRegion.h"

#define  LOG_TAG    "NDK_TextureRegion"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
  
namespace game{

    TextureRegion::TextureRegion(float texWidth, float texHeight, float x, float y, float width, float height)  {
        this->u1 = x / texWidth;
        this->v1 = y / texHeight;
        this->u2 = this->u1 + ( width / texWidth );
        this->v2 = this->v1 + ( height / texHeight );
        
        LOGI("%f %f %f %f", u1, v1, u2, v2);
    }
}