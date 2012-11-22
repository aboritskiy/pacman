#ifndef INT_POSITION
#define INT_POSITION

namespace game{
 
    class IntPosition {
        private:
        int x;
        int y;
    
        public:
        IntPosition( int x, int y );
        char equals(IntPosition* position);
        int getX();
        int getY();
    };
}

#endif 