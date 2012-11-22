#ifndef FLOAT_POSITION
#define FLOAT_POSITION

namespace game{
 
    class FloatPosition {
        private:
        float x;
        float y;
    
        public:
        FloatPosition( float x, float y );
        char equals(FloatPosition* position);
        float getX();
        float getY();
    };
}

#endif 