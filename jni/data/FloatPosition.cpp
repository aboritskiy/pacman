#include <android/log.h>  

#include "FloatPosition.h"
  
#define DEBUG_TAG "NDK_FloatPosition"
  
namespace game{
    FloatPosition::FloatPosition( float x, float y ) {
        this->x = x;
        this->y = y;
    }

    char FloatPosition::equals( FloatPosition* position) {
        return ( (this->x == position->x) && (this->y == position->y) );
    }
    
    float FloatPosition::getX () {
        return x;
    }
    
    float FloatPosition::getY () {
        return y;
    }
}