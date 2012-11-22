#include <android/log.h>  

#include "IntPosition.h"
  
#define DEBUG_TAG "NDK_IntPosition"
  
namespace game{
    IntPosition::IntPosition( int x, int y ) {
        this->x = x;
        this->y = y;
    }

    char IntPosition::equals( IntPosition* position) {
        return ( (this->x == position->x) && (this->y == position->y) );
    }
    
    int IntPosition::getX () {
        return x;
    }
    
    int IntPosition::getY () {
        return y;
    }
}